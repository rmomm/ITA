#include "huffman.h"
#include <queue>

Node::Node(uint8_t s, uint32_t f) : symbol(s), freq(f), left(nullptr), right(nullptr) {}

Node::Node(Node* l, Node* r) : symbol(0), freq(l->freq + r->freq), left(l), right(r) {}

struct Compare {
    bool operator()(Node* a, Node* b) {
        return a->freq > b->freq;
    }
};

Node* build_tree(uint32_t freq[256]) {
    priority_queue<Node*, vector<Node*>, Compare> pq;

    for (int i = 0; i < 256; ++i) {
        if (freq[i] > 0) {
            pq.push(new Node((uint8_t)i, freq[i]));
        }
    }

    if (pq.empty()) return nullptr;

    while (pq.size() > 1) {
        Node* left = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();
        pq.push(new Node(left, right));
    }

    return pq.top();
}

void build_codes(Node* root,
    const string& code,
    unordered_map<uint8_t, string>& table) {
    if (!root) 
        return;

    if (!root->left && !root->right) {
        table[root->symbol] = code.empty() ? "0" : code;
        return;
    }

    build_codes(root->left, code + "0", table);
    build_codes(root->right, code + "1", table);
}

void free_tree(Node* root) {
    if (!root) 
        return;
    free_tree(root->left);
    free_tree(root->right);
    delete root;
}