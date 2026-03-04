#include "Header.h"
#include <fstream>
#include <vector>
#include <iostream>

bool decode(const string& inputfile, const string& outputfile) {
    ifstream fin(inputfile, ios::binary);
    if (!fin.is_open()) 
        return false;

    ofstream fout(outputfile, ios::binary);
    if (!fout.is_open()) {
        cout << "Cannot create output file: " << outputfile << "\n";
        return false;
    }

    char sig[4];
    if (!fin.read(sig, 4)) {
        cout << "File too short or empty\n";
        return false;
    }
    if (string(sig, 4) != "RLE1") {
        cout << "Not a valid RLE file\n";
        return false;
    }

    while (!fin.eof()) {
        unsigned char L;
        if (!fin.read(reinterpret_cast<char*>(&L), 1)) 
            break;

        if (L & 0x80) {
            unsigned char value;
            if (!fin.read(reinterpret_cast<char*>(&value), 1)) {
                cout << "Unexpected end of file during repeat block\n";
                return false;
            }
            size_t run_length = (L & 0x7F) + 1;
            for (size_t i = 0; i < run_length; i++)
                fout.put(value);
        }
        else {
            size_t run_length = (L & 0x7F) + 1;
            for (size_t i = 0; i < run_length; i++) {
                unsigned char value;
                if (!fin.read(reinterpret_cast<char*>(&value), 1)) {
                    cout << "Unexpected end of file during literal block\n";
                    return false;
                }

                fout.put(value);
            }
        }
    }

    fin.close();
    fout.close();
    return true;
}