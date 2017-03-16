#ifndef __UTILITY_H_
#define __UTILITY_H_

#include <iostream>
#include <string>
#include <vector>

void fatal(const char *fmt, ...);
void dprint(const char *fmt, ...);
unsigned hex_to_dec(const std::string &hex);
void modular_product(
	const std::vector<unsigned char> &a,
	const std::vector<unsigned char> &b,
	std::vector<unsigned char> &res
);

#endif
