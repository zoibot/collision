#include "graphics.h"

graphics::graphics() : wind(sf::RenderWindow(sf::VideoMode(640,480), "collide!!!")) {
	//SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	//screen = SDL_SetVideoMode(640,480,32,SDL_OPENGL);
	glPointSize(5.0);
	glViewport(0,0,640,480);
}

graphics::~graphics() {
	//SDL_Quit();
}


void graphics::draw(vec2 v, float* color) {
	glColor3fv(color);
	glBegin(GL_POINTS);
	glVertex2d(v.x, v.y);
	glEnd();
}

void graphics::draw(vec2 v, vec2 v2, float* color) {
	glColor3fv(color);
	glBegin(GL_LINES);
	glVertex2d(v.x, v.y);
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
