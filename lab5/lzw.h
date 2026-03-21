#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <cstdint>
using namespace std;

void lzw_comp(const string& input_file, const string& output_file);
void lzw_decomp(const string& input_file, const string& output_file);