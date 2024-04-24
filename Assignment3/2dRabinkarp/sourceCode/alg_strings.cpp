/******************************************************************************
 *  File: alg_strings.cpp
 *
 *  An implementation of string-matching classes.
 *=========================================================
 * modified to allow searching of 2d patterns
 * new functions
 * hash2d:				hashes a 2d block of text and returns the hash
 * hashNextCol			hashes a column from a 2d block of text and returns the value
 * check2d				checks if a block of text is an exact match for the classes existing pattern
 * search2d				searches through a block of text for a match to the pattern hash, if a match is found it calls check2d to verify
 * getRandomPattern		returns a random 2d block of text from a string array, used to grab a random pattern from text
 * getRandomText		similar to getRandomPattern, except it pulls from the supplied file and returns the new random 2d text 
 ******************************************************************************/

#include <iostream>
#include <stack>
#include <list>
#include <string>
#include <random>
#include <chrono>
#include <fstream>
#include "alg_strings.h"
using std::string;

/******************************************************************************
 *  Class: RabinKarp
 *  A class implementing the Rabin-Karp algorithm
 ******************************************************************************/
long RabinKarp::long_random_prime() {
	int min = 0, max = 49;
	long primes[] = {
	  2094665479L, 1783990163L, 2094521287L, 2134397081L, 2126326253L,
	  1957216747L, 1436547389L, 1428780767L, 2075625529L, 1593123733L,
	  2132587157L, 1965562429L, 1164701777L, 1568991883L, 2130061793L,
	  1075370311L, 1711832929L, 2054631589L, 1587361861L, 1435348609L,
	  1332084959L, 1465215911L, 2088173753L, 1933073123L, 1319415599L,
	  1211741129L, 1487473783L, 1656920599L, 1817614213L, 1838911937L,
	  1697951429L, 1673793083L, 1971101663L, 1570547117L, 1869368041L,
	  1855484017L, 2057695543L, 1806695647L, 2082498797L, 2090345119L,
	  1349212999L, 1456810283L, 1271362889L, 1959057733L, 1073964823L,
	  1315871351L, 1308843649L, 1543027127L, 1230659387L, 1828780297L };

	static std::default_random_engine en;
	en.seed(std::chrono::system_clock::now().time_since_epoch().count());
	static std::uniform_int_distribution<int> dist{ min, max };

	return primes[dist(en)];
}
long RabinKarp::hash(const std::string& key, int m) const {
	long h = 0;
	for (int j = 0; j < m; j++) {
		h = (R * h + key[j]) % q;
	}
	return h;
}


//row = text[]
//col = text[][]
//creates a 2d hash of the input string[] over the given range
long RabinKarp::hash2d(const string* key, int m, int r, int c) const {
	long h = 0;
	long t = 0;
	for (int col = 0; col < m; col++) {//loop horizontally
		for (int row = 0; row < m; row++) {//loop vertically
			t = (R * t + key[row + r][col + c]) % RQ;//use prime RQ (251) so values dont overflow past 1 byte (256)
		}
		//build next step of 2d hash and add value to 1d hash array
		h = (R * h + t) % q;
		hashedArr[col] = t;
		t = 0;
	}
	return h;
}

//creates a 1d hash of a column which is returned and added to the 2d hash as part of shifting
long RabinKarp::hashNextCol(const string* text, int m, int r, int c) const {
	long h = 0;
	long t = 0;

	for (int row = 0; row < m; row++) {//loop vertically
		t = (R * t + text[row + r][c]) % RQ;//use prime RQ so values dont overflow past 1 byte (256)
	}

	//build new 1d hash and return
	h = (R * h + t) % q;
	t = 0;
	return h;
}

bool RabinKarp::check(const std::string& txt, int i) const {
	for (int j = 0; j < m; j++) {
		if (pat[j] != txt[i + j]) {
			return false;
		}
	}
	return true;
}

//2d overload, checks if the spot in the text matches the pattern from the specified indecies
bool RabinKarp::check2d(const string* text, int r, int c) const {
	for (int row = 0; row < m; row++) {
		for (int col = 0; col < m; col++) {
			if (text[row + r][col + c] != pat2d[row][col]) {
				return false;
			}
		}
	}
	return true;
}

RabinKarp::RabinKarp(const std::string& pat) :pat(pat), R(256) {
	m = pat.size();
	q = long_random_prime();

	RM = 1;
	for (int i = 1; i <= m - 1; i++) {
		RM = (R * RM) % q;
	}
	pat_hash = hash(pat, m);
}

