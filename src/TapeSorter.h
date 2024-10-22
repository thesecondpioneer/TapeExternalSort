#ifndef TAPESORTER_H
#define TAPESORTER_H

#include "Tape.h"
#include <list>

class TapeSorter {
protected:
    Tape *_tape;
    unsigned long long int _m; //RAM limit in bytes
    long long int _tta = 0; //Amount of temporary tapes used to read the input data block-by-block
    std::vector<int> _block;

    void readBlockFromTape(); //Reads a single block from input file
    int blockPartition(long long int l, long long int r); //Partition function for quicksorting the block
    void blockQuickSort(long long int l, long long int r); //Quicksort function
    void tempWriteBlock(int ind); //Writes the block to a temporary tape
    long long int generateRuns(); //Reads all the data block-by-block, sorts it and writes to temporary files
                                  //Returns the total amount of integers stored on the tape
    void multiwayMergeTapes(const long long int &size); //Merges all the temporary files into a sorted output
public:
    TapeSorter(Tape *tape, unsigned long long int m) : _tape(tape), _m(m) {
        _block.reserve(_m / 4);
    }

    //The main method of this object, which performs quicksorts on each of the block the size of the maximum amount of RAM
    //and performs a multiway merge on them, while still obliging to the RAM limitation
    void sort();
};

#endif