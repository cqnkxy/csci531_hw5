#include <iostream>
#include <vector>
#include <fstream>
#include "tablecheck.h"
#include "utility.h"

using namespace std;

vector<unsigned char> tables::S;
vector<unsigned char> tables::P;
vector<unsigned char> tables::INVS;
vector<unsigned char> tables::INVP;
void tables::set_s(const vector<unsigned char> &vec) { S = vec; }
void tables::set_invs(const vector<unsigned char> &vec) { INVS = vec; }
void tables::set_p(const vector<unsigned char> &vec) { P = vec; }
void tables::set_invp(const vector<unsigned char> &vec) { INVP = vec; }
const vector<unsigned char> &tables::s() { return S; }
const vector<unsigned char> &tables::invs() {return INVS; }
const vector<unsigned char> &tables::p() { return P; }
const vector<unsigned char> &tables::invp() { return INVP; }

void get_invs(const vector<unsigned char> &vec)
{
	vector<unsigned char> inv(256, 0);
	for (size_t i = 0; i < vec.size(); ++i) {
		inv[vec[i]] = (unsigned char)i;
	}
	tables::set_invs(inv);
}

void check_s(const string &value)
{
	vector<unsigned char> vec, exits(256, 0);
	if (value.size() != 512) {
		fatal("value of S should be of size 512");
	}
	for (size_t i = 0; i < value.size(); i += 2){
		unsigned char val = hex_to_dec(value.substr(i, 2));
		if (exits[val]) {
			fatal("%d occur too many times", val);
		}
		vec.push_back(val);
		exits[val] = 1;
	}
	tables::set_s(vec);
	get_invs(vec);
}

void check_p(const string &value)
{
	vector<unsigned char> vec;
	for (size_t i = 0; i < value.size(); i += 2) {
		unsigned char val = hex_to_dec(value.substr(i, 2));
		vec.push_back(val);
	}
	tables::set_p(vec);
}

void check_invp(const string &value)
{
	vector<unsigned char> vec;
	for (size_t i = 0; i < value.size(); i += 2) {
		unsigned char val = hex_to_dec(value.substr(i, 2));
		vec.push_back(val);
	}
	tables::set_invp(vec);	
}

void check_tables(const string &file)
{
	ifstream in(file.c_str());
	if (!in.is_open()) {
		fatal("%s can't be open!", file.c_str());
	}
	string line;
	int col = 0;
	while (in >> line) {
		size_t equl_pos = line.find('=');
		if (equl_pos == string::npos) {
			fatal("line #%d: should be of format key=value", col);
		}
		string opt = line.substr(0, equl_pos);
		if (opt == "S") {
			check_s(line.substr(equl_pos+1, -1));
		} else if (opt == "P") {
			check_p(line.substr(equl_pos+1, -1));
		} else if (opt == "INVP") {
			check_invp(line.substr(equl_pos+1, -1));
		} else {
			fatal("line #%d: the key should be one of (S, P, INVP)", col);
		}
		col += 1;
	}
	vector<unsigned char> res;
	if (tables::p().size() != 4 || tables::invp().size() != 4) {
		fatal("P and INVP should be of length 8");
	}
	modular_product(tables::p(), tables::invp(), res);
	unsigned char expected[] = {0, 0, 0, 1};
	if (res != vector<unsigned char>(expected, expected+4)) {
		fatal("The product of the polynomials represented by P and INVP should be {00}{00}{00}{01}.\n");
	}
}
