/*
		===================
		Alexander Lustig
		CS6420
		Assignment 3 Part 2 KMP Prefix
		Dr. Rague
		Due Jan 22 2023
		===================
		This program takes in a string by command line and returns its prefix function for the Knuth-Morris-Pratt Algorithm
*/
#include <iostream>
using std::string;
using std::cout;


int main(int argv, char* argc[]) {

	string pattern = "";
	int m = 0;

	//check for input string and abort if not present
	if (argv > 1) {
		pattern = argc[1];
		m = pattern.length();
	}
	else {
		cout << "Not enough arguments in program call, Stopping.\n";
		return -1;
	}
	
	//the prefix function builder
	//made from the pseudocode on page 1006
	int k = 0;
	int* result = new int[m]();
	for (int q = 1; q < m; q++) {
		while (k > 0 && pattern[k] != pattern[q]) {
			k--;
		}
		if (pattern[k] == pattern[q]) {
			k++;
		}
		result[q] = k;
	}

	//output result to console
	for (int i = 0; i < m; i++) {
		cout << result[i] << " ";
	}
	cout << "\n";
	return 0;
}