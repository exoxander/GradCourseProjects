/******************************************************************************
 *  File: alg_strings.h
 * 
 *  A header file defining string-matching classes. 
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

#ifndef _ADV_ALG_STRINGS_H_
#define _ADV_ALG_STRINGS_H_

#include <iostream>
#include <string>
using std::string;

/******************************************************************************
 *  Class: RabinKarp
 *  A class implementing the Rabin-Karp algorithm
 ******************************************************************************/
class RabinKarp {
private:
  const string* pat2d; //array of strings
  long* hashedArr;
  std::string pat;
  long pat_hash;   // Pattern hash value
  int m;           // Pattern length
  long q;          // A large prime, small enough to avoid overflow
  int RQ;			//prime used for rows in the 2d pattern
  long RN;			// R(m-1) % RQ
  int R;           // Radix
  long RM;         // R^(M-1) % Q

  long hash(const std::string &key, int m) const;
  long hash2d(const string* key, int m, int r = 0, int c = 0) const;
  long hashNextCol(const string* key, int m, int r = 0, int c = 0) const;
  bool check(const std::string &txt, int i) const;
  bool check2d(const string* text, int row, int col) const;
  
  static long long_random_prime();

public:
  RabinKarp(const std::string &pat);
  RabinKarp(const string* pattern);
  int search(const std::string &txt) const;
  void print(const string* text);
  string search2d(string* text) const;
  string* getRandomPattern(string* text, int size, int max);
  string* getRandomText(std::fstream & file, RabinKarp & r,string path, int size, string* old);
};

#endif