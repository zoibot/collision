#include <cmath>

struct vec2 {
  double x,y;
  vec2(double,double);
  vec2();
  vec2 scale(double);
  vec2 normailize();
  vec2 project(vec2);
  double magnitude();
  double magsquared();
  double dot(vec2);
  vec2 operator+(vec2);
  vec2 operator-(vec2);
  vec2 operator-();
  vec2 operator=(vec2);
};
