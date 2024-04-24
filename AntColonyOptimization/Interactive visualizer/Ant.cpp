/*
	Definitions for all functions for the ant

	move(): moves the ant to a chosen vertex (interally calles decide)

	decide(): uses the std::discrete_distribution and a vector of calculated weights to probabilistically decide on where to move next

	reset(): resets the ants internal information to allow mutliple runs
*/
#include "Ant.h"

//constructor
Ant::Ant(Graph* _graph, float _pWeight, float _dWeight, int _start) {
	graph = _graph;
	pheramoneWeight = _pWeight;
	distanceWeight = _dWeight;
	currentVertex = _start;
	start = _start;
	totalDistance = 0;

	//set up visited array, mark start as visited
	visited = new bool[graph->size];
	for (int i = 0; i < graph->size; i++) {
		visited[i] = false;
	}
	visited[_start] = true;
}

//moves the ant to the chosen unvisited vertex
int Ant::move() {
	int oldvertex = currentVertex;

	//make decision
	currentVertex = decide();

	//move ant to new chosen vertex
	visited[currentVertex] = true;
	path.push_back(currentVertex);

	//log distance
	totalDistance += graph->edges[graph->size * oldvertex + currentVertex].distance;

	return currentVertex;
}

/*
	this is where the magic happens
	the ant will pick a new unvisited vertex based on the strength of the pheramone trail and its distance
	current implimentation: (pheromone strength * pheromone weight) / ( distance* distance weight)^2
*/
int Ant::decide() {
	std::vector<float> weights;
	int decision = -1;
	bool unvisitedVertexFound = false;

	//build connection weight based on distance and pheramones
	//for all distances from the current vertex, compute weight from pheramone value / distance
	for (int col = 0; col < graph->size; col++) {
		float newWeight = 0;

		//if vertex not visited, calculate edge weight 
		//if already visited, weight is defaulted to zero and will not be picked
		if (!visited[col]) {
			float weightedPheramone = graph->edges[graph->size * currentVertex + col].pheramoneStrength * pheramoneWeight;
			float weightedDistance = graph->edges[graph->size * currentVertex + col].distance * distanceWeight;

			weightedDistance *= weightedDistance;
			newWeight = weightedPheramone / weightedDistance;

			unvisitedVertexFound = true;
		}

		weights.push_back(newWeight);
	}

	//if no vertex has been unvisited, return to start
	if (!unvisitedVertexFound) {
		decision = start;
	}
	else {
		//randomly pick from the weights of unvisited vertecies
		std::mt19937 generator(std::chrono::duration_cast<std::chrono::nanoseconds> (std::chrono::system_clock::now().time_since_epoch()).count());
		std::discrete_distribution<int> dist(weights.begin(), weights.end());
		decision = dist(generator);
	}

	return decision;
}

//resets the internal state to ready it for the next trial
void Ant::reset() {
	currentVertex = start;
	totalDistance = 0;

	path.clear();

	//reset visited array
	for (int i = 0; i < graph->size; i++) {
		visited[i] = false;
	}
	visited[start] = true;
}