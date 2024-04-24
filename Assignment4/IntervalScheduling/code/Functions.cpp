#include "Functions.h"
#include <iostream>

void Functions::printActivites(std::vector<activity> vect, bool tab) {
	for (int i = 0; i < vect.size(); i++) {
		if (tab) {
			std::cout << '\t';
		}
		std::cout << "ID:" << vect[i].id << " | " << vect[i].startTime << '-' << vect[i].endTime << '\n';
	}
}

//meant to be used with getline, takes in a string with two integers
//makes an activity and sets the first value to its start, then sets the second value to its end
void Functions::makeActivity(std::string line) {
	activity a;
	int i = 0;
	std::string temp = "";

	//id is used to differentiate activities with identical start and end times
	a.id = itemList.size() + 1;

	//get start time as first number
	while (line[i] != ' ') {
		temp.push_back(line[i]);
		i++;
	}

	//set star time and clear temp
	a.startTime = std::stoi(temp);
	temp = "";

	//get end time as second number
	while (line[i] != '\0') {
		temp.push_back(line[i]);
		i++;
	}

	//set end time
	a.endTime = std::stoi(temp);

	//a now should have ID, start time and end time set

	//loop through itemList and insert before an item with a larger start time
	if (itemList.size() > 0) {
		bool inserted = false;
		for (i = 0; i < itemList.size(); i++) {
			if (itemList[i].startTime > a.startTime) {
				//found the place where next items start time will be greater
				itemList.insert(itemList.begin() + i, a);
				inserted = true;
				break;
			}
		}

		//if whole vector looped through without inserting, put at end
		if (!inserted) {
			itemList.push_back(a);
		}
	}
	else {
		//if vector empty, just insert
		itemList.push_back(a);
	}
}

//returns the next activity in the itemList with a start time >= the input activities end time
activity Functions::getNextAvailable(activity a) {
	activity newActivity;

	//an input with id == -1 means its starting the loop, so just send first item since the list is sorted
	if (a.id == -1) {
		newActivity = itemList[0];
		itemList.erase(itemList.begin());
	}
	else {
		//loop through the list until an activity with startTime >= a.endTime is found
		//if no activity is found, newActivity remains at its default value, the loop calling getNextAvailable() checks for this and ends
		for (int i = 0; i < itemList.size(); i++) {
			if (itemList[i].startTime >= a.endTime) {
				newActivity = itemList[i];
				itemList.erase(itemList.begin() + i);
				break;
			}
		}
	}	
	return newActivity;
}