//Overload for 2d matching
RabinKarp::RabinKarp(const string* pattern) {
	pat2d = pattern;
	hashedArr = new long[m];
	R = 256;
	m = pat2d[0].size();
	q = long_random_prime();
	RQ = 251;
	RM = 1;
	for (int i = 1; i <= m - 1; i++) {
		RM = (R * RM) % q;
	}

	RN = 1;
	for (int i = 1; i <= m - 1; i++) {
		RN = (R * RN) % RQ;
	}

	pat_hash = hash2d(pat2d, m);
}

int RabinKarp::search(const std::string& txt) const {
	int n = txt.length();
	if (n < m) return n;
	long txt_hash = hash(txt, m);

	// check for match at offset 0
	if ((pat_hash == txt_hash) && check(txt, 0))
		return 0;

	// check for hash match; if hash match, check for exact match
	for (int i = m; i < n; i++) {
		// Remove leading digit, add trailing digit, check for match.
		txt_hash = (txt_hash + q - RM * txt[i - m] % q) % q;
		txt_hash = (txt_hash * R + txt[i]) % q;

		// match
		int offset = i - m + 1;
		if ((pat_hash == txt_hash) && check(txt, offset)) {
			return offset;
		}
	}

	return n; // no match
}

//overoad for 2d searching, hashes columns to become a single row, and then hashes that row
//shifts horizontally to utilize the efficient modulus trick
string RabinKarp::search2d(string* text) const {
	int n = text[0].length();
	if (n < m) return "[0,0]";
	long text_hash = hash2d(text, m);

	for (int row = 0; row < n - m; row++) {
		for (int col = 0; col < n - m + 1; col++) {//for all columns

			//check for hash match, then check for exact match
			if (text_hash == pat_hash && check2d(text, row, col)) {
				return ("[" + std::to_string(row) + "," + std::to_string(col) + "]");
			}

			//update 1d hash array
			//shift array items left
			for (int x = 0; x < m; x++) {
				if (x > 0) {
					hashedArr[x - 1] = hashedArr[x];
				}
			}
			//hash next column
			hashedArr[m - 1] = hashNextCol(text, m, row, col + m);

			//redo 2d hash using updated array
			text_hash = 0;
			for (int x = 0; x < m; x++) {
				text_hash = (R * text_hash + hashedArr[x]) % q;
			}
		}
		//after shifting to the end the columns, the existing hash is useless
		//this shifts the algorithm down 1 row and readies it to shift to the right
		text_hash = hash2d(text, m, row + 1);
	}
	return "no match found"; // no match
}

//prints string arrays
void RabinKarp::print(const string* text) {
	int s = text[0].length();

	for (int x = 0; x < s; x++) {
		for (int y = 0; y < s; y++) {
			std::cout << text[x][y] << " ";
		}
		std::cout << "\n";
	}
}

//grabs and returns a random sub-pattern from a given text
string* RabinKarp::getRandomPattern(string* text, int size, int max)
{
	int rr;
	int rc;

	if (size < max) {
		srand(std::chrono::system_clock::now().time_since_epoch().count());
		rr = rand() % (max - size);
		srand(std::chrono::system_clock::now().time_since_epoch().count());
		rc = rand() % (max - size);
	}
	else {
		rr = 0;
		rc = 0;
	}
	string* value = new string[size];
	string t = "";
	for (int row = 0; row < size; row++) {
		for (int col = 0; col < size; col++) {
			t.push_back(text[rr + row][rc + col]);
		}
		value[row] = t;
		t = "";
	}
	return value;
}

//similar to grabRandomPattern but pulls from the given text file and returns this as the new randomText
string* RabinKarp::getRandomText(std::fstream & file, RabinKarp & r,string path, int size, string* old) {
	file.open(path);
	string line = "";
	int max = 1000;
	int rx;
	int ry;
	
	if (size < max) {
		srand(std::chrono::system_clock::now().time_since_epoch().count());
		rx = rand() % (max - size);
		srand(std::chrono::system_clock::now().time_since_epoch().count());
		ry = rand() % (max - size);
	}
	else {
		rx = 0;
		ry = 0;
	}
	//clean up previous string array
	string* del = old;
	old = new string[size];
	del->erase();

	//for random y getline
	//readfrom random x to random x + size
	//store in string array
	//move to next line size times
	for (int y = 0; y < ry + size; y++) {
		std::getline(file, line);

	}//now at random y position

	//for each row, push the characters into the string array for that row
	for (int y = 0; y < size; y++) {
		std::getline(file, line);

		old[y] = "";
		//text[y].reserve(size+1);
		for (int x = 0; x < size; x++) {
			old[y] += (line[rx + x]);//causes a memory error on allocation sometimes
		}
	}
	file.close();
	return old;
}