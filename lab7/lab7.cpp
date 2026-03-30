#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
#include <random>
#include <cmath>
#include <unordered_set>
#include <algorithm>

using namespace std;

const uint32_t N_SIZE = 1 << 16;


uint16_t hashPolynomial(const string& s, uint16_t k) {
    uint32_t h = 0; 

    for (size_t i = 0; i < s.size(); i += 2) {
        uint16_t a1 = (uint8_t)s[i];
        uint16_t a2 = (i + 1 < s.size()) ? (uint8_t)s[i + 1] : 0;

        uint16_t x_i = a1 + (a2 << 8);

        h = (h * k + x_i) % N_SIZE;
    }
    return (uint16_t)h;
}

class BloomFilter {
private:
    vector<uint64_t> T;
    vector<uint16_t> keys;

public:
    BloomFilter(int s) {
        T.assign(N_SIZE / 64, 0);

        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<uint16_t> dist(1, N_SIZE - 1);

        unordered_set<uint16_t> used;

        while ((int)keys.size() < s) {
            uint16_t key = dist(gen);

            if (used.insert(key).second) {
                keys.push_back(key);
            }
        }
    }

    void add(const string& x) {
        for (uint16_t k : keys) {
            uint16_t h = hashPolynomial(x, k);
            T[h / 64] |= (1ULL << (h % 64));
        }
    }

    bool contains(const string& x) const {
        for (uint16_t k : keys) {
            uint16_t h = hashPolynomial(x, k);
            if (!(T[h / 64] & (1ULL << (h % 64)))) {
                return false;
            }
        }
        return true;
    }
};

string randomString(mt19937& gen) {
    static const string chars = "abcdefghijklmnopqrstuvwxyz0123456789";
    uniform_int_distribution<> lenDist(1, 50);
    uniform_int_distribution<> charDist(0, chars.size() - 1);

    int len = lenDist(gen);
    string s;
    for (int i = 0; i < len; i++) { 
        s += chars[charDist(gen)]; 
    }
    return s;
}

double estimateError(int s, int n) {
    random_device rd;
    mt19937 gen(rd());
    double sumProb = 0;

    for (int rep = 0; rep < 100; rep++) {
        BloomFilter bf(s);
        unordered_set<string> inserted;

        while ((int)inserted.size() < n) {
            string x = randomString(gen);
            if (inserted.insert(x).second) {
                bf.add(x);
            }
        }

        int M = 0;
        while (true) {
            string test = randomString(gen);
            if (inserted.count(test)) {
                continue;
            }

            M++;
            if (bf.contains(test)) {
                sumProb += (1.0 / M);
                break;
            }
        }
    }
    return sumProb / 100.0;
}

int main() {
    cout << "alpha\ts\tn\tP_err" << endl;
    cout << "------------------------------------" << endl;

    for (double alpha = 0.05; alpha <= 0.51; alpha += 0.05) {
        int n = alpha * N_SIZE;
        int s = max(1, (int)(log(2.0) / alpha));

        double error = estimateError(s, n);
        cout << alpha << "\t" << s << "\t" << n << "\t" << error << endl;
    }

    return 0;
}