#include "huffman.h"
#include "BitStream.h"
using namespace std;

Node::Node(uint8_t s, uint32_t f) : symbol(s), freq(f), left(nullptr), right(nullptr) {}
Node::Node(Node* l, Node* r) : symbol(0), freq(l->freq + r->freq), left(l), right(r) {}

struct Compare {
    bool operator()(Node* a, Node* b) {
        return a->freq > b->freq;
    }
};


Node* build_tree(uint32_t freq[256]) {
    priority_queue<Node*, vector<Node*>, Compare> pq;
    for (int i = 0; i < 256; ++i)
        if (freq[i] > 0) {
            pq.push(new Node((uint8_t)i, freq[i]));
        }

    if (pq.empty()) { 
        return nullptr; 
    }

    while (pq.size() > 1) {
        Node* left = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();
        pq.push(new Node(left, right));
    }
    return pq.top();
}

void build_codes(Node* root, const string& code, unordered_map<uint8_t, string>& table) {
    if (!root) {
        return;
    }
    if (!root->left && !root->right) {
        table[root->symbol] = code.empty() ? "0" : code;
        return;
    }
    build_codes(root->left, code + "0", table);
    build_codes(root->right, code + "1", table);
}


void free_tree(Node* root) {
    if (!root) {
        return;
    }
    free_tree(root->left);
    free_tree(root->right);
    delete root;
}


bool huffman_compress(const string& input, const string& output) {
    ifstream in(input, ios::binary);
    if (!in) {
        return false;
    }

    uint32_t freq[256] = { 0 };
    vector<uint8_t> data;
    uint8_t byte;
    while (in.read((char*)&byte, 1)) {
        freq[byte]++;
        data.push_back(byte);
    }
    in.close();

    ofstream meta(output, ios::binary);
    if (!meta) {
        return false;
    }
    meta.write((char*)freq, sizeof(freq));
    meta.close();

    Node* root = build_tree(freq);
    if (!root) {
        return true;
    }

    unordered_map<uint8_t, string> codes;
    build_codes(root, "", codes);

    BitStream bs(output, BitStream::Write);
    bs.SeekWrite(sizeof(freq));

    for (uint8_t b : data) {
        const string& code = codes[b];
        vector<uint8_t> bits(code.size());
        for (size_t i = 0; i < code.size(); ++i) {
            bits[i] = (code[i] == '1') ? 1 : 0;
        }
        bs.WriteBitSequence(bits.data(), bits.size());
    }

    bs.Close();
    free_tree(root);
    return true;
}


bool huffman_decompress(const string& input, const string& output) {
    ifstream in(input, ios::binary);
    if (!in) {
        return false;
    }

    uint32_t freq[256];
    in.read((char*)freq, sizeof(freq));
    in.close();

    Node* root = build_tree(freq);
    if (!root) {
        return false;
    }

    uint64_t total = 0;
    for (int i = 0; i < 256; ++i) {
        total += freq[i];
    }

    ofstream out(output, ios::binary);
    if (!out) {
        return false;
    }

    BitStream bs(input, BitStream::Read);
    bs.SeekRead(sizeof(freq));

    Node* current = root;
    uint64_t written = 0;
    while (written < total) {
        uint8_t bit;
        bs.ReadBitSequence(&bit, 1);
        current = bit ? current->right : current->left;

        if (!current->left && !current->right) {
            out.put(current->symbol);
            written++;
            current = root;
        }
    }

    bs.Close();
    out.close();
    free_tree(root);
    return true;
}