#include "lzw.h"

void lzw_decomp(const string& input_file, const string& output_file) {
    ifstream in(input_file, ios::binary);
    ofstream out(output_file, ios::binary);

    if (!in || !out) {
        cout << "File error\n";
        return;
    }

    uint16_t max_dict_size;
    uint8_t mode;

    if (!in.read((char*)&max_dict_size, sizeof(max_dict_size)) || !in.read((char*)&mode, sizeof(mode))) {
        cout << "Invalid file format\n";
        return;
    }

    vector<string> dict;

    for (int i = 0; i < 256; i++){
        dict.push_back(string(1, char(i)));
    }

    uint16_t prev_code;

    if (!in.read((char*)&prev_code, sizeof(prev_code))) {
        cout << "Empty file\n";
        return;
    }

    if (prev_code >= dict.size()) {
        cout << "Corrupted file\n";
        return;
    }

    string S = dict[prev_code];
    out << S;

    uint16_t code;

    while (in.read((char*)&code, sizeof(code))) {
        string entry;

        if (code < dict.size()) {
            entry = dict[code];
        }
        else if (code == dict.size()) {
            entry = S + S[0];
        }
        else{
            cout << "Decoding error\n";
            return;
        }

        out << entry;

        if (dict.size() < max_dict_size) {
            dict.push_back(S + entry[0]);
        }
        else {
            if (mode == 1) {
                dict.clear();
                for (int i = 0; i < 256; i++) {
                    dict.push_back(string(1, char(i)));
                }
            }
        }

        S = entry;
    }

    cout << "Decompressed successfully!\n";
}