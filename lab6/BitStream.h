#pragma once
#pragma once
#include <fstream>
#include <string>
#include <stdexcept>
using namespace std;

class BitStream {
public:
    enum Mode { 
        Read,
        Write 
    };

    BitStream(const string& filename, Mode m);
    ~BitStream();

    void WriteBitSequence(const uint8_t* data, size_t bitCount);
    void ReadBitSequence(uint8_t* data, size_t bitCount);
    void Flush();
    void Close();

    void SeekWrite(size_t pos);
    void SeekRead(size_t pos);

private:
    Mode mode;
    uint8_t buffer;
    int bitPosition;
    fstream file;
};