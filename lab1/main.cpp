#include "encoder.h"
#include "decoder.h"
#include <iostream>
#include <string>
using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cout << "  Encode: " << argv[0] << " -e \n";
        cout << "  Decode: " << argv[0] << " -d \n";
        return 1;
    }

    string mode = argv[1];
    string infile = argv[2];
    string outfile = "";

    if (mode == "-e") {
        if (argc >= 4) 
            outfile = argv[3];
    }
    else if (mode == "-d") {
        if (argc < 4) {
            cerr << "Please specify output file for decoding.\n";
            return 1;
        }
        outfile = argv[3];
    }
    else {
        cerr << "Unknown mode. Use -e or -d.\n";
        return 1;
    }

    if (mode == "-e") {
        Base64Encoder encoder;
        if (encoder.encodeFile(infile, outfile)) {
            cout << "Encoding complete.\n";
        }
        else {
            cerr << "File encoding error.\n";
            return 1;
        }
    }
    else { 
        Base64Decoder decoder;
        if (decoder.decodeFile(infile, outfile)) {
            cout << "Decoding complete.\n";
        }
        else {
            cerr << "File decoding error.\n";
            return 1;
        }
    }

    return 0;
}
