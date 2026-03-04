#include "huffman.h"
#include <fstream>

bool huffman_decompress(const string& input, const string& output) {
    ifstream in(input, ios::binary);
    if (!in) 
        return false;

    uint32_t freq[256];
    in.read((char*)freq, 
        256 * sizeof(uint32_t));

    Node* root = build_tree(freq);
    if (!root) return false;

    uint64_t total = 0;
    for (int i = 0; i < 256; ++i)
        total += freq[i];

    ofstream out(output,ios::binary);
    if (!out) 
        return false;

    Node* current = root;
    uint8_t byte;
    uint64_t written = 0;

    while (in.read((char*)&byte, 1) && written < total) {
        for (int i = 7; i >= 0 && written < total; --i) {
            bool bit = (byte >> i) & 1;
            current = bit ? current->right : current->left;

            if (!current->left && !current->right) {
                out.put(current->symbol);
                written++;
                current = root;
            }
        }
    }

    free_tree(root);
    in.close();
    out.close();
    return true;
}