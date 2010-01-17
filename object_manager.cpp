#include "object_manager.h"
#include <iostream>
#include <list>
#include <map>
vec2 collisionpt(0,0);

std::vector<object*> object_manager::objects;

void object_manager::add_object(object* o) {
	objects.push_back(o);
}

void object_manager::remove_object(object* o) {
	//nahthing
}

bool object_manager::remove_object_at(int x, int y) {
	for(unsigned int i = 0; i < objects.size(); i++) {
		if (objects[i]->contains(x,y)) {
			remove_object(objects[i]);
			return true;
		}
	}
	return false;
}

void object_manager::move_objects(vec2 collidea, object *a, object *b) {
	vec2 col = collidea.normalize();
	interval alen = a->project(col);
	interval blen = b->project(col);
	interval newint = alen.intersect(blen);
	if(newint.max < newint.min) {
		return;
	}
	vec2 newoff = col.scale(newint.max) - col.scale(newint.min);
	if(newoff.dot(col) < 0)
		newoff = -newoff;
	a->position = a->position - newoff.scale(b->mass/(a->mass+b->mass));
	b->position = b->position + newoff.scale(a->mass/(a->mass+b->mass));
}

void object_manager::handle_collision(vec2 collidea, object *a, object *b) {
	//this entire function is a fail
	//fix all vertices calls
	//something is wrong here
	int asize = 0;
	vec2 top2a[2];
	bool twoa = false;
	double anglea;
	for(unsigned int k = 0; k < a->vertices().size(); k++) {
		if(asize == 0 || top2a[0].dot(collidea) < a->vertices()[k].dot(collidea)) {
			top2a[1] = top2a[0];
			top2a[0] = a->vertices()[k];
			if(asize == 0) asize = 1;
		} else if (asize == 1 || top2a[1].dot(collidea) < a->vertices()[k].dot(collidea)) {
			top2a[1] = a->vertices()[k];
			if(asize == 1) asize = 2;
		}
	}
	vec2 edge = top2a[0] - top2a[1];
	anglea = acos(edge.dot(collidea)/(collidea.magsquared() * edge.magsquared()));
	if((anglea - M_PI/2) < 0.0005) {
		twoa = true;
	}
	int bsize = 0;
	vec2 top2b[2];
	bool twob = false;
	double angleb;
	for(unsigned int k = 0; k < b->vertices().size(); k++) {
		if(asize == 0 || top2b[0].dot(collidea) > b->vertices()[k].dot(collidea)) {
			top2b[1] = top2b[0];
			top2b[0] = b->vertices()[k];
			if(bsize == 0) bsize = 1;
		} else if (bsize == 1 || top2b[1].dot(collidea) > b->vertices()[k].dot(collidea)) {
			top2b[1] = b->vertices()[k];
			if(bsize == 1) bsize = 2;
		}
	}
	edge = top2b[0] - top2b[1];
	angleb = acos(edge.dot(collidea)/(collidea.magsquared() * edge.magsquared()));
	if((angleb - M_PI/2) < 0.0005) {
		twob = true;
	}
	if(!twoa) {
		//if there is only one collision point on obj a (vertex-edge collision)
		collisionpt = top2a[0] + a->position;
	} else if(!twob) {
		//or obj b
		collisionpt = top2b[0] + b->position;
	} else {
		//edge-edge collision
		//have 4 points, need to discard outer two
		collisionpt.x = collisionpt.y = 0;
		std::vector<vec2> pts;
		pts.push_back(top2a[0]+a->position);
		pts.push_back(top2a[1]+a->position);
		pts.push_back(top2b[0]+b->position);
		pts.push_back(top2b[1]+b->position);
		vec2 max1,max2;
		double dist;
		double maxdist = 0;
		//this is crap
		for(unsigned int i = 0; i < pts.size(); i++) {
			for(unsigned int j = i; j < pts.size(); j++) {
				dist = (pts[i]-pts[j]).magsquared();
				if(dist > maxdist) {
					max1 = pts[i];
					max2 = pts[j];
					maxdist = dist;
				}
			}
		}
		for(unsigned int i = 0; i < 4; i++) {
			if(!(pts[i] == max1 || pts[i] == max2)) {
				collisionpt = collisionpt + pts[i];
			}
		}
		collisionpt = collisionpt.scale(0.5);
		//anglea
	}
	debug_layer::points.push_back(collisionpt);
	//debug_layer::lines.push_back(std::pair<vec2,vec2>(collisionpt - collidea.scale(10), collisionpt + collidea.scale(10)));
	vec2 rap, rbp, vab;
	rap = (collisionpt - a->position).leftnorm();
	rbp = (collisionpt - b->position).leftnorm();
	vab = a->velocity - b->velocity;
	collidea = collidea.normalize();
	vec2 impulse;
	//coefficient of restitution
	double e = 0.0f;
	//a more realistic model would calculate the rotational intertia, I just use the mass
	double ia, ib;
	ia = a->mass * 10;
	ib = b->mass * 10;
	//calculate magnitude and direction of impulse
	//WHAT THIS DOES NOT COUNTERACT ACCELERATION FAIL FAIL FAIL
	impulse = collidea.scale(vab.scale(-(1 + e)).dot(collidea)/(collidea.magsquared()*(1/a->mass+1/b->mass)
				+(rap.dot(collidea)*rap.dot(collidea))/ia+(rbp.dot(collidea)*rbp.dot(collidea))/ib));
	debug_layer::lines.push_back(std::pair<vec2,vec2>(collisionpt - impulse.scale(10), collisionpt + impulse.scale(10)));
	//apply impulse to translational and rotational velocities
	a->velocity = a->velocity + impulse.scale(1/a->mass);
	b->velocity = b->velocity - impulse.scale(1/b->mass);
	//this is backward sometimes?
	a->angularvelocity = a->angularvelocity - rap.dot(impulse.scale(1/ia));
	b->angularvelocity = b->angularvelocity + rbp.dot(impulse.scale(1/ib));
	//friction
	double f = 0.2f;
	//scale back the part of the velocity perpendicular to the normal to simulate friction
	//a->velocity = a->velocity - collidea.leftnorm().normalize().scale(f * a->velocity.dot(collidea.leftnorm().normalize()));
	//b->velocity = b->velocity - collidea.leftnorm().normalize().scale(f * b->velocity.dot(collidea.leftnorm().normalize()));
	//if(a->velocity.magsquared() < 5) a->velocity = vec2(0,0);
	//if(b->velocity.magsquared() < 5) b->velocity = vec2(0,0);
	//if(abs(a->angularvelocity) < .3) a->angularvelocity = 0;
	//if(abs(b->angularvelocity) < .3) b->angularvelocity = 0;
}

