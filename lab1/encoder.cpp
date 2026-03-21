#include "encoder.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
using namespace std;

const string Alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

string Base64Encoder::encode(const vector<uint8_t>& data) {
    string result;

    for (size_t i = 0; i < data.size(); i += 3) {
        uint32_t block = 0;
        int r = data.size() - i;

        block |= data[i] << 16;
        if (r > 1) { 
            block |= data[i + 1] << 8; 
        }

        if (r > 2) { 
            block |= data[i + 2]; 
        }

        int c1 = (block >> 18) & 0x3F;
        int c2 = (block >> 12) & 0x3F;
        int c3 = (block >> 6) & 0x3F;
        int c4 = block & 0x3F;

        result += Alphabet[c1];
        result += Alphabet[c2];
        result += (r > 1) ? Alphabet[c3] : '=';
        result += (r > 2) ? Alphabet[c4] : '=';
    }

    return result;
}

bool Base64Encoder::encodeFile(const string& inputFile, const string& outputFile) {
    cout << "Input file: " << inputFile << endl;
    cout << "Output file param: " << outputFile << endl;

    ifstream in(inputFile, ios::binary);
    if (!in.is_open()) {
        cerr << "Cannot open input file: " << inputFile << endl;
        return false;
    }

    vector<uint8_t> data((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
    in.close();
    cout << "Bytes read: " << data.size() << endl;

    string encoded = encode(data);
    cout << "Encoded size: " << encoded.size() << endl;

    string outFileName = outputFile;
    if (outFileName.empty()) { 
        outFileName = inputFile + ".base64"; 
    }
    cout << "Final output file: " << outFileName << endl;

    ofstream out(outFileName);
    if (!out.is_open()) {
        cerr << "Cannot create/write to file: " << outFileName << endl;
        return false;
    }

    size_t pos = 0;
    while (pos < encoded.size()) {
        size_t len = min<size_t>(76, encoded.size() - pos);
        out << encoded.substr(pos, len) << "\n";
        pos += len;
    }

    out.close();

    return true;
}
