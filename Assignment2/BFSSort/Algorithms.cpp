/*===================================
Defines the functions "TOPOSORT" and "UNIQUETOPO"
TOPOSORT: uses a bredth first search to compute the in-degree of all vertecies
		then iterates over the lowest in-degree in the list, decriments all vertecies it connects to
		and finally pushes that vertecies ID into the topoList which is returned

UNIQUETOPO: checks that each item in the input topoList is linked to by the previous item
		if the current item is not linked to by the previous, the sort is not unique and it returns false
		otherwise it continues through the entire list, returns true if all items pass the test
===================================*/
#include "Algorithms.h"
#include <iostream>


/*=========================================================================
TOPOSORT
=========================================================================*/
list<int> Algorithms::TOPOSORT(Digraph input, int size) {
	list<vertex> adjList = list<vertex>();
	list<vertex> forest;//starts as copy of adjList but becomes a breadth-search forest
	list<vertex> checkList = list<vertex>();
	queue<int> greyQueue = queue<int>();
	list<vertex> zeroList = list<vertex>();
	list<int> topoList;

	//load input into adjList
	for (int vert = 0; vert < size; vert++) {
		vertex newVertex;
		newVertex.id = vert;
		newVertex.parent = -1;
		newVertex.inDegree = 0;
		newVertex.state = 0;
		newVertex.edges = input.adj(vert);
		adjList.push_back(newVertex);
	}

	forest = adjList;
	
	//loop through whole thing in case several trees are produced
	for (list<vertex>::iterator vert = forest.begin(); vert != forest.end(); ++vert) {

		//vertex not discovered, new tree starts
		if (vert->state == 0) {
			list<vertex>::iterator currentVertex = vert;

			//seeding BFS with the root
			currentVertex->state = 1;//grey
			greyQueue.push(currentVertex->id);
			while (greyQueue.size() > 0) {

				//increse indegree of all children, put all white (state == 0) childen into grey list and set parent
				//loop through all children in currentVertex.edges
				for (list<int>::iterator child = currentVertex->edges.begin(); child != currentVertex->edges.end();) {

					//loop through forest to find currentVertex with same ID as *child
					for (list<vertex>::iterator finder = forest.begin(); finder != forest.end(); ++finder) {

						//when child is found inside forest, increase degree
						if (finder->id == *child) {
							finder->inDegree++;
							if (finder->state == 0) {//if child has state 0 (white)
								finder->state = 1;
								finder->parent = currentVertex->id;
								greyQueue.push(finder->id);//add to greyQueue
								++child;//increment child if entry not deleted
							}
							//if child already found or complete, remove child from currentVertex edge list to produce forest
							else if(finder->parent != currentVertex->id) {
								child = currentVertex->edges.erase(child);
							}

							if (child == currentVertex->edges.end()) {
								//for loop throws an error if child iterator reaches edges.end()
								//since its already at the end of the edge list, this breaks out of both loops preventing the issue
								goto breakOnEnd;
							}
							break;
						}
					}
				}
breakOnEnd:
				//all children looped through, set currentVertex state to black and pop from greyQeue
				currentVertex->state = 2;
				greyQueue.pop();
				//move to next vert in greyQueue
				for (list<vertex>::iterator it = forest.begin(); it != forest.end(); ++it) {
					if (greyQueue.size() > 0 && it->id == greyQueue.front()) {
						currentVertex = it;
					}
				}

			}
		}
	}
	//======================================================================================================
	//whole adjacency list looped through, forest now represents a forest with at least 1 tree from the search
	//now starting topological sort using in-degree and forest computed in the breadth first search
	//======================================================================================================
	
	while (forest.size() > 0) {

		//loop through forest to find every vertex with in-degree zero, and put into zeroList
		for (list<vertex>::iterator vert = forest.begin(); vert != forest.end(); ++vert) {
			if (vert->inDegree == 0) {
				zeroList.push_back(*vert);
				//std::cout << vert->id << "zero-queued" << "\n";
			}
		}
		//for all items in zeroList
		while (zeroList.size() > 0) {
			
			//loop through adjList to find the corrisponding vertex (with intact edges)
			list<vertex>::iterator it = adjList.begin();
			for (list<vertex>::iterator vert = adjList.begin(); vert != adjList.end(); ++vert) {
				if (vert->id == zeroList.front().id) {
					it = vert;
					break;
				}
			}
			//loop through edge list
			for (list<int>::iterator child = it->edges.begin(); child != it->edges.end(); ++child) {

				//loop through forest to and decriment all edges from vert in the adjList
				for (list<vertex>::iterator finder = forest.begin(); finder != forest.end(); ++finder) {
					if (finder->id == *child) {
						finder->inDegree--;
						break;
					}
				}
			}
			
			//all children looped through, add vert->id to topoList
			topoList.push_back(it->id);
			//std::cout << "pushing: " << zeroList.front().id << "\n";

			//remove zeroList.front() from the forest
			for (list<vertex>::iterator vert = forest.begin(); vert != forest.end(); ++vert) {
				if (zeroList.size() > 0 && vert->id == zeroList.front().id) {
					forest.erase(vert);
					break;
				}
			}
			//erase zeroList.front()
			zeroList.erase(zeroList.begin());
		}
	}
	//return topological sort
	return topoList;
}

/*=========================================================================
UNIQUETOPO
=========================================================================*/
bool Algorithms::UNIQUETOPO(Digraph input, list<int> topoList, int size) {
	//list<int> topoList = Algorithms::TOPOSORT(input, size);
	// run through topoList list and determine if each item points directly to the next
	// if chain is broken, not unique

	list<vertex> adjList = list<vertex>();
	//load input into adjList
	for (int vert = 0; vert < size; vert++) {
		vertex newVertex;
		newVertex.id = vert;
		newVertex.parent = -1;
		newVertex.inDegree = 0;
		newVertex.state = 0;
		newVertex.edges = input.adj(vert);
		adjList.push_back(newVertex);
	}

	//check each item in the topological list against the adjacency list
	//continue until topolist[i] has no connections to topolist[i+1], then return false
	list<int>::iterator current = topoList.begin();
	list<int>::iterator last = current;
	bool foundEdge = false;
	++current;
	for (; current != topoList.end(); ++current) {
		//if last->edgelist contains current->id, continue
		//else, return false
		//find last->edge list
		for (list<vertex>::iterator vert = adjList.begin(); vert != adjList.end(); ++vert) {
			if (vert->id == *last) {
				//check the list for current
				for (list<int>::iterator edge = vert->edges.begin(); edge != vert->edges.end(); ++edge) {
					if (*edge == *current) {
						foundEdge = true;
						break;
					}
				}
				if (foundEdge) {
					break;
				}
				else {
					return false;
				}
			}
		}
		last = current;
	}
	
	return true;
}



