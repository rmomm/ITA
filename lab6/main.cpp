#include "bwt_mtf.h"
#include "lzw.h"
#include "huffman.h"
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

string mtf_to_string(const vector<int>& mtf) {
    string result;

    for (int x : mtf) {
        result.push_back(static_cast<unsigned char>(x));
    }

    return result;
}

vector<int> string_to_mtf(const string& data){
    vector<int> mtf;

    for (unsigned char c : data) {
        mtf.push_back(static_cast<int>(c));
    }

    return mtf;
}

string readFile(const string& filename) {
    ifstream in(filename, ios::binary);
    return string((istreambuf_iterator<char>(in)),
        istreambuf_iterator<char>());
}

void writeFile(const string& filename, const string& data) {
    ofstream out(filename, ios::binary);
    out.write(data.data(), data.size());
}

void compress_pipeline(const string& input, const string& output, int method, bool useBWT, bool useMTF){
    string data = readFile(input);

    int bwtIndex = 0;

    if (useBWT) {
        data = bwt_transform(data, bwtIndex);
    }


    if (useMTF) {
            auto mtf = mtf_encode(data);
            data = mtf_to_string(mtf);
    }
    

    writeFile("temp.bin", data);

    if (method == 1) {
        huffman_compress("temp.bin", output);
    }
    else {
        lzw_comp("temp.bin", output);
    }

    ofstream meta("meta.txt");
    meta << useBWT << " " << useMTF << " " << bwtIndex << " " << method;
}

void decompress_pipeline(const string& input, const string& output) {
    bool useBWT, useMTF;
    int bwtIndex, method;

    ifstream meta("meta.txt");
    meta >> useBWT >> useMTF >> bwtIndex >> method;

    if (method == 1) {
        huffman_decompress(input, "temp.bin");
    }
    else {
        lzw_decomp(input, "temp.bin");
    }

    string data = readFile("temp.bin");

    if (useMTF) {
        auto mtf = string_to_mtf(data);
        data = mtf_decode(mtf);
    }

    if (useBWT) {
        data = bwt_inverse(data, bwtIndex);
    }

    writeFile(output, data);
}

int main() {

    while (true) {

        int choice;
        cout << "\n1 - Compress\n2 - Decompress\n0 - Exit\n";
        cin >> choice;

        if (choice == 0) break;

        string input, output;
        cout << "Input file: ";
        cin >> input;

        cout << "Output file: ";
        cin >> output;

        if (choice == 1) {

            int method;
            cout << "1 - Huffman, 2 - LZW: ";
            cin >> method;

            bool useBWT, useMTF;
            cout << "Use BWT (1/0): ";
            cin >> useBWT;

            cout << "Use MTF (1/0): ";
            cin >> useMTF;

            compress_pipeline(input, output, method, useBWT, useMTF);

        }
        else if (choice == 2) {

            decompress_pipeline(input, output);
        }
    }

    return 0;
}