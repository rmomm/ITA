#include "lzw.h"

int main() {
    string command;

    while (true) {
       cout << "Enter command " << "(compress/decompress/stop): ";
       cin >> command;

       if (command == "stop") {
           break;
       }

        string input_file;
        string output_file;

        cout << "Input file: ";
        cin >> input_file;

        cout << "Output file: ";
        cin >> output_file;

        if (command == "compress") {
            lzw_comp( input_file, output_file);
        }
        else if (command == "decompress") {
            lzw_decomp( input_file, output_file);
        }
        else {
            cout << "Unknown command\n";
        }
    }

    return 0;
}