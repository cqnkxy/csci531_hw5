#ifndef __TABLECHECK_H_
#define __TABLECHECK_H_

#include <iostream>
#include <vector>
#include <string>

class tables {
public:
	tables() {}
	static const std::vector<unsigned char> &s();
	static const std::vector<unsigned char> &invs();
	static const std::vector<unsigned char> &p();
	static const std::vector<unsigned char> &invp();
	static void set_s(const std::vector<unsigned char>&);
	static void set_invs(const std::vector<unsigned char>&);
	static void set_p(const std::vector<unsigned char>&);
	static void set_invp(const std::vector<unsigned char>&);
private:
	static std::vector<unsigned char> S;
	static std::vector<unsigned char> INVS;
	static std::vector<unsigned char> P;
	static std::vector<unsigned char> INVP;
};

void check_tables(const std::string &);

#endif
