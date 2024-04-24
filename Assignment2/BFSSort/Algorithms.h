/*===================================
Declares the class "Algorithms" and struct "vertex"
and their functions
===================================*/

#pragma once
#include <list>
#include <stack>
#include <queue>
#include "alg_graphs.h"
using std::list;
using std::stack;
using std::queue;

class Algorithms {
public:

	//return list<int> representing the order of the sort
	list<int> TOPOSORT(Digraph input, int size);

	//returns a bool indicating of the given topological sort is unique
	bool UNIQUETOPO(Digraph input, list<int> topoList, int size);
};

struct vertex {
	int id;
	int parent;
	int inDegree;
	int state;//color, 0 = white, 1 = grey, 2 = black
	list<int> edges;
};



