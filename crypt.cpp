#include <vector>
#include <cassert>
#include "crypt.h"
#include "keyexpand.h"
#include "config.h"
#include "tablecheck.h"
#include "utility.h"

using namespace std;

static const int Nk = aes_bits_config::Nk;
static const int Nr = aes_bits_config::Nr;
static const int Nb = aes_bits_config::Nb;

// Add roundKey to state. Modify state in-place.
void addRoundKey(vector<vector<unsigned char> > &state, int roundKeyIdx)
{
	for (int c = 0; c < Nb; ++c) {
		const vector<unsigned char> &key = roundKeys::keys()[roundKeyIdx+c];
		for (int r = 0; r < 4; ++r) {
			state[r][c] ^= key[r];
		}
	}
}

void subBytes(vector<vector<unsigned char> > &state)
{
	const vector<unsigned char> &sbox = tables::s();
	for (int r = 0; r < 4; ++r) {
		for (int c = 0; c < Nb; ++c) {
			int row = state[r][c] >> 4, col = state[r][c] & 0x0f;
			assert((unsigned char)((row << 4) | col) == state[r][c]);
			state[r][c] = sbox[row*16+col];
		}
	}
}

void invSubBytes(vector<vector<unsigned char> > &state)
{
	const vector<unsigned char> &inv_sbox = tables::invs();
	for (int r = 0; r < 4; ++r) {
		for (int c = 0; c < Nb; ++c) {
			int row = state[r][c] >> 4, col = state[r][c] & 0x0f;
			assert((unsigned char)((row << 4) | col) == state[r][c]);
			state[r][c] = inv_sbox[row*16+col];
		}
	}
}

// only circularly shift the last 3 rows
void shiftRows(vector<vector<unsigned char> > &state)
{
	for (int i = 1; i <=3; ++i) {
		left_circular_shift(state[i], i);
	}
}

void invShiftRows(vector<vector<unsigned char> > &state)
{
	for (int i = 1; i <= 3; ++i) {
		left_circular_shift(state[i], 4-i);
	}
}

void mixColumns(vector<vector<unsigned char> > &state)
{
	vector<vector<unsigned char> > _s = state;
	const vector<unsigned char> &P = tables::p();
	for (int c = 0; c < Nb; ++c) {
		state[0][c] = big_dot(P[3], _s[0][c]) ^ big_dot(P[0], _s[1][c]) ^ big_dot(P[1], _s[2][c]) ^ big_dot(P[2], _s[3][c]);
		state[1][c] = big_dot(P[2], _s[0][c]) ^ big_dot(P[3], _s[1][c]) ^ big_dot(P[0], _s[2][c]) ^ big_dot(P[1], _s[3][c]);
		state[2][c] = big_dot(P[1], _s[0][c]) ^ big_dot(P[2], _s[1][c]) ^ big_dot(P[3], _s[2][c]) ^ big_dot(P[0], _s[3][c]);
		state[3][c] = big_dot(P[0], _s[0][c]) ^ big_dot(P[1], _s[1][c]) ^ big_dot(P[2], _s[2][c]) ^ big_dot(P[3], _s[3][c]);
	}
}

void invMixColumns(vector<vector<unsigned char> > &state)
{
	vector<vector<unsigned char> > _s = state;
	const vector<unsigned char> &INVP = tables::invp();
	for (int c = 0; c < Nb; ++c) {
		state[0][c] = big_dot(INVP[3], _s[0][c]) ^ big_dot(INVP[0], _s[1][c]) ^ big_dot(INVP[1], _s[2][c]) ^ big_dot(INVP[2], _s[3][c]);
		state[1][c] = big_dot(INVP[2], _s[0][c]) ^ big_dot(INVP[3], _s[1][c]) ^ big_dot(INVP[0], _s[2][c]) ^ big_dot(INVP[1], _s[3][c]);
		state[2][c] = big_dot(INVP[1], _s[0][c]) ^ big_dot(INVP[2], _s[1][c]) ^ big_dot(INVP[3], _s[2][c]) ^ big_dot(INVP[0], _s[3][c]);
		state[3][c] = big_dot(INVP[0], _s[0][c]) ^ big_dot(INVP[1], _s[1][c]) ^ big_dot(INVP[2], _s[2][c]) ^ big_dot(INVP[3], _s[3][c]);
	}
}

