/*
		===================
		Alexander Lustig
		CS6420
		Assignment 4 Part 2: Interval scheduling
		Dr. Rague
		Due March 26 2023
		===================
		
		This program takes a text file as an argument
		for this text file each line is a pair of numbers representing the start and end times of an activity

		using the earliest-start greedy choice, the program then returns the minimum number of schedules with their assigned activities that are needed
		to ensure all activities can run without activities overlapping on the same schedule
*/
#include<iostream>
#include<fstream>
#include<string>
#include "Functions.h"
using std::string;
using std::cout;

int main(int argc, char* argv[]) {

	int size = 0;
	string path = "";
	string data = "";
	std::ifstream file;
	Functions f;
	std::vector<std::vector<activity>> scheduleList;

	//stop program if too few arguments
	if (argc < 2) {
		cout << "Not enough arguments in program call, stopping...\n";
		return -1;
	}

	path = argv[1];
	file.open(path);

	//stop program if there is a problem with the file
	if (!file.good()) {
		cout << "Unable to open file: " << path << '\n';
		file.close();
		return -1;
	}

	//read in each line and make into an activity
	while (file.peek() != EOF) {
		std::getline(file, data);
		//ignore any blank lines that get in the file
		if (data != "") {
			f.makeActivity(data);
		}
		
	}

	cout << "Sorted activities\n";
	f.printActivites(f.itemList);

	//while itemList not empty, repeatedly create a new schedule and run getNextAvailable until it recieves an a.id == -1 (none found), repeat
	while (f.itemList.size() > 0) {
		std::vector<activity> items;
		activity a;
		do {
			a = f.getNextAvailable(a);
			if (a.id != -1) {
				items.push_back(a);
			}
		} while (a.id != -1);

		//add to list of activity schedules
		scheduleList.push_back(items);
	}

	//loop through each schedule and print relevent information
	cout << "\nResources needed: " << scheduleList.size() << "\n\n";
	f.printActivites(f.itemList);
	for (int i = 0; i < scheduleList.size(); i++) {
		cout << "Assigned activities for resource: " << i+1 << "\n";
		f.printActivites(scheduleList[i],true);
		std::cout << '\n';
	}

	return 0;
}