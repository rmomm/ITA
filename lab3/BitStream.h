#pragma once
#include <fstream>
#include <cstdint>
#include <string>
using namespace std;

class BitStream {
public:
    enum Mode {
        Read,
        Write
    };

    BitStream(const string& filename, Mode mode);
    ~BitStream();

    void WriteBitSequence(const uint8_t* data, size_t bitCount);
    void ReadBitSequence(uint8_t* data, size_t bitCount);

    void Close();

private:
    fstream file;
    Mode mode;

    uint8_t buffer;     
    int bitPosition;     

    void Flush();
};