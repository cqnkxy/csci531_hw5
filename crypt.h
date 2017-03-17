#ifndef __CRYPT_H_
#define __CRYPT_H_

#include <iostream>
#include <string>

void encrypt(
	const std::string &key,
	const std::string &keyfile,
	std::istream &in
);

void decrypt(
	const std::string &key,
	const std::string &keyfile,
	std::istream &in
);

#endif
