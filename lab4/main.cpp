#include <iostream>
#include "huffman.h"

int main() {
    int choice;
    string in, out;

    cout << "1 - Compress\n";
    cout << "2 - Decompress\n";
    cout << "Enter choice: ";
    cin >> choice;

    if (choice == 1) {
        cout << "Input file: ";
        cin >> in;
        cout << "Output file: ";
        cin >> out;

        if (huffman_compress(in, out))
            cout << "File compressed successfully.\n";
        else
            cout << "Compression failed.\n";
    }
    else if (choice == 2) {
        cout << "Input file: ";
        cin >> in;
        cout << "Output file: ";
        cin >> out;

        if (huffman_decompress(in, out))
            cout << "File decompressed successfully.\n";
        else
           cout << "Decompression failed.\n";
    }
    else {
        cout << "Invalid choice.\n";
    }

    return 0;
}