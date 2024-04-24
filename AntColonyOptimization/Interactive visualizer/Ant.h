/*
	the ant
	contains all the ants internal information and the functions for traversing the graph
*/
#pragma once
#include "Graph.h"
#include <vector>
#include <random>
#include <chrono>

class Ant {
public:
	//how strongly the ant is attracted to pheramones vs distance
	float pheramoneWeight = 0;
	float distanceWeight = 0;

	//origin vertex and ants current position as the vertex list index
	int start = 0;
	int currentVertex = 0;

	//the global graph
	Graph* graph;

	//bool[] showing which vertecies have already been visited by this ant
	bool* visited;

	//total distance travelled by ant
	float totalDistance = 0;

	//the path of the ant
	std::vector<int> path;

	//constructors
	Ant() { graph = nullptr; visited = nullptr; };
	Ant(Graph* _graph, float _pWeight = 1, float _dWeight = 1, int _start = 0);

	//pick and move to an unvisited vertex, if all vertecies visited, return to start
	int decide();
	int move();

	//reset the internal state of the ant to re-run
	void reset();
};