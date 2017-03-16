#include <iostream>
#include <string>
#include <cassert>
#include <vector>

using namespace std;

const bool DEBUG = true;

void fatal(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);
	exit(1);
}

void dprint(const char *fmt, ...)
{
	if (DEBUG) {
		va_list args;
		va_start(args, fmt);
		vprintf(fmt, args);
		va_end(args);
	}
}

// the hex is supposed to be in range 00~ff
unsigned hex_to_dec(const string &hex)
{
	unsigned res = 0;
	for (size_t i = 0; i < hex.size(); i++) {
		char ch = tolower(hex[i]);
		assert('0' <= ch && ch <= 'f');
		res = (res << 4) + ('0' <= ch && ch <= '9' ? ch - '0' : ch - 'a' + 10);
	}
	return res;
}

inline unsigned char xtimes(unsigned char b)
{
	if (b >> 7) {
		return (b << 1) ^ 0x1B;
	}
	return b << 1;
}

// Output: p(x) = a(x) • b(x)
// 1) setp←{01},A←a,v[0]←A
// 2) if b0 = 1 then set p ← a
// 3) forifrom1to7do:
// 3.1) v[i] ← xtimes(A), A ← v[i]
// 3.2) if bi = 1 then set p ← A ⊕ p
// 4) return (p)
inline unsigned char big_dot(unsigned char a, unsigned char b)
{
 	unsigned char p = (b & 0x1) ? a : 0, v = a;
	for (int i = 1; i <= 7; ++i) {
		v = xtimes(v);
		if ((b >> i) & 0x1) {
			p ^= v;
		}
	}
	return p;
}

void modular_product(
	const vector<unsigned char> &a,
	const vector<unsigned char> &b,
	vector<unsigned char> &res
){
	assert(a.size() == 4 && b.size() == 4);
	res.resize(4);
	res[3] = big_dot(a[3], b[3]) ^ big_dot(a[0], b[2]) ^ big_dot(a[1], b[1]) ^ big_dot(a[2], b[0]);
	res[2] = big_dot(a[2], b[3]) ^ big_dot(a[3], b[2]) ^ big_dot(a[0], b[1]) ^ big_dot(a[1], b[0]);
	res[1] = big_dot(a[1], b[3]) ^ big_dot(a[2], b[2]) ^ big_dot(a[3], b[1]) ^ big_dot(a[0], b[0]);
	res[0] = big_dot(a[0], b[3]) ^ big_dot(a[1], b[2]) ^ big_dot(a[2], b[1]) ^ big_dot(a[3], b[0]);
}
