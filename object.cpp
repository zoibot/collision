#include "object.h"
#include "object_manager.h"
#include <iostream>

object::object() {
	acceleration = vec2(0,0);
	velocity = vec2(0,0);
	angularvelocity = 0;
	angle = 0;
	position = vec2(0,0);
	mass = 10;
	object_manager om;
	om.add_object(this);
}

void object::update() {
	velocity = velocity + acceleration;
	position = position + velocity;
	angle = angle + angularvelocity;
}

interval interval::intersect(interval b) {
	interval res;
	if(min > b.min) {
		res.min = min;
	} else {
		res.min = b.min;
	}
	if(max < b.max) {
		res.max = max;
	} else {
		res.max = b.max;
	}
	return res;
}

polygon::polygon() {
	hasPhysics = true;
	hasGraphics = true;
	color[0] = 1.0;
	color[1] = 1.0;
	color[2] = 1.0;
}

interval polygon::project(vec2 axis) {
  axis = axis.normalize();
  double p;
  interval result;
  std::vector<vec2> verts = vertices();
  result.min = result.max = (verts[0]+position).dot(axis);
  for(unsigned int i = 0; i < verts.size(); i++) {
    p = (verts[i]+position).dot(axis);
    if(p < result.min) result.min = p;
    if(p > result.max) result.max = p;
  }
  return result;
}

vec2 polygon::collide(object* o) {
	interval a, b, intersect;
  	vec2 result(100000,10000000);
  	vec2 temp;
  	std::vector<vec2> verts = vertices();
  for(unsigned int i = 1; i < verts.size()+1; i++) {
    //find normal
	  vec2 side;
	  if(i == verts.size()) {
		  side = verts[0] - verts[i-1];
	  } else {
		side = verts[i] - verts[i-1];
	  }
    vec2 norm = side.rightnorm().normalize();
	a = project(norm);
	b = o->project(norm);
	intersect = a.intersect(b);
	if(intersect.max < intersect.min) {
		//found separating axis, no collision
		result.x = 0;
		result.y = 0;
		break;
	} else {
		if(intersect.max < 0) {
			temp = norm.scale(intersect.min) - norm.scale(intersect.max);
		} else {
			temp = norm.scale(intersect.max) - norm.scale(intersect.min);
		}
		if (temp.magsquared() < result.magsquared())
			result = -temp;
	}
  }
  if (result.magsquared() > 1000) 
	  std::cout << "wtf" << std::endl;
  return result;
}

bool polygon::contains(int x, int y) {
	return false;
}

void polygon::update() {
	object::update();
}

std::vector<vec2> polygon::vertices() {
	std::vector<vec2> verts;
	for(unsigned int i = 0; i < points.size(); i++) {
		verts.push_back(points[i].rotate(angle));
	}
	return verts;
}

void polygon::draw(graphics *g) {
	std::vector<vec2> verts = vertices();
	glPushMatrix();
	glTranslated(position.x, position.y, 0);
	glColor3fv(color);
	glBegin(GL_LINE_LOOP);
	for(unsigned int i = 0; i < verts.size(); i++) {
		glVertex2d(verts[i].x, verts[i].y);
	}
	glEnd();
	glPopMatrix();
}

std::vector<vec2> debug_layer::points;
std::vector<std::pair<vec2, vec2> > debug_layer::lines;

debug_layer::debug_layer() {
	hasPhysics = false;
	hasGraphics = true;
}

void debug_layer::draw(graphics *g) {
	std::cout << "drawing debug crap" << std::endl;
	float red[3] = {1.0, 0.0, 0.0};
	for(int i = 0; i < points.size(); i++) {
		g->draw(points[i], red);
	}
	for(int i = 0; i < lines.size(); i++) {
		g->draw(lines[i].first, lines[i].second, red);
	}
	points.clear();
	lines.clear();
}

std::vector<vec2> debug_layer::vertices() {
	std::vector<vec2> res;
	return res;
}

vec2 debug_layer::collide(object *o) {
	std::cout << "WHAT DON'T GET HERE" << std::endl;
	return vec2(0,0);
}

bool debug_layer::contains(int x, int y) {
	return false;
}

interval debug_layer::project(vec2 a) {
	return interval();
}
