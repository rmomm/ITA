#pragma once
#include <fstream>
#include <cstdint>
#include <string>

class BitStream {
public:
    enum Mode {
        Read,
        Write
    };

    BitStream(const std::string& filename, Mode mode);
    ~BitStream();

    void WriteBitSequence(const uint8_t* data, size_t bitCount);
    void ReadBitSequence(uint8_t* data, size_t bitCount);

    void Close();

private:
    std::fstream file;
    Mode mode;

    uint8_t buffer;     
    int bitPosition;     

    void Flush();
};