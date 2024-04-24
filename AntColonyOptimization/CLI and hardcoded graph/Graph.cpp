/*
	The functions to make the graph work correctly

	evaporate(): reduces all pheromone values on the graph by the specified amount

	computeDistance(): pre-computes the distances for all edges in the graph for ants to access during trials

	generate(): creates [graph.size] number of random verticies around the center of the input value range,
	meant to be used with the OLC pixel game engine to visually demo the algorithm

	reset(): resets all edge pheromone values so the graph can be used in a new set of trials

	applyPheromones(): applys the input pheromone values to every edge along the input path

	useTestGraph(): holds a bunch of hard coded test cases for running the program
*/

#include "Graph.h"

//constructor
Graph::Graph(int _size) {
	size = _size;
	vertecies = new vertex[size];
	edges = new edge[size * size];
}


//reduce all the graphs pheramone values by the input
//minimum of 1 to avoid divide by zero issues
void Graph::evaporate(float strength) {
	for (int i = 0; i < size * size; i++) {

		//reduce by the strength of the pheramones by the given factor for each edge
		if (edges[i].pheramoneStrength > 1) {
			edges[i].pheramoneStrength -= strength;
		}
		if (edges[i].pheramoneStrength < 1) {
			edges[i].pheramoneStrength = 1;
		}
	}
}


//pre-compute the distances and store them so the ant doesnt need to
//do this itself for each trial
void Graph::computeDistances() {

	//rows represent individual vertecies
	for (int row = 0; row < size; row++) {
		//a column represents all distances from the current vertex
		for (int col = 0; col < size; col++) {
			if (row == col) {
				//distance to itself is -1
				edges[size * row + col].distance = -1;
			}
			else {
				float tx = (vertecies[col].x - vertecies[row].x);
				tx = tx > 0 ? tx : tx * -1;
				float ty = (vertecies[col].y - vertecies[row].y);
				ty = ty > 0 ? ty : ty * -1;

				edges[size * row + col].distance = sqrt(tx + ty);
			}
		}
	}
}

//generate random vertecies near the middle of the input range, for the graph size
//the range is meant to be the size of the screen for the visual demo
void Graph::generate(int _maxX, int _maxY) {
	for (int i = 0; i < size; i++) {
		int x = (rand() % _maxX / 2) + _maxX / 4;
		int y = (rand() % _maxY / 2) + _maxY / 4;
		vertecies[i] = vertex(x, y);
	}

	//build the distance matrix
	computeDistances();
}

//reset all pheramone strengths for a new run
void Graph::reset() {
	for (int i = 0; i < size * size; i++) {
		edges[i].pheramoneStrength = 100;
	}
}

//increase the pheramone levels for each edge in a path by the specified amount
void Graph::applyPheramones(std::vector<int> _path, float strength) {
	//add pheramones to each edge in direction of travel
	for (int i = 1; i < _path.size(); i++) {
		edges[size * _path[i - 1] + _path[i]].pheramoneStrength += strength;
	}
}



//a bunch of hard-coded graphs to use as test cases

