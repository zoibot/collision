#ifndef VECTOR_H_
#define VECTOR_H_

#include <cmath>
#include <string>

#ifndef M_PI
#define M_PI 3.141592
#endif

struct vec2 {
  double x,y;
  vec2(double,double);
  vec2();
  vec2 scale(double);
  vec2 normalize();
  vec2 project(vec2);
  double magnitude();
  double magsquared();
  vec2 rightnorm();
  vec2 leftnorm();
  vec2 rotate(int);
  double dot(vec2);
  vec2 operator+(vec2);
  vec2 operator-(vec2);
  vec2 operator-();
  vec2 operator=(vec2);
  bool operator==(vec2);
  std::string text();
};

int testvectors();

#endif