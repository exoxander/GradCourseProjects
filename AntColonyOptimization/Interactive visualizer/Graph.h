/*
	The graph is a list of verticies and a 2d (1d) array of the distances and directional pheromone weights
	every vertex has an edge to every other vertex

	the class also contains a number of graph related helper functions for building random graphs and 
	working with the ant pheromones
*/

#pragma once
#include <cmath>
#include <vector>
struct edge {
	float distance = 1;
	float pheramoneStrength = 100;
};

struct vertex {
public:
	//position in the world
	float x = 0;
	float y = 0;

	vertex(float _x = 0, float _y = 0) { x = _x; y = _y; };
};

class Graph {
public:
	vertex* vertecies;
	int size;
	edge* edges;

	//constructors
	Graph() { vertecies = nullptr; size = 0; edges = nullptr; };
	Graph(int _size);

	//destructor
	~Graph() { delete[] vertecies; delete[] edges; };

	//reduce the pheramone values
	void evaporate(float strength);

	//generates a graph of the given size within the input maximum values
	void generate(int maxX, int maxY);

	//build the distance matrix, do here and store so ants can quickly access it
	void computeDistances();

	//applies pheramones to all 
	void applyPheramones(std::vector<int> _input, float strength = 1);

	//resets the graph to its start state for re-runs
	void reset();

	//builds the graph using the specified hard-coded test
	void useTestGraph(int g);
};