void object_manager::update() {
	vec2 collidea, collideb;
	std::list<vec2> collisionpts;

	std::map<std::pair<object*,object*>,vec2> collisions;
	
	for(unsigned int i = 0; i < objects.size(); i++) {
		objects[i]->update();
	}
	for(unsigned int i = 0; i < objects.size(); i++) {
		if(!objects[i]->hasPhysics) continue;
		//handle collisions
		//find all collision vectors
		//needs optimization
		for(unsigned int j = i+1; j < objects.size(); j++) {
			if(!objects[j]->hasPhysics) continue;
			if(objects[i]->vertices().empty() || objects[j]->vertices().empty()) continue;
			collidea = objects[i]->collide(objects[j]);
			collideb = objects[j]->collide(objects[i]);
			if(collidea.magsquared() > collideb.magsquared()) collidea = -collideb;
			if(collidea.x != 0 || collidea.y != 0) { //if there is a collision
				collisions[std::pair<object*,object*>(objects[i], objects[j])] = collidea;
			}
		}
	}
	//Actually handle collisions based on the vectors we found
	for(std::map<std::pair<object*,object*>,vec2>::iterator cur = collisions.begin();
		cur != collisions.end();
		cur++) {
			handle_collision((*cur).second, (*cur).first.first, (*cur).first.second);
	}
	for(unsigned int z = 0; z < 10; z++) {
		for(std::map<std::pair<object*,object*>,vec2>::iterator cur = collisions.begin();
			cur != collisions.end();
			cur++) {
				move_objects((*cur).second, (*cur).first.first, (*cur).first.second);
		}
	}
}

void object_manager::draw(graphics *g) {
	for(unsigned int i = 0; i < objects.size(); i++) {
		objects[i]->draw(g);
	}
}
