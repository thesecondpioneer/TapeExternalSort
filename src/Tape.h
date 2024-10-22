#ifndef TAPE_H
#define TAPE_H
#include <fstream>
#include <vector>
#include <string>
#include "TapeDevice.h"
#include "FileTapeDevice.h"

std::vector<double> readTapeConfig(const std::string &fileName);

class Tape {
protected:
    TapeDevice* device;

    double _readDelay, _writeDelay, _rewindDelay, _moveDelay;

    double _totalDelay = 0;
public:
    Tape() = default;

    Tape(std::string config, TapeDevice* device);

    Tape(const Tape &) = default;

    void moveTo(unsigned long long int ind); //move to a certain element on tape

    void moveNext(); //move to the next element on tape

    void movePrev(); //move to the previos element on tape

    void rewind(); //move to the beginning of the tape

    int read(); //read data at the current position of the head

    bool isEnd(); //check if at the end of tape

    void write(int data); //write data to the current head position

    void stop(); //stop operating on the tape
};
#endif