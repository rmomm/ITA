#pragma once
#include <string>
#include <vector>
using namespace std;

class Base64Decoder {
    public:
        Base64Decoder() = default;

        vector<uint8_t> decode(const string& text, bool& s, string& er);

        bool decodeFile(const string& inputFile, const string& outputFile);
};