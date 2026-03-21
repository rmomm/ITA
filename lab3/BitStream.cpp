#include "BitStream.h"
#include <stdexcept>

BitStream::BitStream(const string& filename, Mode m)
    : mode(m), buffer(0), bitPosition(0) {
    if (mode == Write) {
        file.open(filename, ios::binary | ios::out);
    }
    else {
        file.open(filename, ios::binary | ios::in);
    }

    if (!file.is_open()) {
        throw runtime_error("Cannot open file");
    }
}

BitStream::~BitStream() {
    Close();
}

void BitStream::Flush() {
    if (mode == Write && bitPosition > 0) {
        file.put(static_cast<char>(buffer));
        buffer = 0;
        bitPosition = 0;
    }
}

void BitStream::Close() {
    if (file.is_open()) {
        if (mode == Write) {
            Flush();
        }
        file.close();
    }
}

void BitStream::WriteBitSequence(const uint8_t* data, size_t bitCount) {
    if (mode != Write)
        throw runtime_error("Stream not opened for writing");

    for (size_t i = 0; i < bitCount; ++i) {
        size_t byteIndex = i / 8;
        size_t bitIndex = i % 8;

        uint8_t bit = (data[byteIndex] >> bitIndex) & 1;

        buffer |= (bit << bitPosition);
        bitPosition++;

        if (bitPosition == 8) {
            file.put(static_cast<char>(buffer));
            buffer = 0;
            bitPosition = 0;
        }
    }
}

void BitStream::ReadBitSequence(uint8_t* data, size_t bitCount) {
    if (mode != Read)
        throw runtime_error("Stream not opened for reading");

    size_t byteCount = (bitCount + 7) / 8;
    for (size_t i = 0; i < byteCount; ++i)
        data[i] = 0;

    for (size_t i = 0; i < bitCount; ++i) {
        if (bitPosition == 0) {
            int value = file.get();
            if (value == EOF)
                throw runtime_error("Unexpected end of file");

            buffer = static_cast<uint8_t>(value);
        }

        uint8_t bit = (buffer >> bitPosition) & 1;

        size_t byteIndex = i / 8;
        size_t bitIndex = i % 8;

        data[byteIndex] |= (bit << bitIndex);

        bitPosition++;

        if (bitPosition == 8)
            bitPosition = 0;
    }
}