void Graph::useTestGraph(int _graph) {
	switch (_graph) {
	case 1:
		//t1, default test case, this graph was used to tune the colony
		//best recorded distance: 68.565 | 1000 ants, ~1 second
		vertecies[0] = vertex(11, 33);
		vertecies[1] = vertex(-18, 7);
		vertecies[2] = vertex(14, -9);
		vertecies[3] = vertex(1, -6);
		vertecies[4] = vertex(-7, -4);
		vertecies[5] = vertex(3, -4);
		vertecies[6] = vertex(8, -7);
		vertecies[7] = vertex(-5, 13);
		vertecies[8] = vertex(7, 21);
		vertecies[9] = vertex(12, -15);
		vertecies[10] = vertex(6, -3);
		vertecies[11] = vertex(34, -1);
		vertecies[12] = vertex(6, 14);
		vertecies[13] = vertex(11, 11);
		vertecies[14] = vertex(-26, -7);
		vertecies[15] = vertex(4, -17);
		vertecies[16] = vertex(6, 3);
		vertecies[17] = vertex(9, 9);
		vertecies[18] = vertex(7, -4);
		vertecies[19] = vertex(-8, -16);
		break;

	case 2:
		//t2
		vertecies[0] = vertex(-8, 5);
		vertecies[1] = vertex(-5, -1);
		vertecies[2] = vertex(6, -9);
		vertecies[3] = vertex(1, -17);
		vertecies[4] = vertex(-20, -5);
		vertecies[5] = vertex(-3, -2);
		vertecies[6] = vertex(2, -12);
		vertecies[7] = vertex(-19, 8);
		vertecies[8] = vertex(8, 23);
		vertecies[9] = vertex(-12, 15);
		vertecies[10] = vertex(9, 1);
		vertecies[11] = vertex(22, -14);
		vertecies[12] = vertex(-6, 7);
		vertecies[13] = vertex(12, -11);
		vertecies[14] = vertex(-7, -12);
		vertecies[15] = vertex(4, -6);
		vertecies[16] = vertex(-6, 3);
		vertecies[17] = vertex(-9, -9);
		vertecies[18] = vertex(12, -4);
		vertecies[19] = vertex(-8, -1);
		break;

	case 3:
		//t3
		vertecies[0] = vertex(6, 5);
		vertecies[1] = vertex(11, -1);
		vertecies[2] = vertex(6, 4);
		vertecies[3] = vertex(1, -8);
		vertecies[4] = vertex(8, -5);
		vertecies[5] = vertex(-3, -17);
		vertecies[6] = vertex(-6, -12);
		vertecies[7] = vertex(-19, 20);
		vertecies[8] = vertex(8, 10);
		vertecies[9] = vertex(2, 5);
		vertecies[10] = vertex(-9, 1);
		vertecies[11] = vertex(11, -3);
		vertecies[12] = vertex(-16, 5);
		vertecies[13] = vertex(6, -5);
		vertecies[14] = vertex(7, -19);
		vertecies[15] = vertex(-4, -16);
		vertecies[16] = vertex(-8, 3);
		vertecies[17] = vertex(-9, -19);
		vertecies[18] = vertex(13, -4);
		vertecies[19] = vertex(-8, -11);
		break;

	case 4:
		//t4
		vertecies[0] = vertex(6, -10);
		vertecies[1] = vertex(11, -1);
		vertecies[2] = vertex(16, 14);
		vertecies[3] = vertex(11, -18);
		vertecies[4] = vertex(18, -15);
		vertecies[5] = vertex(-13, -7);
		vertecies[6] = vertex(-16, -2);
		vertecies[7] = vertex(-9, 2);
		vertecies[8] = vertex(18, 1);
		vertecies[9] = vertex(12, 5);
		vertecies[10] = vertex(-9, 11);
		vertecies[11] = vertex(1, -13);
		vertecies[12] = vertex(-8, 9);
		vertecies[13] = vertex(6, -15);
		vertecies[14] = vertex(17, -15);
		vertecies[15] = vertex(-4, -16);
		vertecies[16] = vertex(-8, 23);
		vertecies[17] = vertex(-9, -29);
		vertecies[18] = vertex(13, -24);
		vertecies[19] = vertex(-28, -11);
		break;

	case 5:
		//t5
		vertecies[0] = vertex(26, -1);
		vertecies[1] = vertex(31, -21);
		vertecies[2] = vertex(26, 24);
		vertecies[3] = vertex(9, -1);
		vertecies[4] = vertex(-9, 7);
		vertecies[5] = vertex(13, 7);
		vertecies[6] = vertex(-16, 20);
		vertecies[7] = vertex(29, 12);
		vertecies[8] = vertex(-18, 11);
		vertecies[9] = vertex(-6, 5);
		vertecies[10] = vertex(9, 14);
		vertecies[11] = vertex(21, 23);
		vertecies[12] = vertex(-18, 9);
		vertecies[13] = vertex(6, -15);
		vertecies[14] = vertex(1, -5);
		vertecies[15] = vertex(-14, -6);
		vertecies[16] = vertex(8, -7);
		vertecies[17] = vertex(-9, 9);
		vertecies[18] = vertex(-7, -24);
		vertecies[19] = vertex(18, -11);
		break;
	}
}