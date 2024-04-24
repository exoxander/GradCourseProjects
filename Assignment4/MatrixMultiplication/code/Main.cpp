/*
		===================
		Alexander Lustig
		CS6420
		Assignment 4 Part 1: Matrix Multiplication
		Dr. Rague
		Due March 26 2023
		===================
		This program takes two arguments 
		1: the size n of the n x n matricies it will read in, whre n is a power of 2
		2: the path of a txt file where it will read the matricies from

		the program then runs three different matrix multiplication algorithms and prints the results to the console
		along with the processing time taken by each algorithm

		Unfortunately there is a bug in my implimentation of strassens algorithm causing the final matrix to be incorrect
		i am very certain the issues are in

		-------------------------------< Reflection block >------------------------------
		In my current implimentation of both the recursive and strassens matrix multiplication
		algorithms, the function spends a lot of time building new matricies (double arrays).
		As it stands, for the largest matrix i tested (64x64) strassens algorithm performed notably and reliably faster than
		the recursive algorithm, howver both were significantly slower than the iterative approach.
		If i were to remake both functions to strictly operate on partitions of the original input
		arrays instead of building new sub-arrays, i beleive their performance would increase signficantly
		and its possible that strassens algorithm would surpass the iterative algorithm in speed.
		---------------------------------------------------------------------------------
*/
#include <iostream>
#include <fstream>
#include <string>
#include "Functions.h"
#include <chrono>

using std::cout;
using std::fstream;
using std::string;


int main(int argc, char* argv[]) {

	//command line argument format [size] [path]
	int size = 0;
	string path = "";
	string data = "";
	std::ifstream file;
	Functions f = Functions();
	double* a = nullptr;
	double* b = nullptr;
	double* c = nullptr;

	//stop program if too few arguments
	if (argc < 3) {
		cout << "Not enough arguments in program call, stopping...\n";
		return -1;
	}

	size = atoi(argv[1]);
	path = argv[2];
	file.open(path);

	//stop program if there is a problem with the file
	if (!file.good()) {
		cout << "Unable to open file: " << path << '\n';
		file.close();
		return -1;
	}

	std::getline(file, data);//matrix a
	a = f.make_matrix(size,data);

	std::getline(file, data);//matrix b
	b = f.make_matrix(size, data);


	//show both input matricies in the console
	cout << "Matrix A:\n";
	f.print_in_2d(size,a);
	cout << "\n";
	cout << "Matrix B:\n";
	f.print_in_2d(size, b);

	//iteratively multiply with time tracking
	auto start = std::chrono::steady_clock::now();
	c = f.iterative_mult(size, a, b);
	auto end = std::chrono::steady_clock::now();
	auto duration = end - start;

	//print matrix to console
	cout << "\n";
	cout << "Matrix C (iterative):\n";
	cout << std::chrono::duration<double, std::milli>(duration).count() << " miliseconds\n";
	f.print_in_2d(size, c);
	c = nullptr;
	
	//Recursive multiply with time tracking
	start = std::chrono::steady_clock::now();
	c = f.recursive_mult(size, a, b);
	end = std::chrono::steady_clock::now();
	duration = end - start;

	//print matrix to console
	cout << "\n";
	cout << "Matrix C (recursive):\n";
	cout << std::chrono::duration<double, std::milli>(duration).count() << " miliseconds\n";
	f.print_in_2d(size, c);
	c = nullptr;
	
	//strassens algrorithm with time tracking
	start = std::chrono::steady_clock::now();
	c = f.strassen_mult(size, a, b);
	end = std::chrono::steady_clock::now();
	duration = end - start;

	//print matrix to console
	cout << "\n";
	cout << "Matrix C (strassen):\n";
	cout << std::chrono::duration<double, std::milli>(duration).count() << " miliseconds\n";
	f.print_in_2d(size, c);
	return 0;
}  