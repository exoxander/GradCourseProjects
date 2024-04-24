/*
	constructor and utility functions
	returns of the given coordinants are inside the bounds of the button, meant to be used on a mouse click
*/
#include "Button.h"

Button::Button(int _x, int _y, int _sx, int _sy, std::string _name) {
	x = _x;
	y = _y;
	xSize = _sx;
	ySize = _sy;
	name = _name;
}

bool Button::clicked(int mouseX, int mouseY) {
	//if inside x range
	if (mouseX > x && mouseX < x + xSize) {
		//if inside y range
		if (mouseY > y && mouseY < y + ySize) {
			return true;
		}
	}

	return false;
}