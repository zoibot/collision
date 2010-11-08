#ifndef OBJECT_H_
#define OBJECT_H_

#include <vector>
#include <list>
#include <SFML/Graphics.hpp>
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

struct colored_point {
	colored_point(vec2 point, float *color) {
		this->point = point;
		for(int i = 0; i < 3; i++)
			this->color[i] = color[i];
	}
	vec2 point;
	float color[3];
};

struct line {
	line(vec2 st, vec2 end) {
		this->start = st;
		this->end = end;
	}
	vec2 start;
	vec2 end;
};

struct colored_line {
	//colored_line(vec2 start, vec2 end) : start(start), end(end) {}
	colored_line(vec2 start, vec2 end, float *color) : start(start, color), end(end, color) {}
	colored_line(vec2 start, float* color1, vec2 end, float *color2) : start(start, color1), end(end, color2) {}
	colored_point start;
	colored_point end;
};

struct object {
	object();
	//~object();
	double mass, angle, angularvelocity;
	vec2 position, velocity, acceleration;
	virtual std::vector<vec2> vertices() = 0;
	virtual vec2 collide(object*) = 0;
	virtual std::vector<vec2> closestpt(vec2 penetration) = 0;
	virtual bool contains(int,int) = 0;
	double energy();
	void update();
	virtual void draw(graphics*) = 0;
	virtual interval project(vec2) = 0;
	bool hasPhysics, hasGraphics;
	float color[3];
};

struct polygon : public object {
	polygon();
	std::vector<vec2> points;
	interval project(vec2);
	vec2 collide(object*);
	std::vector<vec2> closestpt(vec2 penetration);
	bool contains(int,int);
	void update();
	void draw(graphics*);
	std::vector<vec2> vertices();
};

struct debug_layer : public object {
	debug_layer();
	static std::vector<colored_point> points;
	static std::vector<colored_line> lines;
	static void add_line(colored_point);
	static void add_point(colored_line);
	void draw(graphics*);
	void update();

	vec2 collide(object*);
	std::vector<vec2> closestpt(vec2 penetration);
	std::vector<vec2> vertices();
	bool contains(int,int);
	interval project(vec2);
};

struct text_display : public object {
public:
	text_display(std::string text, int x, int y);
	void update();
	void draw(graphics*);
	void change_text(std::string text);

	vec2 collide(object*);
	std::vector<vec2> closestpt(vec2 penetration);
	std::vector<vec2> vertices();
	bool contains(int,int);
	interval project(vec2);
private:
	sf::String str;
};


#endif
