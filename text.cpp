#include "object.h"
#include "object_manager.h"
#include <iostream>

text_display::text_display(std::string text, int x, int y) : str(text) {
	str.SetPosition((float)x, (float)y);
	hasPhysics = false;
	hasGraphics = true;
}

void text_display::draw(graphics *g) {
	g->wind.Draw(str);
}

void text_display::update() {
}

void text_display::change_text(std::string text) {
	str.SetText(text);
}

std::vector<vec2> text_display::vertices() {
	std::vector<vec2> res;
	return res;
}

vec2 text_display::collide(object *o) {
	std::cout << "WHAT DON'T GET HERE" << std::endl;
	return vec2(0,0);
}

std::vector<vec2> text_display::closestpt(vec2 test) {
	throw new std::exception;
}

bool text_display::contains(int x, int y) {
	return false;
}

interval text_display::project(vec2 a) {
	return interval();
}

