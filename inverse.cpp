#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include "inverse.h"
#include "utility.h"

using namespace std;

static void output(
	int i,
	const vector<unsigned char> &rem,
	const vector<unsigned char> &quo,
	const vector<unsigned char> &aux
){
	assert(rem.size() == 4 && quo.size() == 4 && aux.size() == 4);
	printf("i=%d, rem[i]={%02x}{%02x}{%02x}{%02x}, quo[i]={%02x}{%02x}{%02x}{%02x}, aux[i]={%02x}{%02x}{%02x}{%02x}\n", 
		i,
		rem[0],rem[1],rem[2],rem[3],
		quo[0],quo[1],quo[2],quo[3],
		aux[0],aux[1],aux[2],aux[3]
	);
}

// check to see if a given array is all 0
static bool all_zero(const vector<unsigned char> &arr, int size=4)
{
	for (int i = 0; i < size && i < (int)arr.size(); ++i){
		if (arr[i] != 0) {
			return false;
		}
	}
	return true;
}

// check to see if the terminating condition has bee satisfied,
// i.e. rem = {0x0, 0x0, 0x0, 0x1}
static bool terminated(const vector<unsigned char> &arr)
{
	return all_zero(arr, 3) && arr[3] == 0x1;
}

// big_dot(num, res) = target
static unsigned char get_quo(unsigned char target, unsigned char num)
{
	return big_dot(multiplicative_inverse(num), target);
}

static void big_dot_seq(
	const vector<unsigned char> &vec,
	unsigned char num,
	vector<unsigned char> &res
){
	res.resize(vec.size());
	for (int i = 0; i < (int)vec.size(); ++i){
		res[i] = big_dot(vec[i], num);
	}
}

static void pack_zero_front(vector<unsigned char> &vec, size_t sz)
{
	if (vec.size() == sz) {
		return;
	}
	vector<unsigned char> tmp(sz, 0);
	int idx = sz;
	while (!vec.empty()) {
		tmp[--idx] = vec.back();
		vec.pop_back();
	}
	vec = tmp;
}

// filter out the leading zeros in `in` vector.
// `in` and `out` can't be the same vector.
static void filter_zero(
	const vector<unsigned char> &in,
	vector<unsigned char> &out
){
	out.clear();
	size_t i = 0;
	for (; i < in.size() && in[i] == 0; ++i){}
	for (; i < in.size(); ++i) {
		out.push_back(in[i]);
	}
}

// the vector of quotient/remainder can't be the same
// as the vector of divident/divisor
static void long_hand_division(
	const vector<unsigned char> &divident,
	const vector<unsigned char> &divisor,
	vector<unsigned char> &quotient,
	vector<unsigned char> &remainder
){
	vector<unsigned char> _divident, _divisor;
	filter_zero(divident, _divident);
	filter_zero(divisor, _divisor);
	assert(_divident.size() >= _divisor.size());
	quotient.clear();
	remainder.assign(_divident.begin(), _divident.begin()+_divisor.size()-1);
	vector<unsigned char> tmp(_divisor.size(), 0);
	bool last_step = _divisor.size() == 1;
	if (last_step) {
		// if last step, make sure we get a remainder of 1
		_divident.back() ^= 0x1;
	}
	for (size_t i = _divisor.size(); i <= _divident.size(); ++i) {
		remainder.push_back(_divident[i-1]);
		if (remainder[0] == 0) {
			remainder.erase(remainder.begin(), remainder.begin()+1);
			quotient.push_back(0);
		} else {
			unsigned char q = get_quo(remainder[0], _divisor[0]);
			quotient.push_back(q);
			big_dot_seq(_divisor, q, tmp);
			word_xor(remainder, tmp, remainder);
			assert(remainder[0] == 0x0);
			remainder.erase(remainder.begin(), remainder.begin()+1);
		}
	}
	pack_zero_front(quotient, 4);
	pack_zero_front(remainder, 4);
	if (last_step) {
		remainder.back() ^= 0x1;
	}
}

void inverse(const string &poly)
{
	if (poly.size() != 8) {
		fatal("Invalid poly length!\n");
	}
	if (!all_hex(poly)) {
		fatal("value of poly should be hex digits [0-9a-zA-Z]\n");
	}
	vector<unsigned char> p;
	for (size_t i = 0; i < poly.size(); i += 2){
		p.push_back(hex_to_dec(poly.substr(i, 2)));
	}
	int pre = 0, cur = 1;
	vector<vector<unsigned char> > rems;
	vector<vector<unsigned char> > quos(2, vector<unsigned char>(4, 0x0));
	vector<vector<unsigned char> > auxs(2, vector<unsigned char>(4, 0x0));
	rems.resize(2);
	rems[0].push_back(0x01);
	rems[0].push_back(0x00);
	rems[0].push_back(0x00);
	rems[0].push_back(0x00);
	rems[0].push_back(0x01);
	rems[1] = p;
	auxs[1][3] = 0x1;
	output(
		1,
		vector<unsigned char>(rems[0].begin()+1, rems[0].end()),
		quos[0],
		auxs[0]
	);
	output(2, rems[1], quos[1], auxs[1]);
	vector<unsigned char> tmp(4, 0), mprod(4, 0);
	for (int i = 3; i <= 6; ++i){
		long_hand_division(rems[pre], rems[cur], quos[cur], tmp);
		modular_product(quos[cur], auxs[cur], mprod);
		word_xor(auxs[pre], mprod, auxs[pre]);
		output(i, tmp, quos[cur], auxs[pre]);
		rems[pre] = tmp;
		if (all_zero(tmp)) {
			printf("{%02x}{%02x}{%02x}{%02x} does not have a multiplicative inverse.\n",
				p[0], p[1], p[2], p[3]
			);
			return;
		}
		if (terminated(tmp)){
			printf("Multiplicative inverse of {%02x}{%02x}{%02x}{%02x} is {%02x}{%02x}{%02x}{%02x}\n",
				p[0], p[1], p[2], p[3],
				auxs[pre][0], auxs[pre][1], auxs[pre][2], auxs[pre][3]
			);
			return;
		}
		swap(pre, cur);
	}
}
