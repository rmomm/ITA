#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <list>
#include <map>

using namespace std;

string bwt_transform(const string& input, int& index);
string bwt_inverse(const string& code, int no);

vector<int> mtf_encode(const string& input);
string mtf_decode(const vector<int>& input);

string mtf_to_string(const vector<int>& mtf);
vector<int> string_to_mtf(const string& data);