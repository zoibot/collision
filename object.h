#ifndef OBJECT_H_
#define OBJECT_H_

#include <list>
#include <vector>
#include "vector.h"


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
	void update();
	virtual interval project(vec2) = 0;
	virtual bool contains_point(int,int);
};

struct polygon : public object {
	std::vector<vec2> points;
	interval project(vec2);
	vec2 collide(object*);
	void update();
	std::vector<vec2> vertices();
	bool contains_point(int,int);
};


#endif
