#include "decoder.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
using namespace std;

const string Alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

vector<uint8_t> Base64Decoder::decode(const string& text, bool& s, string& er) {
    vector<uint8_t> result;
    s = true;
    er = "";

    int table[256];
    fill(begin(table), end(table), -1);
    for (int i = 0; i < 64; i++)
        table[(unsigned char)Alphabet[i]] = i;

    vector<int> buffer;
    int paddingCount = 0;
    bool finished = false;

    for (size_t i = 0; i < text.size(); i++) {
        char c = text[i];

        if (c == '=') {
            if (!finished) {
                buffer.push_back(0);
                paddingCount++;
            }
            else {
                s = false;
                er = "Invalid '=' after end of message at position " + to_string(i);
                return {};
            }
        }
        else {
            int val = table[(unsigned char)c];
            if (val == -1) {
                s = false;
                er = "Invalid Base64 character at position " + to_string(i);
                return {};
            }
            if (paddingCount > 0) {
                s = false;
                er = "Invalid Base64: non-padding character after '=' at position " + to_string(i);
                return {};
            }
            buffer.push_back(val);
        }

        if (buffer.size() == 4) {
            uint32_t block = (buffer[0] << 18) | (buffer[1] << 12) | (buffer[2] << 6) | buffer[3];
            result.push_back((block >> 16) & 0xFF);
            if (paddingCount < 2) {
                result.push_back((block >> 8) & 0xFF); 
            }
               
            if (paddingCount < 1) {
                result.push_back(block & 0xFF);
            }

            buffer.clear();
            if (paddingCount > 0) {
                finished = true;
            }
            paddingCount = 0;
        }
    }

    if (!buffer.empty()) {
        s = false;
        er = "Invalid Base64 length at end of input";
        return {};
    }

    return result;
}


bool Base64Decoder::decodeFile(const string& inputFile, const string& outputFile) {
    ifstream in(inputFile);
    if (!in.is_open()) {
        cerr << "Cannot open input file: " << inputFile << endl;
        return false;
    }

    vector<string> lines;
    string line;
    while (getline(in, line)) {
        lines.push_back(line);
    }
    in.close();

    string text;
    int row = 0;
    bool finished = false;
    for (auto& l : lines) {
        row++;
        if (l.empty()) continue;

        if (l[0] == '-') continue;

        if (!finished && l.size() != 76 && row != lines.size()) {
            cerr << "Line " << row << ": Incorrect line length (" << l.size() << ")" << endl;
            return false;
        }

        if (finished && !l.empty() && l[0] != '-') {
            cerr << "Warning: Data after end of message (line " << row << ")" << endl;
            break;
        }

        for (size_t i = 0; i < l.size(); i++) {
            char c = l[i];
            if (c != '=' && Alphabet.find(c) == string::npos) {
                cerr << "Line " << row << ", char " << i + 1 << ": Invalid input character '" << c << "'" << endl;
                return false;
            }
            if (c == '=') finished = true;
        }

        text += l;
    }

    bool s;
    string er;
    vector<uint8_t> data = decode(text, s, er);

    if (!s) {
        cerr << er << endl;
        return false;
    }

    ofstream out(outputFile, ios::binary);
    if (!out.is_open()) {
        cerr << "Cannot open output file: " << outputFile << endl;
        return false;
    }

    out.write(reinterpret_cast<const char*>(data.data()), data.size());
    out.close();

    return true;
}

