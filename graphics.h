#ifndef GRAPHICS_H_
#define GRAPHICS_H_
#include <SFML/Graphics.hpp>
#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include "object.h"

class graphics {
public:
	graphics();
	~graphics();
	void draw(vec2 v, float*);
	void draw(vec2 v, vec2 v2, float*);
	void begin_update();
	void end_update();
//private:
	sf::RenderWindow wind;
};

#endif