void encrypt(
	const string &key,
	const string &tablefile,
	istream &in
){
	vector<vector<unsigned char> > state(4, vector<unsigned char>(4, 0));
	int i;
	unsigned char byte;
	// state is packed column wise.
	for (i = 0; i < 16 && in >> noskipws >> byte; ++i) {
		state[i%4][i/4] = byte;
	}
	if (i < 16) {
		fatal("The size of the input file is less than 16 bytes\n");
	}
	keyexpand(key, tablefile, false);
	vector<vector<unsigned char> >::iterator k_itr = roundKeys::keys().begin();
	printf("round[%2d].input    %s\n", 0, vecs_to_str(state).c_str());
	printf("round[%2d].k_sch    %s\n", 0, vecs_to_str(k_itr, k_itr+4).c_str());
	addRoundKey(state, 0);
	for (int round = 1; round < Nr; ++round) {
		advance(k_itr, 4);
		printf("round[%2d].start    %s\n", round, vecs_to_str(state).c_str());
		subBytes(state);
		printf("round[%2d].s_box    %s\n", round, vecs_to_str(state).c_str());
		shiftRows(state);
		printf("round[%2d].s_row    %s\n", round, vecs_to_str(state).c_str());
		mixColumns(state);
		printf("round[%2d].m_col    %s\n", round, vecs_to_str(state).c_str());
		addRoundKey(state, round*Nb);
		printf("round[%2d].k_sch    %s\n", round, vecs_to_str(k_itr, k_itr+4).c_str());
	}
	printf("round[%2d].start    %s\n", Nr, vecs_to_str(state).c_str());
	subBytes(state);
	printf("round[%2d].s_box    %s\n", Nr, vecs_to_str(state).c_str());
	shiftRows(state);
	printf("round[%2d].s_row    %s\n", Nr, vecs_to_str(state).c_str());
	advance(k_itr, 4);
	printf("round[%2d].k_sch    %s\n", Nr, vecs_to_str(k_itr, k_itr+4).c_str());
	addRoundKey(state, Nr*Nb);
	printf("round[%2d].output   %s\n", Nr, vecs_to_str(state).c_str());
}

void decrypt(
	const string &key,
	const string &tablefile,
	istream &in
){
	vector<vector<unsigned char> > state(4, vector<unsigned char>(4, 0));
	int i;
	unsigned char byte;
	// state is packed column wise.
	for (i = 0; i < 16 && in >> noskipws >> byte; ++i) {
		state[i%4][i/4] = byte;
	}
	if (i < 16) {
		fatal("The size of the input file is less than 16 bytes\n");
	}
	keyexpand(key, tablefile, false);
	vector<vector<unsigned char> >::iterator k_itr = roundKeys::keys().end();
	printf("round[%2d].iinput   %s\n", 0, vecs_to_str(state).c_str());
	printf("round[%2d].ik_sch   %s\n", 0, vecs_to_str(k_itr-4, k_itr).c_str());
	addRoundKey(state, Nr*Nb);
	for (int round = 1; round < Nr; ++round) {
		advance(k_itr, -4);
		printf("round[%2d].istart   %s\n", round, vecs_to_str(state).c_str());
		invShiftRows(state);
		printf("round[%2d].is_row   %s\n", round, vecs_to_str(state).c_str());
		invSubBytes(state);
		printf("round[%2d].is_box   %s\n", round, vecs_to_str(state).c_str());
		addRoundKey(state, (Nr-round)*Nb);
		printf("round[%2d].ik_sch   %s\n", round, vecs_to_str(k_itr-4, k_itr).c_str());
		printf("round[%2d].ik_add   %s\n", round, vecs_to_str(state).c_str());
		invMixColumns(state);
		// printf("round[%2d].im_col   %s\n", round, vecs_to_str(state).c_str());
	}
	printf("round[%2d].istart   %s\n", Nr, vecs_to_str(state).c_str());
	invShiftRows(state);
	printf("round[%2d].is_row   %s\n", Nr, vecs_to_str(state).c_str());
	invSubBytes(state);
	printf("round[%2d].is_box   %s\n", Nr, vecs_to_str(state).c_str());
	advance(k_itr, -4);
	printf("round[%2d].ik_sch   %s\n", Nr, vecs_to_str(k_itr-4, k_itr).c_str());
	addRoundKey(state, 0);
	printf("round[%2d].ioutput  %s\n", Nr, vecs_to_str(state).c_str());
}
