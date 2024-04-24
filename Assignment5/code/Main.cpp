/*
		===================
		Alexander Lustig
		CS6420
		Assignment 5: Greedy algorithms vs Dynamic Programming
		Dr. Rague
		Due March 9 2023, Extended to March 14
		===================

		This program runs a greedy, recursive top-down, and iterative bottom up algorithm to find the minimum 
		number of bills that can be returned for that value.

		If a number is input in the program call, the program runs all three algorithms on that value and prints their outputs to the console.

		If no number is input, the program runs the Top-down and Bottom-up functions on the provided test cases and prints their outputs to the console.
*/
#include <iostream>
#include <string>
#include "Functions.h"
using std::cout;
using std::string;

int main(int argc, char* argv[]) {
	int amount = 10;
	int testValues[] = {118, 10, 32, 100, 148, 414, 732, 1278, 101, 111};//10
	vector<int> result;
	Functions f;

	if (argc > 1) {
		amount = std::stoi(argv[1]);
	}	

	//run input number if argument provided
	if (argc > 1) {

		//greedy compute and print
		cout << "Value: " << amount << '\n';
		cout << "Greedy: ";
		result = f.step_b_greedy(amount);
		f.step_j_dl_print_change(result);


		//top down recursive compute and print
		f.fillTable(amount);
		result = f.step_f_dl_top_down(amount);
		cout << "Top-down: ";
		f.step_j_dl_print_change(result);

		//bottom up iterative compute and print
		f.fillTable(amount);
		cout << "Bottom-up: ";
		result = f.step_g_dl_bottom_up(amount);
		f.step_j_dl_print_change(result);
	}
	else {

		//loop through test cases if no argument input
		for (int i = 0; i < 10; i++) {
			cout << "\n=================================\n\tValue: " << testValues[i] << '\n';

			//top down recursive compute and print
			f.fillTable(testValues[i]);
			result = f.step_f_dl_top_down(testValues[i]);
			cout << "\tTop-down: ";
			f.step_j_dl_print_change(result);

			//bottom up iterative compute and print
			f.fillTable(testValues[i]);
			cout << "\tBottom-up: ";
			result = f.step_g_dl_bottom_up(testValues[i]);
			f.step_j_dl_print_change(result);
		}
	}	

	return 0;
}

