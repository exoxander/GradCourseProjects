/*============================================================
		Alexander Lustig
		CS6420
		Assignment 6: Exploring ant colony optimization
		Dr. Rague
		Due April 23 2023

		This file runs my ant colony optimization algorithm on a hard coded graph and returns the shortest path found
		for that run and compares that value to the shortest path ever found by the colony in my testing

		optionally also shows every shortest distance as its found or every single distance the swarm finds

		arguments:
		...\program name | number of ant trials (n >= 1) | print shortest path (0 or 1) | print every path (0 or 1) | if n <= 5, runs a hard coded test case, if n > 5, runs a generated graph of size n
		there are diminishing returns for numbers of ant trials beyond 1000
============================================================*/
#include <iostream>
#include <string>
#include "Ant.h"
using std::cout;
using std::string;


int main(int argc, char* argv[]) {

	int graphInputValue = 1;
	int antTrials = 1000;
	bool showShortestPath = false;
	bool showEveryAntPath = false;
	bool usingRandomGraph = false;

	//capture command line arguments
	switch (argc) {
	case 5:
		if (std::stoi(argv[4]) > 5) {	
			usingRandomGraph = true;
		}
		graphInputValue = std::stoi(argv[4]);
	case 4:
		showEveryAntPath = std::stoi(argv[3]);
	case 3:
		showShortestPath = std::stoi(argv[2]);
	case 2:
		antTrials = std::stoi(argv[1]);
	}

	//set up graph
	Graph graph = Graph((graphInputValue > 5 ? graphInputValue : 20));

	if (usingRandomGraph) {
		graph.generate(80, 80);
	}
	else {
		//test cases are all 20 verticies
		graph.useTestGraph(graphInputValue);
		//compute distances for the graph
		graph.computeDistances();
	}

	//tuning the swarm behavior is finnicky, its like tuning a piano where each
	//string is mislabeled and goes to the wrong place
	//these values seem to work very well for this graph

	//ant decision weights
	float distanceWeight = 2;//2
	float pheramoneWeight = 8;//10

	//pheromone laying and evaporation values
	float pheramoneAmp = 80;//50
	float evaporateValue = 1;//1


	float minDistance = 0;
	float lastDistance = 0;
	
	

	//set up the ant
	Ant ant = Ant(&graph, pheramoneWeight, distanceWeight);
	int bestAnt = 0;


	//The ants go marching
	for (int i = 0; i < antTrials; i++) {

		//move the ant through the entire graph
		for (int x = 0; x < graph.size; x++) {
			ant.move();
		}

		//check if this path distance smaller than the last one and apply a trail
		if (ant.totalDistance < lastDistance) {
			//do a small pheramone placement on the path
			graph.applyPheramones(ant.path, pheramoneAmp / 4);
		}

		//check if this path is a new total minimum distance and give it much more pheromones
		if (i == 0 || ant.totalDistance < minDistance) {
			minDistance = ant.totalDistance;
			graph.applyPheramones(ant.path, pheramoneAmp);
			bestAnt = i;

			//output this path to display the new shortest distance
			if (showShortestPath) {
				cout << "New best path found: " << ant.totalDistance << " by: " << i << '\n';
			}
		}

		//set the "last distance" for the next run
		lastDistance = ant.totalDistance;

		//optionally display every single distance found by every ant
		if (showEveryAntPath) {
			cout << "path found: " << ant.totalDistance << " by: " << i << '\n';
		}

		//evaporate pheromones for all edges, reset ant for next run
		graph.evaporate(evaporateValue);
		ant.reset();
	}

	//print out the shortest distance found
	cout << "\n================================================================================\n";
	cout << "Shortest path found ";
	if (usingRandomGraph) {
		cout << " for random graph size " << graphInputValue;
	}
	else {
		cout << " for test case " << graphInputValue;
	}
	cout<<": " << minDistance << " by trial " << bestAnt << '\n';
	

	return 0;
}