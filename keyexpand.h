#ifndef __KEYEXPAND_H_
#define __KEYEXPAND_H_

#include <string>
#include <vector>

class roundKeys
{
public:
	static std::vector<std::vector<unsigned char> > &keys();
	static void set_keys(const std::vector<std::vector<unsigned char> > &);
private:
	static std::vector<std::vector<unsigned char> > Keys;
};
// `keyexpand` will read in tablefile as well as expand the key
// to get roundKeys
void keyexpand(
	const std::string &key,
	const std::string &tablefile,
	bool output=true
);

#endif
