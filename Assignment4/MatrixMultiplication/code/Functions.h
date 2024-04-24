/*=======================
* Contains the Functions class
* the Functions class holds the functions required by the assignment as well as
* a number of smaller helper functions
=======================*/
#pragma once
#include <fstream>
#include <string>
#include <iostream>

class Functions {
public:
	//square matrix multiplication
	double* iterative_mult(int size, double* a, double* b);

	//recursive matrix multiplication
	double* recursive_mult(int size, double* a, double* b);

	//strassens matrix multiplication
	double* strassen_mult(int size, double* a, double* b);

	//print given matrix to the console
	void print_in_2d(int size, double* matrix);

	//return a matrix from a string, use with getLine
	double* make_matrix(int size, std::string data);

	//add two matricies together and return the result
	double* add_matrix(int size, int row, int col, double* a, double* b);

	//subtract b from a and return the result matrix
	double* subtract_matrix(int size, int row, int col, double* a, double* b);

	//return a sub matrix of the given size from the given coords row = y col = x
	double* partition(int size, int _row, int _col, double* matrix);

	//place b inside a at given coords row = y col = x  
	void place(int size, int row, int col, double* a, double* b);

};