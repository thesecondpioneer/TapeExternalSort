#ifndef FILE_TAPE_DEVICE_H
#define FILE_TAPE_DEVICE_H

#include "TapeDevice.h"
#include <string>
#include <fstream>

class FileTapeDevice : public TapeDevice {
protected:
    std::ifstream _input_file;
    std::ofstream _output_file;

public:
    ~FileTapeDevice() override;

    FileTapeDevice() = default;

    FileTapeDevice(const FileTapeDevice &) = default;

    FileTapeDevice(FileTapeDevice &&) = default;

    FileTapeDevice(const std::string &fileName);

    void writeData(int data) override; //write data to the current position of the head

    int readData() override; //read data at the current position of the head

    void moveNext() override; //move to the next element of the tape

    void movePrev() override; //move to the previos element of the tape

    void stop() override; //stop operating on the tape (close the file)
};

#endif