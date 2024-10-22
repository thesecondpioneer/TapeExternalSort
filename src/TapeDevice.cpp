#include "TapeDevice.h"

TapeDevice::~TapeDevice() = default;

void TapeDevice::writeData(int data) {

}

int TapeDevice::readData() {
    return 0;
}

void TapeDevice::movePrev() {

}

void TapeDevice::moveNext() {

}

unsigned long long int TapeDevice::getPos() const {
    return _pos;
}

void TapeDevice::stop() {

}