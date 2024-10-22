#ifndef TAPE_DEVICE_H
#define TAPE_DEVICE_H

class TapeDevice{
protected:
    //The current position of the read-write head
    unsigned long long int _pos;

    //The bool variable for checking if the head is at the end of the tape
    bool _isEnd = false;

public:
    virtual ~TapeDevice();

    virtual void writeData(int data);

    virtual int readData();

    virtual void movePrev();

    virtual void moveNext();

    virtual void stop();

    unsigned long long int getPos() const;

    friend class Tape;
};
#endif