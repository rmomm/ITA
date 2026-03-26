#include "bwt_mtf.h"

string bwt_transform(const string& input, int& index) {
    int n = input.length();
    if (n == 0) return "";

    vector<int> p(n);
    for (int i = 0; i < n; i++) p[i] = i;

    sort(p.begin(), p.end(), [&](int a, int b) {
        for (int i = 0; i < n; i++) {
            char char_a = input[(a + i) % n];
            char char_b = input[(b + i) % n];
            if (char_a != char_b) return char_a < char_b;
        }
        return false;
        });

    string result = "";
    for (int i = 0; i < n; i++) {
        result += input[(p[i] + n - 1) % n];
        if (p[i] == 0) {
            index = i;
        }
    }
    return result;
}


string bwt_inverse(const string& code, int no) {
    int n = code.length();
    if (n == 0) return "";

    string sorted_code = code;
    sort(sorted_code.begin(), sorted_code.end());

    vector<int> T(n);
    vector<vector<int>> pos_map(256);
    for (int i = 0; i < n; i++) {
        pos_map[(unsigned char)code[i]].push_back(i);
    }

    vector<int> current_idx(256, 0);
    for (int i = 0; i < n; i++) {
        unsigned char c = (unsigned char)sorted_code[i];
        T[i] = pos_map[c][current_idx[c]++];
    }

    string res = "";
    int pos = no;
    for (int i = 0; i < n; i++) {
        pos = T[pos];
        res += code[pos];
    }
    return res;
}