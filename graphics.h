#ifndef GRAPHICS_H_
#define GRAPHICS_H_
#include <SDL/SDL.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include "object.h"

class graphics {
public:
	graphics();
	~graphics();
	void draw(polygon *p);
	void draw(debug_layer *d);
	void draw(vec2 v);
	void draw(vec2 v, vec2 v2);
	void begin_update();
	void end_update();
private:
	SDL_Surface *screen;
};

#endif
