#include "vector.h"

struct interval {
  double min, max;
  interval combine(interval);
};

struct object {
  vec2 position, velocity, acceleration;
  virtual void draw(); //?
  virtual void update();
  virtual interval project(vec2);
};

struct polygon {
  std::vector<vec2> points;
  interval project(vec2);
};

struct box : public object {
  vec2 size;
  vec2 collides(object);
  interval project(vec2);
};
