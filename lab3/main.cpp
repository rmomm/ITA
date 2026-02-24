#include "BitStream.h"
#include <iostream>
#include <iomanip> 

void printBits(uint8_t* arr, size_t bitCount) {
    for (size_t i = 0; i < bitCount; ++i) {
        size_t byteIndex = i / 8;
        size_t bitIndex = i % 8;
        uint8_t bit = (arr[byteIndex] >> bitIndex) & 1;
        cout << (int)bit;
    }
}

void printHex(uint8_t* arr, size_t byteCount) {
    for (size_t i = 0; i < byteCount; ++i) {
        cout << hex << setw(2) << setfill('0') << (int)arr[i] << " ";
    }
    cout << endl;
}

int main() {
    try {
        uint8_t a1[2] = { 0xE1, 0x01 }; 
        uint8_t a2[2] = { 0xEE, 0x00 }; 
        uint8_t a3[1] = { 0b10101010 }; 
        uint8_t a4[2] = { 0xFF, 0x00 }; 
        uint8_t a5[3] = { 0b00011100, 0b10100101, 0b11100011 }; 

        BitStream writer("test.bin", BitStream::Write);
        writer.WriteBitSequence(a1, 9);
        writer.WriteBitSequence(a2, 9);
        writer.WriteBitSequence(a3, 5);
        writer.WriteBitSequence(a4, 12);
        writer.WriteBitSequence(a5, 13);
        writer.Close();

        BitStream reader("test.bin", BitStream::Read);
        uint8_t b1[2], b2[2], b3[1], b4[2], b5[3];

        reader.ReadBitSequence(b1, 9);
        reader.ReadBitSequence(b2, 9);
        reader.ReadBitSequence(b3, 5);
        reader.ReadBitSequence(b4, 12);
        reader.ReadBitSequence(b5, 13);
        reader.Close();

        cout << "Test 1: "; printBits(b1, 9); cout << "   Hex: "; printHex(b1, 2);
        cout << "Test 2: "; printBits(b2, 9); cout << "   Hex: "; printHex(b2, 2);
        cout << "Test 3: "; printBits(b3, 5); cout << "   Hex: "; printHex(b3, 1);
        cout << "Test 4: "; printBits(b4, 12); cout << "   Hex: "; printHex(b4, 2);
        cout << "Test 5: "; printBits(b5, 13); cout << "   Hex: "; printHex(b5, 3);
    }
    catch (const exception& ex) {
        cout << "Error: " << ex.what() << endl;
    }

    return 0;
}