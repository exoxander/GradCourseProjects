/*
		===================
		Alexander Lustig
		CS6420
		Assignment 2 Part 2: Classifying edges in digraphs
		Dr. Rague
		Due Feb 05 2023
		===================
		Takes in a graph.txt file as an argument
		Uses the provided DFS algorithm and prints out the resulting tree and 
		the types of each vertecies edges

		I beleive the running time for the modified print function is O(|v| + |e|)
*/
#include "alg_graphs.h"
#include <string>
#include <fstream>
#include <iostream>
using std::string;
using std::cout;


int main(int argc, char* argv[]) {
	string path;
	std::ifstream file;
	int length = 0;
	string line;

	//stop program if too few arguments
	if (argc < 2) {
		cout << "Not enough arguments in program call, stopping...\n";
		return -1;
	}

	path = argv[1];
	file.open(path);

	//stop program if given file does not exist or is bad
	if (!file.good()) {
		cout << "Unable to open file: " << path << '\n';
		file.close();
		return -1;
	}

	//load first line as length and build digraph
	std::getline(file, line);
	length = stoi(line);
	Digraph data = Digraph(length);

	//load file info into "data"
	file >> data;

	//run provided DFS algorithm
	DepthFirstSearch DFS = DepthFirstSearch(data);

	//use modified DepthFirstSearch.str() to print the tree and all edge types to the console
	cout << DFS.str() << '\n';

	file.close();
	return 0;
}