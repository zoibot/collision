#ifndef OBJECT_H_
#define OBJECT_H_

#include <vector>
#include "vector.h"

class graphics;

/* interval is a simple double precision interval object that supports
 * intersection and union with other intervals.
 */
struct interval {
	double min, max;
	interval intersect(interval);
	interval combine(interval);
};

struct object {
	object();
	//~object();
	double mass, angle, angularvelocity;
	vec2 position, velocity, acceleration;
	virtual std::vector<vec2> vertices() = 0;
	virtual vec2 collide(object*) = 0;
	virtual bool contains(int,int) = 0;
	void update();
	virtual void draw(graphics*) = 0;
	virtual interval project(vec2) = 0;
	bool hasPhysics, hasGraphics;
};

struct polygon : public object {
	polygon();
	std::vector<vec2> points;
	interval project(vec2);
	vec2 collide(object*);
	bool contains(int,int);
	void update();
	void draw(graphics*);
	std::vector<vec2> vertices();
};

struct debug_layer : public object {
	debug_layer();
	static std::vector<vec2> points;
	static std::vector<std::pair<vec2,vec2>> lines;
	void draw(graphics*);
	void update();
};


#endif