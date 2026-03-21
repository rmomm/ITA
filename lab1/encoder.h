#pragma once
#include <string>
#include <vector>
using namespace std;

class Base64Encoder {
public:
    Base64Encoder() = default;

    string encode(const vector<uint8_t>& data);

    bool encodeFile(const string& inputFile, const string& outputFile = "");
};