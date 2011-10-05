#include "graphics.h"

graphics::graphics() : wind(sf::VideoMode(640,480), "Physics Demo", sf::Style::Titlebar) {
	glPointSize(5.0);
	glViewport(0,0,640,480);
	wind.PreserveOpenGLStates(true);
}

graphics::~graphics() {
}


void graphics::draw(vec2 v, float* color) {
	glColor3fv(color);
	glBegin(GL_POINTS);
	glVertex2d(v.x, v.y);
	glEnd();
}

void graphics::draw(vec2 v, float* color1, vec2 v2, float* color2) {
	glBegin(GL_LINES);
	glColor3fv(color1);
	glVertex2d(v.x, v.y);
	glColor3fv(color2);
	glVertex2d(v2.x, v2.y);
	glEnd();
}

void graphics::begin_update() {
	glClear(GL_COLOR_BUFFER_BIT);
	wind.SetActive();
	glLoadIdentity();
	glOrtho(0, 640, 480, 0, -1, 1);
}

void graphics::end_update() {
	glFlush();
	wind.Display();
}


float *color(float r, float g, float b) {
	float *temp = new float[3];
	temp[0] = r; temp[1] = g; temp[2] = b;
	return temp;
}
