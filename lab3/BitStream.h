#pragma once
#include <fstream>
#include <string>
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
    void Close();

private:
    void Flush();

    Mode mode;
    fstream file;
    uint8_t buffer;      
    size_t bitPosition;  
};