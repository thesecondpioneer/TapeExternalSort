#include <iostream>
#include "TapeSorter.h"

void binaryWriteInts(const std::string& inputFileName, const std::string& outputFileName){
    std::ifstream fin(inputFileName);
    std::ofstream fout(outputFileName, std::ios::binary);
    int a;
    while(fin){
        fin >> a;
        if(fin) {
            fout.write(reinterpret_cast<const char *>(&a), sizeof(int));
        }
    }
    fin.close();
    fout.close();
}

void binaryReadInts(const std::string& inputFileName, const std::string& outputFileName){
    std::ifstream fin(inputFileName, std::ios::binary);
    std::ofstream fout(outputFileName);
    int a;
    while(fin.read(reinterpret_cast<char *>(&a), sizeof(int))){
        fout << a << ' ';
    }
    fin.close();
    fout.close();
}

void clearTempFiles(const char* outputFileName){
    int i = 0;
    while(std::ifstream((("../tmp/temp_tape" + std::to_string(i) + ".dat").c_str())).good()){
        std::remove(("../tmp/temp_tape" + std::to_string(i) + ".dat").c_str());
        i++;
    }
    std::remove("../input_tape.dat");
    std::remove("../output_tape.dat");
    std::remove(outputFileName);
}

std::string findLastTempFile(){
    int i = 0;
    while(std::ifstream((("../tmp/temp_tape" + std::to_string(i) + ".dat").c_str())).good()){
        i++;
    }
    return "../tmp/temp_tape" + std::to_string(i-1) + ".dat";
}

int main(int argc, char** argv) {
    if (argc != 3){
        std::cout << "Usage: <input file name> <output file name>";
        return 0;
    }
    clearTempFiles(argv[2]); //clear the temporary files in tmp directory, also the input .dat file and output files
                                         //otherwise, we would be appending to them.
    binaryWriteInts(argv[1], "../input_tape.dat");
    FileTapeDevice td("../input_tape.dat");
    Tape tape("../tape.config", &td);
    TapeSorter sorter(&tape, 3000000);
    sorter.sort();
    binaryReadInts(findLastTempFile(), argv[2]); //the last temporary file is our sorted file!
    return 0;
}
