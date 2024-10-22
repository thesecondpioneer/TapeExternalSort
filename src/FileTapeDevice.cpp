#include "FileTapeDevice.h"
#include <iostream>

FileTapeDevice::FileTapeDevice(const std::string &fileName) {
    _pos = 0;
    _input_file.open(fileName, std::ios::binary);
    _output_file.open(fileName, std::ios::binary | std::ios::app);
    _output_file.seekp(0);
    _input_file.seekg(0);
}

void FileTapeDevice::writeData(int data) {
    _output_file.write(reinterpret_cast<char *>(&data), sizeof(int));
    _output_file.seekp(_pos * sizeof(int));
}

int FileTapeDevice::readData() {
    int result;
    if(!_input_file.read(reinterpret_cast<char *>(&result), sizeof(int))){
        _isEnd = true;
    }
    _input_file.seekg(_pos * sizeof(int));
    return result;
}

void FileTapeDevice::moveNext() {
    if (!_isEnd) {
        _pos += 1;
        _output_file.seekp(_pos * sizeof(int));
        _input_file.seekg(_pos * sizeof(int));
    }
}

void FileTapeDevice::movePrev() {
    if(_isEnd) _isEnd = false;
    _pos -= 1;
    _output_file.seekp(_pos * sizeof(int));
    _input_file.seekg(_pos * sizeof(int));
}

FileTapeDevice::~FileTapeDevice() {
    _input_file.close();
    _output_file.close();
}

void FileTapeDevice::stop() {
    _input_file.close();
    _output_file.close();
}