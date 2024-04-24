/*===============================
* This file contains the activity struct, used for storing activity information and differentiating activities
* and the Functions class which holds all the activity related function declarations used in Main.cpp
===============================*/
#pragma once
#include <string>
#include <vector>
struct activity {
	int id = -1;
	int startTime = -1;
	int endTime = -1;
};

class Functions {
public:
	//vector of activites, they are inserted in earliest start time order while being read in
	std::vector<activity> itemList;

	//return the next activity that does not overlap the current activity a and remove it from the itemList
	activity getNextAvailable(activity a);

	//meant to be used with getline, creates and inserts a new activity into the item list in earliest start time order
	void makeActivity(std::string line);

	//print all items in an activity vector, optionally add tabs for formatting
	void printActivites(std::vector<activity> vect, bool tab = false);
};