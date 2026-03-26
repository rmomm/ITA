#include "bwt_mtf.h"

vector<int> mtf_encode(const string& input) {
    list<unsigned char> alphabet;

    for (int i = 0; i < 256; i++) {
        alphabet.push_back((unsigned char)i);
    }

    vector<int> result;

    for (unsigned char c : input) {

        int index = 0;

        for (auto it = alphabet.begin(); it != alphabet.end(); ++it, ++index) {

            if (*it == c) {

                result.push_back(index);
                alphabet.erase(it);
                alphabet.push_front(c);

                break;
            }
        }
    }

    return result;
}

string mtf_decode(const vector<int>& input) {
    list<unsigned char> alphabet;

    for (int i = 0; i < 256; i++) {
        alphabet.push_back((unsigned char)i);
    }

    string result;

    for (int index : input) {
        auto it = alphabet.begin();
        advance(it, index);

        unsigned char c = *it;
        result += c;
        alphabet.erase(it);
        alphabet.push_front(c);
    }

    return result;
}