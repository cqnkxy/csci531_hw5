#ifndef __UTILITY_H_
#define __UTILITY_H_

#include <iostream>
#include <string>
#include <vector>

void fatal(const char *fmt, ...);
void dprint(const char *fmt, ...);
unsigned hex_to_dec(const std::string &hex);
unsigned char big_dot(unsigned char, unsigned char);
void modular_product(
	const std::vector<unsigned char> &a,
	const std::vector<unsigned char> &b,
	std::vector<unsigned char> &res
);
// self and other are of size 4, i.e. a word.
// the result of the function will be saved in res
// res can be one of self and other or a new vector.
void word_xor(
	const std::vector<unsigned char> &self,
	const std::vector<unsigned char> &other,
	std::vector<unsigned char> &res
);
std::string vec_to_str(const std::vector<unsigned char> &vec);
// column-wise
std::string vecs_to_str(const std::vector<std::vector<unsigned char> >&);
std::string vecs_to_str(
	const std::vector<std::vector<unsigned char> >::iterator &start,
	const std::vector<std::vector<unsigned char> >::iterator &end
);
void left_circular_shift(std::vector<unsigned char> &, int);
#endif
