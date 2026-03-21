#include "lzw.h"

void lzw_comp(const string& input_file, const string& output_file) {
    ifstream in(input_file, ios::binary);
    ofstream out(output_file, ios::binary);

    if (!in || !out) {
        cout << "File error\n";
        return;
    }

    uint16_t max_dict_size = 65535;
    uint8_t mode = 1; 

    out.write((char*)&max_dict_size, sizeof(max_dict_size));
    out.write((char*)&mode, sizeof(mode));

    unordered_map<string, uint16_t> dict;

    for (int i = 0; i < 256; i++) {
        dict[string(1, char(i))] = i;
    }

    uint16_t dict_size = 256;
    string S = "";
    char C;

    while (in.get(C)) {
        string SC = S + C;

        if (dict.count(SC)) {
            S = SC;
        }
        else {
            if (!S.empty()) {
                uint16_t I = dict[S];
                out.write((char*)&I, sizeof(I));
            }

            if (dict_size < max_dict_size) {
                dict[SC] = dict_size++;
            }
            else {
                if (mode == 1) {
                    dict.clear();
                    for (int i = 0; i < 256; i++) {
                        dict[string(1, char(i))] = i;
                    }
                    dict_size = 256;
                }
            }

            S = string(1, C);
        }
    }

    if (!S.empty()) {
        uint16_t I = dict[S];
        out.write((char*)&I, sizeof(I));
    }

    cout << "Compressed successfully!\n";
}