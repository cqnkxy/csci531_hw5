#include <iostream>
#include <string>
#include <cassert>
#include "modprod.h"
#include "utility.h"

using namespace std;

string modprod(const string&poly1, const string &poly2)
{
	assert(poly1.size() == 8 && poly2.size() == 8);
	vector<unsigned char> p1, p2, res;
	for (int i = 0; i < 8; i += 2) {
		p1.push_back(hex_to_dec(poly1.substr(i, 2)));
		p2.push_back(hex_to_dec(poly2.substr(i, 2)));
	}
	modular_product(p1, p2, res);
	string ret = "";
	char buf[61];
	for (int i = 0; i < 4; ++i) {
		sprintf(buf, "%02x", res[i]);
		ret += buf;
	}
	sprintf(
		buf,
		"{%02x}{%02x}{%02x}{%02x} CIRCLEX {%02x}{%02x}{%02x}{%02x} = {%02x}{%02x}{%02x}{%02x}",
		p1[0], p1[1], p1[2], p1[3],
		p2[0], p2[1], p2[2], p2[3],
		res[0], res[1], res[2], res[3]
	);
	return buf;
}
