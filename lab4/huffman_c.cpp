#include "huffman.h"
#include <fstream>
#include <vector>

bool huffman_compress(const string& input, const string& output) {
    ifstream in(input, ios::binary);
    if (!in)
        return false;

    uint32_t freq[256] = { 0 };
    vector<uint8_t> data;

    uint8_t byte;
    while (in.read((char*)&byte, 1)) {
        freq[byte]++;
        data.push_back(byte);
    }
    in.close();

    ofstream out(output, ios::binary);
    if (!out) 
        return false;

    out.write((char*)freq, 
        256 * sizeof(uint32_t));

    Node* root = build_tree(freq);
    if (!root) {
        out.close();
        return true;
    }

    unordered_map<uint8_t, string> codes;
    build_codes(root, "", codes);

    uint8_t buffer = 0;
    int bit_count = 0;

    for (uint8_t b : data) {
        const string& code = codes[b];
        for (char bit : code) {
            buffer <<= 1;
            if (bit == '1') buffer |= 1;
            bit_count++;

            if (bit_count == 8) {
                out.put(buffer);
                buffer = 0;
                bit_count = 0;
            }
        }
    }

    if (bit_count > 0) {
        buffer <<= (8 - bit_count);
        out.put(buffer);
    }

    free_tree(root);
    out.close();
    return true;
}