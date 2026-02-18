#include "Header.h"
#include <fstream>
#include <vector>

bool decode(const std::string& inputfile, const std::string& outputfile) {
    std::ifstream fin(inputfile, std::ios::binary);
    if (!fin.is_open()) 
        return false;

    std::ofstream fout(outputfile, std::ios::binary);
    if (!fout.is_open()) 
        return false;

    while (!fin.eof()) {
        unsigned char L;
        if (!fin.read(reinterpret_cast<char*>(&L), 1)) break;

        if (L & 0x80) {
            unsigned char value;
            if (!fin.read(reinterpret_cast<char*>(&value), 1)) return false;
            size_t run_length = (L & 0x7F) + 1;
            for (size_t i = 0; i < run_length; i++)
                fout.put(value);
        }
        else {
            size_t run_length = (L & 0x7F) + 1;
            for (size_t i = 0; i < run_length; i++) {
                unsigned char value;
                if (!fin.read(reinterpret_cast<char*>(&value), 1)) return false;
                fout.put(value);
            }
        }
    }

    fin.close();
    fout.close();
    return true;
}