#include "graphics.h"

graphics::graphics() {
	SDL_Init(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	screen = SDL_SetVideoMode(800,600,32,SDL_OPENGL);
	glPointSize(5.0);
}

graphics::~graphics() {
	SDL_FreeSurface(screen);
	SDL_Quit();
}

void graphics::draw(object *o) {
	std::vector<vec2> verts = o->vertices();
	glPushMatrix();
	glTranslated(o->position.x, o->position.y, 0);
	glBegin(GL_LINE_LOOP);
	for(unsigned int i = 0; i < verts.size(); i++) {
		glVertex2d(verts[i].x, verts[i].y);
	}
	glEnd();
	glPopMatrix();
}

void graphics::draw(vec2 v) {
	glBegin(GL_POINTS);
	glVertex2d(v.x, v.y);
	glEnd();
}

void graphics::draw(vec2 v, vec2 v2) {
	glBegin(GL_LINES);
	glVertex2d(v.x, v.y);
	glVertex2d(v2.x, v2.y);
	glEnd();
}

void graphics::begin_update() {
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	glOrtho(0, 640, 480, 0, -1, 1);
}

void graphics::end_update() {
	glFlush();
	SDL_GL_SwapBuffers();
}
