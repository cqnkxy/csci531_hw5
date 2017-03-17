#include <string>
#include <vector>
#include <cassert>
#include "keyexpand.h"
#include "tablecheck.h"
#include "utility.h"
#include "config.h"

using namespace std;

static const int Nk = aes_bits_config::Nk;
static const int Nr = aes_bits_config::Nr;
static const int Nb = aes_bits_config::Nb;

vector<vector<unsigned char> >roundKeys::Keys;
void roundKeys::set_keys(const vector<vector<unsigned char> > &vecs) {
	Keys = vecs;
}
vector<vector<unsigned char> > &roundKeys::keys() { return Keys; }

inline void subWord(vector<unsigned char> &word)
{
	assert(word.size() == 4);
	for (int i = 0; i < 4; ++i) {
		word[i] = tables::s()[word[i]];
	}
}

inline void rotWord(vector<unsigned char> &word)
{
	unsigned char byte = word[0];
	word.erase(word.begin(), word.begin()+1);
	word.push_back(byte);
}

// big_dot power. Implemented as a lookup table.
inline void rcon(int i, vector<unsigned char> &res)
{
	res.assign(4, 0);
	switch (i) {
		case 0: break;
		case 1: res[0] = 0x01; break;
		case 2: res[0] = 0x02; break;
		case 3: res[0] = 0x04; break;
		case 4: res[0] = 0x08; break;
		case 5: res[0] = 0x10; break;
		case 6: res[0] = 0x20; break;
		case 7: res[0] = 0x40; break;
		case 8: res[0] = 0x80; break;
		case 9: res[0] = 0x1b; break;
		case 10: res[0] = 0x36; break;
		default: fatal("keyexpand.cpp rcon: i must be in range [0, 10] for AES-128\n");
	}
}

void keyexpand(
	const std::string &keystr,
	const std::string &tablefile,
	bool output
){
	check_tables(tablefile);
	if (keystr.size() != 32) {
		fatal("key should be of length 32!\n");
	}
	// transform key string to bytes
	vector<unsigned char> key;
	for (int i = 0; i < 32; i += 2) {
		key.push_back(hex_to_dec(keystr.substr(i, 2)));
	}
	vector<vector<unsigned char> > keys(Nb*(Nr+1), vector<unsigned char>());
	for (int i = 0; i < Nk; ++i) {
		keys[i].push_back(key[4*i]);
		keys[i].push_back(key[4*i+1]);
		keys[i].push_back(key[4*i+2]);
		keys[i].push_back(key[4*i+3]);
	}
	vector<unsigned char> rcon_i;
	for (int i = Nk; i < Nb*(Nr+1); ++i) {
		vector<unsigned char> tmp = keys[i-1];
		if (i % Nk == 0) {
			rotWord(tmp);
			subWord(tmp);
			rcon(i/Nk, rcon_i);
			word_xor(tmp, rcon_i, tmp);
		} else if (Nk > 6 && i % Nk == 4) {
			// AES-256
			subWord(tmp);
		}
		word_xor(keys[i-Nk], tmp, keys[i]);
	}
	roundKeys::set_keys(keys);
	if (output) {
		for (int i = 0; i < (int)keys.size(); ++i) {
			printf("w[%2d]: %s\n", i, vec_to_str(keys[i]).c_str());
		}
	}
}
