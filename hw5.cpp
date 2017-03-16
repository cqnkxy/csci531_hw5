#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "utility.h"
#include "tablecheck.h"
#include "crypt.h"
#include "modprod.h"
#include "keyexpand.h"

using namespace std;

const bool DEBUG = false;

void malformed_command() 
{
	const string USAGE = 
		"\t\nhw5 tablecheck -t=tablefile\n"
	    "\t\nhw5 modprod -p1=poly1 -p2=poly2\n"
	    "\t\nhw5 keyexpand -k=key -t=tablefile\n"
	    "\t\nhw5 encrypt -k=key -t=tablefile [file]\n"
	    "\t\nhw5 decrypt -k=key -t=tablefile [file]\n"
	    "\t\nhw5 inverse -p=poly\n";
	cerr << "Malformed command. Use it as:" << endl << USAGE << endl;
	exit(1);
}

void parse_tablecheck(int argc, char *argv[]) 
{
	if (argc != 3) {
		malformed_command();
	}
	if (strncmp(argv[2], "-t=", 3) != 0) {
		malformed_command();
	}
	check_tables(argv[2]+3);
}

void parse_modprod(int argc, char *argv[])
{
	if (argc != 4) {
		malformed_command();
	}
	string poly1, poly2;
	for (int i = 2; i <= 3; ++i) {
		string opt(argv[i]);
		if (opt.substr(0, 4) == "-p1=") {
			poly1 = opt.substr(4, -1);
		} else if (opt.substr(0, 4) == "-p2=") {
			poly2 = opt.substr(4, -1);
		} else {
			malformed_command();
		}
	}
	if (poly1.size() != 8 || poly2.size() != 8) {
		fatal("poly1 and poly2 should both be of length 8\n.");
	}
	cout << modprod(poly1, poly2) << endl;
}

void parse_keyexpand(int argc, char *argv[])
{
	if (argc != 4) {
		malformed_command();
	}
	string key, tablefile;
	for (int i = 2; i <= 3; ++i) {
		string opt(argv[i]);
		if (opt.substr(0, 3) == "-k=") {
			key = opt.substr(3, -1);
		} else if (opt.substr(0, 3) == "-t=") {
			tablefile = opt.substr(3, -1);
		} else {
			malformed_command();
		}
	}
	keyexpand(key, tablefile);
}

void parse_cmd_run(int argc, char *argv[]) 
{
	if (argc < 3) {
		malformed_command();
	} else if (strcmp(argv[1], "tablecheck") == 0) {
		parse_tablecheck(argc, argv);
	} else if (strcmp(argv[1], "modprod") == 0){
		parse_modprod(argc, argv);
	} else if (strcmp(argv[1], "keyexpand") == 0) {
		parse_keyexpand(argc, argv);
	} else {
		malformed_command();
	}
}

int main(int argc, char *argv[]) 
{
	parse_cmd_run(argc, argv);
	return 0;
}
