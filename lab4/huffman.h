#pragma once
#include <cstdint>
#include <string>
#include <unordered_map>
using namespace std;

struct Node {
    uint8_t symbol;
    uint32_t freq;
    Node* left;
    Node* right;

    Node(uint8_t s, uint32_t f);
    Node(Node* l, Node* r);
};

Node* build_tree(uint32_t freq[256]);

void build_codes(Node* root, const string& code,unordered_map<uint8_t,string>& table);

bool huffman_compress(const string& input, const string& output);

bool huffman_decompress(const string& input, const string& output);

void free_tree(Node* root);