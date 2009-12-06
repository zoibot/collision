#include "vector.h"
#include <sstream>

vec2::vec2(double x_init, double y_init) {
  x = x_init;
  y = y_init;
}

vec2::vec2() {};

vec2 vec2::scale(double s) {
  vec2 temp(x * s, y * s);
  return temp;
}

vec2 vec2::normalize() {
  return scale(1/magnitude());
}

vec2 vec2::project(vec2 b) {
  return b.scale(dot(b)/b.magsquared());
}

vec2 vec2::rightnorm() {
  vec2 temp(-y,x); //should be -x in a normal coord system, but y is pointed down
  return temp;
}

vec2 vec2::leftnorm() {
  vec2 temp(y,-x);
  return temp;
}

vec2 vec2::rotate(int degrees) {
	double radians = degrees * (M_PI/180.0f);
	vec2 temp(x*cos(radians) - y*sin(radians), x*sin(radians) + y*cos(radians));
	return temp;
}

double vec2::magnitude() {
  return sqrt(magsquared());
}

double vec2::magsquared() {
  return dot(*this);
}

double vec2::dot(vec2 b) {
  return x*b.x + y*b.y;
}

vec2 vec2::operator+(vec2 b) {
  vec2 temp(x + b.x, y+b.y);
  return temp;
}

vec2 vec2::operator-(vec2 b) {
  vec2 temp(x - b.x, y - b.y);
  return temp;
}

vec2 vec2::operator-() {
  vec2 temp(-x,-y);
  return temp;
}

vec2 vec2::operator=(vec2 b) {
  x = b.x;
  y = b.y;
  return *this;
}

bool vec2::operator==(vec2 b) {
	return x == b.x && y == b.y;
}

std::string vec2::text() {
	std::stringstream s;
	s << "(" << x << "," << y << ")";
	return s.str();
}
