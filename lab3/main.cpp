#include "BitStream.h"
#include <iostream>
#include <iomanip>
using namespace std;

void printBits(uint8_t* arr, size_t bitCount) {
    for (size_t i = 0; i < bitCount; ++i) {
        size_t byteIndex = i / 8;
        size_t bitIndex = i % 8;
        size_t totalInThisByte = ((byteIndex == bitCount / 8) && (bitCount % 8 != 0)) ? (bitCount % 8) : 8;

        uint8_t bit;
        if (totalInThisByte == 8) {
            bit = (arr[byteIndex] >> (7 - bitIndex)) & 1;
        }
        else {
            bit = (arr[byteIndex] >> (totalInThisByte - 1 - bitIndex)) & 1;
        }
        cout << (int)bit;
    }
}

void printHex(uint8_t* arr, size_t byteCount) {
    for (size_t i = 0; i < byteCount; ++i)
        cout << hex << setw(2) << setfill('0') << (int)arr[i] << " ";
    cout << endl;
}

int main() {
    try {
        uint8_t a1[2] = { 0xE1, 0x05 };       
        uint8_t a2[2] = { 0xEE, 0x00 };      
        uint8_t a3[1] = { 0x0A };          
        uint8_t a4[2] = { 0xFF, 0x00 };      
        uint8_t a5[3] = { 0x1C, 0x05, 0xCC }; 


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

        cout << "Test 1 (9 bit):  ";  printBits(b1, 9);  cout << " | Hex: "; printHex(b1, 2);
        cout << "Test 2 (9 bit):  ";  printBits(b2, 9);  cout << " | Hex: "; printHex(b2, 2);
        cout << "Test 3 (5 bit):  ";  printBits(b3, 5);  cout << " | Hex: "; printHex(b3, 1);
        cout << "Test 4 (12 bit): ";  printBits(b4, 12); cout << " | Hex: "; printHex(b4, 2);
        cout << "Test 5 (13 bit): ";  printBits(b5, 13); cout << " | Hex: "; printHex(b5, 3);

    }
    catch (const exception& ex) {
        cerr << "Error: " << ex.what() << endl;
    }
    return 0;
}