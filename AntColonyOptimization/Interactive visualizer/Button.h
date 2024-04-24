/*
	a simple button for the visualizer, checks if the mouse is inside its bounds
*/

#pragma once
#include <string>
class Button {
public:
	int x;
	int y;
	int xSize;
	int ySize;
	std::string name;

	Button(int _x, int _y, int _sx, int _sy, std::string _name);
	bool clicked(int _x, int _y);
};