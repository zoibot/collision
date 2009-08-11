interval polygon::project(vec2 axis) {
  axis = axis.normalize();
  double p;
  interval result;
  result.min = result.max = points[0].dot(axis);
  for(int i = 0; i < points.size; i++) {
    p = points[i].dot(axis);
    if(p < result.min) result.min = p;
    if(p > result.max) result.max = p;
  }
  return result;
}

vec2 polygon::collide(object o) {
  for(int i = 1; i < points.size; i++) {
    //find normal
    vec2 side = points[i] - points[i-1];
    vec2 norm = side.rightortho();
  }
}

void box::update() {
  x = getCollisionList();
  for(x) {
    if(vector = collision) {
      react;
    }
  }
}

vec2 box::collides(object o) {
  vec2 axis1(1,0);
  vec2 axis2(0,1);
  
}

interval box::project(vec2 a) {
  vec2 side1(size.x,0);
  vec2 side2(0,size.y);
  vec2 origin(position.x-(size.x/2.0f), position.y-(size.y/2.0f));
  origin = origin.project(a); //make two segments and combine
  side1 = side1.project(a);
  side2 = side2.project(a);
  return side1.magnitude() + side2.magnitude();
}

void box::draw();
