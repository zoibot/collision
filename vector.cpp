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
  return scale(1/magnitude);
}

vec2 vec2::project(vec2 b) {
  return b.scale(dot(b)/b.magsquared());
}

double vec2::magnitude() {
  return sqrt(magsquared);
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

vec2 vec2::operator=(vec2 b) {
  x = b.x;
  y = b.y;
  return *this;
}
