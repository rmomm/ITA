#include "Header.h"
#include <fstream>
#include <vector>
#include <iostream>

bool encode(const string& inputfile, const string& outputfile) {
    ifstream fin(inputfile, ios::binary);
    if (!fin.is_open()) {
        cout << "Cannot open input file: " << inputfile << "\n";
        return false;
    }

    ofstream fout(outputfile, ios::binary);
    if (!fout.is_open()) {
        cout << "Cannot create output file: " << outputfile << "\n";
        return false;
    }

    fout.write("RLE1", 4);

    vector<unsigned char> buffer((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>());

    size_t i = 0;
    while (i < buffer.size()) {
        size_t run_length = 1;

        while (i + run_length < buffer.size() && buffer[i] == buffer[i + run_length] && run_length < 128)
            run_length++;

        if (run_length > 1) {
            unsigned char L = 128 + (run_length - 1);
            fout.put(L);
            fout.put(buffer[i]);
            i += run_length;
        }
        else {
            size_t start = i;
            while (i < buffer.size() &&
                (i + 1 >= buffer.size() || buffer[i] != buffer[i + 1]) &&
                (i - start) < 128)
                i++;

            unsigned char L = (i - start) - 1; 
            fout.put(L);
            for (size_t j = start; j < i; j++)
                fout.put(buffer[j]);
        }
    }

    fin.close();
    fout.close();
    return true;
}