/*
		===================
		Contains the Greedy, Top-down, Bottom-up, and print function declarations from the instructions
		as well as the helper function fillTable()
		values contains all of the usable bill values
		table is a vector containing vectors, each sub-vector represents the optimal bill arrangement for the value of the tables index
		(table[10] = vector [7 + 1 + 1 + 1])
		===================
*/


#pragma once
#include <vector>
#include <iostream>
using std::vector;

class Functions {
public:
	//bill values
	int values[8] = { 1,4,7,13,28,52,91,365 };

	//table[target] is the optimal arrangement of bills for the value target
	vector<vector<int>> table;

	//fill / overwrite the table with empty values and the bill values
	void fillTable(int size);

	//greedy solution, always picks the largest bill that does not exceed the amount needed
	vector<int> step_b_greedy(int target, int lastBill = 7);

	//recursive Top-dopwn dynamic programming
	vector<int> step_f_dl_top_down(int target);

	//iterative Bottom-up dynamic programming
	vector<int> step_g_dl_bottom_up(int target);

	//print to console
	void step_j_dl_print_change(vector<int> _values);
};