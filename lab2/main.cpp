#include "Header.h"
#include <iostream>

int main() {
    int choice;
    string inputfile, outputfile;

    cout << "RLE Compression/Decompression\n";
    cout << "1 - Encode\n";
    cout << "2 - Decode\n";
    cout << "Enter choice: ";
    cin >> choice;

    cout << "Enter input file name: ";
    cin >> inputfile;
    cout << "Enter output file name: ";
    cin >> outputfile;

    bool result = false;

    if (choice == 1) {
        result = encode(inputfile, outputfile);
        if (result)
            cout << "File successfully compressed!\n";
        else
            cout << "Compression failed.\n";
    }
    else if (choice == 2) {
        result = decode(inputfile, outputfile);
        if (result)
            cout << "File successfully decompressed!\n";
        else
            cout << "Decompression failed.\n";
    }
    else {
        cout << "Invalid choice.\n";
    }

    return 0;
}