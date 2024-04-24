/*
		===================
		Alexander Lustig
		CS6420
		Assignment 3 Part 1: 2D Rabin-karp matching
		Dr. Rague
		Due Feb 19 2023
		===================
		This program and modifed RabinKarp class check for 2d pattern matches
		the program takes in a 1000x1000 string file as an argument and runs randomized matching tests using that file
		if no file is supplied the program runs several hard coded tests
		while the program works for most cases, it does incorrectly miss some patterns that occur near the bottom right corner of the text
		the program also occasionally throws an error while closing, all the tests still run normally and the output is still valid
*/
#include <iostream>
#include <string>
#include "alg_strings.h"
#include <fstream>
#include <random>
#include <chrono>
using std::fstream;
using std::string;
using std::cout;


int main(int argc, char* argv[]) {

	string path = "";
	string result = "";
	fstream file;
	string hr = "=================\n";
	string line;
	string* text;
	
	//hard coded patterns
	string p1[2] = { "kl","op" };//5x5 2,2
	string p2[2] = { "ea","ct" };//8x8 5,3
	string p3[2] = { "gh","kl" };//4x4 5,3
	string p4[2] = { "on","ia" };//8x8 3,1

	//hard coded texts
	string test_text8x8[8] = { "absolute","bacteria","computer","concrete","diameter","endeavor","fraction","inspiree" };
	string test_text4x4[4] = { "abcd","efgh","ijkl","mnop" };
	string test_text5x5[5] = { "absol","bacte","aoklu","coopr","diame"};

	string* randomPattern;
	string* randomText = new string[1];
	randomText[0] = "none";

	RabinKarp r = RabinKarp(p1);

	//if no arguments supplied, do hardcoded tests
	if (argc < 2) {
		/*=================
		* HARDCODED TESTS
		* run if no file is supplied
		=================*/
		//test 1, P1 & 5x5
		r = RabinKarp(p1);
		cout << hr;
		r.print(test_text5x5);
		cout << "pattern:\n";
		r.print(p1);
		result = r.search2d(test_text5x5);
		cout << result << "\n";

		//test 2, P2 & 8x8
		r = RabinKarp(p2);
		cout << hr;
		r.print(test_text8x8);
		cout << "pattern:\n";
		r.print(p2);
		result = r.search2d(test_text8x8);
		cout << result << "\n";

		//test 3, P3 & 4x4
		r = RabinKarp(p3);
		cout << hr;
		r.print(test_text4x4);
		cout << "pattern:\n";
		r.print(p3);
		result = r.search2d(test_text4x4);
		cout << result << "\n";

		//test 4, P4 & 8x8
		r = RabinKarp(p4);
		cout << hr;
		r.print(test_text8x8);
		cout << "pattern:\n";
		r.print(p4);
		result = r.search2d(test_text8x8);
		cout << result << "\n";
	}
	else {
		/*===========================
		* RANDOMIZED TESTS
		* run if a file is supplied as an argument
		===========================*/
		path = argv[1];
		file.open(path);

		//stop program if given file does not exist or is bad
		if (!file.good()) {
			cout << "Unable to open file: " << path << '\n';
			file.close();
			return -1;
		}		
		file.close();
		int patterns[4] = { 2, 8, 16, 64 };
		int sizes[4] = { 5, 25, 100, 1000 };
		
		for (int i = 0; i < 4; i++) {
			randomText = r.getRandomText(file, r, path, sizes[i], randomText);
			randomPattern = r.getRandomPattern(randomText, patterns[i], sizes[i]);

			cout << hr << "Random test : " << i + 1 << "\n" << hr;
			if (i < 2) {
				cout << "text:\n";
				r.print(randomText);
				cout << "pattern:\n";
				r.print(randomPattern);
				cout << "location [row,col]:\n";
			}


			RabinKarp a = RabinKarp(randomPattern);
			result = a.search2d(randomText);
			cout << result << "\n";
		}		
	}
	return 0;
}