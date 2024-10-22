#include "Tape.h"
#include <cmath>

//reads the config file and returns the parameters as a vector
std::vector<double> readTapeConfig(const std::string &config){
    std::vector<double> result(4);
    std::ifstream fin(config);
    std::string param;
    for(int i = 0; i < 4; i++){
        fin >> param;
        if (param == "readDelay:"){
            fin >> result[0];
        } else if (param == "writeDelay:"){
            fin >> result[1];
        } else if (param == "rewindDelay:"){
            fin >> result[2];
        } else if (param == "moveDelay:"){
            fin >> result[3];
        }
    }
    return result;
}


Tape::Tape(std::string config, TapeDevice *device) {
    std::vector<double> params = readTapeConfig(config);
    _readDelay = params[0];
    _writeDelay = params[1];
    _rewindDelay = params[2];
    _moveDelay = params[3];
    this->device = device;
}

void Tape::moveTo(unsigned long long int ind){
    unsigned long long int steps;
    if (ind < device->_pos){
        steps = device->_pos - ind;
        for(int i = 0; i < steps; i++){
            device->movePrev();
        }
    } else {
        steps = ind - device->_pos;
        for(int i = 0; i < steps; i++){
            device->moveNext();
        }
    }
    _totalDelay += steps * _moveDelay;
}

void Tape::rewind() {
    for(int i = 0; i < device->_pos; i++){
        device->movePrev();
    }
    _totalDelay += _rewindDelay;
}

int Tape::read() {
    _totalDelay += _readDelay;
    return device->readData();
}

bool Tape::isEnd(){
    return device->_isEnd;
}

void Tape::write(int data) {
    device->writeData(data);
    _totalDelay += _writeDelay;
}

void Tape::moveNext() {
    device->moveNext();
    _totalDelay += _moveDelay;
}

void Tape::movePrev() {
    device->movePrev();
    _totalDelay += _moveDelay;
}

void Tape::stop() {
    device->stop();
}