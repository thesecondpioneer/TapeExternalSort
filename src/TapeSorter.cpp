#include "TapeSorter.h"
#include "FileTapeDevice.h"
#include <sys/stat.h>

void TapeSorter::readBlockFromTape() {
    for (int i = 0; i < _m / sizeof(int); i++) {
        _block.emplace_back(_tape->read());
        if (_tape->isEnd()) {
            _block.resize(_block.size() - 1);
            return;
        }
        _tape->moveNext();
    }
}

int TapeSorter::blockPartition(long long int l, long long int r) {
    int p = _block[r];
    int i = l - 1;
    for (long long int j = l; j <= r - 1; j++) {
        if (_block[j] < p) {
            i++;
            std::swap(_block[i], _block[j]);
        }
    }
    std::swap(_block[i + 1], _block[r]);
    return i + 1;
}

void TapeSorter::blockQuickSort(long long int l, long long int r) {
    if (l < r) {
        int pi = blockPartition(l, r);
        blockQuickSort(l, pi - 1);
        blockQuickSort(pi + 1, r);
    }
}

void TapeSorter::tempWriteBlock(int ind) {
    FileTapeDevice td("../tmp/temp_tape" + std::to_string(ind) + ".dat");
    Tape tempTape("../tape.config", &td);
    for (int i = 0; i < _block.size(); i++) {
        tempTape.write(_block[i]);
        tempTape.moveNext();
    }
    tempTape.stop();
}

long long int TapeSorter::generateRuns() {
    long long int result = 0;
    while (true) {
        readBlockFromTape();
        result += _block.size();
        if (_block.empty()) {
            break;
        }
        blockQuickSort(0, _block.size() - 1);
        tempWriteBlock(_tta);
        _block.clear();
        _block.reserve(_m / sizeof(int));
        _tta++;
    }
    _block.clear();
    return result;
}

void TapeSorter::multiwayMergeTapes(const long long int &size) {
    //We need the intermediate_tapes variable to know how many new tapes we created during merging to keep track of the
    //indices in the file names
    //We need written_size variable to keep track of how many integers we have written to the tape and stop when we write all the integers
    //We need temp_index to iterate over temporary files
    long long int intermediate_tapes = 0, written_size, temp_index = 0;
    do {
        FileTapeDevice otd("../tmp/temp_tape" + std::to_string(_tta + intermediate_tapes) + ".dat");
        written_size = 0;
        Tape output_tape("../tape.config", &otd);

        std::list<Tape> tapes(_m / sizeof(int)); //Here, the tape devices are stored for reading from them in consecutive order
        auto current_tape = tapes.begin();
        std::vector<FileTapeDevice> fileDevices;
        fileDevices.reserve(_m / sizeof(int));

        //Adding the tapes to the list
        for (int i = 0; i < _m / sizeof(int); i++, temp_index++) {
            if (std::ifstream(("../tmp/temp_tape" + std::to_string(temp_index) + ".dat")).good() &&
               (temp_index != _tta + intermediate_tapes)){ //can't add the last temporary tape for sorting
                                                           // since we will be outputting to it
                fileDevices.emplace_back("../tmp/temp_tape" + std::to_string(temp_index) + ".dat");
                *current_tape = Tape("../tape.config", &fileDevices[i]);
                current_tape++;
            } else break;
        }
        tapes.resize(fileDevices.size());
        std::list<int> v(tapes.size());
        current_tape = tapes.begin();
        //Read a single int from each tape to fill up the RAM  to the limit
        for (auto i = v.begin(); i != v.end(); i++) {
            *i = current_tape->read();
            current_tape++;
        }
        while (!tapes.empty()) {
            int min_element = INT_MAX;
            auto min_v_pos = v.begin(); //The position of the minimal element of this iteration
            auto min_tapes_pos = tapes.begin(); //The position of the tape that holds the minimal element of this iteration
            current_tape = tapes.begin();
            //Finding the minimal elemnt
            for (auto i = v.begin(); i != v.end(); i++) {
                if (*i < min_element) {
                    min_element = *i;
                    min_v_pos = i;
                    min_tapes_pos = current_tape;
                }
                current_tape++;
            }
            output_tape.write(min_element);
            written_size++;
            //Read the next element instead of the minimal one we just outputted
            min_tapes_pos->moveNext();
            *min_v_pos = min_tapes_pos->read();

            output_tape.moveNext();
            //Erase the tape if it's empty
            if (min_tapes_pos->isEnd()) {
                v.erase(min_v_pos);
                tapes.erase(min_tapes_pos);
            }
        }
        intermediate_tapes++;
    } while (written_size != size);
}

void TapeSorter::sort() {
    long long int size = generateRuns();
    if (_tta > 1) multiwayMergeTapes(size); //If total tape amount is 1 one then it means all was sorted in RAM and we don't need to merge
}
