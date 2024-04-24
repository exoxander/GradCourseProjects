/*
		===================
		Contains the definitions for all the functions in the follwing order

		fillTable():				helper function, fills / overwrites the table with -1s and bill values

		step_j_dl_print_change():	Formats and prints the input vector<int> to the console

		step_b_greedy():			Returns an arrangement of bills based on the greedy choice of the picking the largest bill that fits

		step_f_dl_top_down():		Returns an optimal bill arragement utilizing a top-down recursive algorithm

		step_g_dl_bottom_up():		Returns an optimal bill arrangement utilizing an bottom-up iterative algorithm
		===================
*/

#pragma once
#include "Functions.h"

//utility function, fills vector<int>[] table with empty vectors and bill values
void Functions::fillTable(int _size) {
	int bill = 0;

	//loop through the table, if i == bill value, table[i] = bill list
	//else, table[i] marked as empty
	for (int i = 0; i <= _size; i++) {
		vector<int> newValue;

		if (i == values[bill]) {
			//set as known bill value
			bill++;
			newValue.push_back(i);
		}
		else {
			//set as -1 to mark empty
			newValue.push_back(-1);
		}

		table.push_back(newValue);
	}
}
//-------------------------------------------------------------------Part J | print change to console
void Functions::step_j_dl_print_change(vector<int> _input) {
	for (int i = 0; i < _input.size(); i++) {
		if (i > 0) {
			std::cout << " + ";
		}
		std::cout << _input[i];
	}
	std::cout << '\n';
}


//-------------------------------------------------------------------Part B | greedy recursive
vector<int> Functions::step_b_greedy(int _target, int lastBill) {
	vector<int> result;
	vector<int> t;

	//base case, return an empty vector
	if (_target == 0) {
		return result;
	}

	//if bill fits in target, reduce target by bill and recurse
	if (_target >= values[lastBill]) {
		result.push_back(values[lastBill]);
		t = step_b_greedy(_target - values[lastBill], lastBill);
	}

	//if bill does not fit in target, move to next smaller bill and recurse
	else {
		t = step_b_greedy(_target, lastBill - 1);
	}

	//copy elements from t into result
	result.insert(result.end(), t.begin(), t.end());
	return result;
}

//--------------------------------------------------------------------Part F | Top down memoized recursive
vector<int> Functions::step_f_dl_top_down(int _target) {
	vector<int> result;
	int i = 0;

	//if this value has already been computed or is a bill value
	//return existing value
	if (table.at(_target).front()  > 0) {
		return table[_target];
	}

	//if value hasnt already been computed, recurse for all bills smaller than it
	//return vector with smallest size
	i = 0;
	while (_target >= values[i] && i < 8) {
		vector<int>temp = step_f_dl_top_down(_target - values[i]);

		//if this arrangement is smaller than the previous smallest, pick it
		if ( i == 0 || temp.size() < result.size()) {
			result = temp;
			result.emplace(result.begin(), values[i]);
		}
		
		i++;
	}

	//set this result in the table for future use
	table[_target] = result;

	return result;
}

//--------------------------------------------------------------------Part G | bottom up iterative
vector<int> Functions::step_g_dl_bottom_up(int _target) {
	vector<int> result;
	vector<int> temp;

	for (int i = 2; i <= _target; i++) {
		for (int b = 0; b < 7; b++) {

			//stop inner loop if bill larger than value
			if (i < values[b]) {
				break;
			}

			//find minimum combination from all bills smaller or equal to i
			//min of table[current - bills[i]] + bills[i]

			if (table.at(i).front() == -1) {
				//spot marked as empty, find minimum combination
				temp = table.at(i - values[b]);
				temp.emplace(temp.begin(), values[b]);
			}
			else {
				//if not marked as empty, use its arrangement as the baseline
				temp = table.at(i);
			}

			//set on first loop / check if a smaller arrangement is found
			if (b == 0 || temp.size() < result.size()) {
				result = temp;
			}
		}

		//put into table
		table.at(i) = temp;
	}

	result = table.at(_target);
	return result;
}