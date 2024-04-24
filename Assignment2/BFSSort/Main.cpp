/*
		===================
		Alexander Lustig
		CS6420
		Assignment 2 Part 1: Topological sort using Breadth-First Search
		Dr. Rague
		Due Feb 05 2023
		===================
		Takes in a graph.txt as an argument
		Runs a Breadth-First search and topological sort on resulting tree
		Prints the resulting topological order and whether that order is unique to the console
*/
#include "alg_graphs.h"
#include "Algorithms.h"
#include <istream>
#include <fstream>
#include <string>
#include <iostream>
using std::cout;
using std::string;

int main(int argc, char* argv[]) {
	string path;
	std::ifstream file;
	int length = 0;
	string line;
	list<int> topoList = list<int>();
	bool unique = false;
	Algorithms a = Algorithms();

	//stop program if no path specified
	if (argc < 2) {
		cout << "Not enough arguments in program call, stopping...\n";
		return -1;
	}

	path = argv[1];

	file.open(path);

	//stop program if file does not exist / is bad
	if (!file.good()) {
		cout << "Unable to open file: " << path << '\n';
		file.close();
		return -1;
	}

	//load first line as length and build Digraph "data"
	std::getline(file, line);
	length = stoi(line);
	Digraph data = Digraph(length);

	//load file info into "data"
	file >> data;

	//run topological sort and check if the solution is unique
	topoList = a.TOPOSORT(data, length);
	unique = a.UNIQUETOPO(data, topoList, length);

	//print results to the console
	cout << "\nTopological sort: ";
	for (list<int>::iterator it = topoList.begin(); it != topoList.end(); ++it) {
		cout << *it << " ";
	}
	cout << "\nUnique: " << std::boolalpha << unique << "\n";
	

	
	//end
	file.close();
	return 0;
}