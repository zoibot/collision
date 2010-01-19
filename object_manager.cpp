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
	//find closest points to collision on both sides (1 or 2 points from each object)
	//this should absolutely be methods of the object, since it will be totally different for circle etc
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
	debug_layer::lines.push_back(std::make_pair(top2a[0], top2a[1]));
	vec2 edge = top2a[0] - top2a[1];
	anglea = acos(edge.dot(collidea)/(collidea.magnitude() * edge.magnitude()));
	if((anglea - M_PI/2) < 0.005) {
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
	angleb = acos(edge.dot(collidea)/(collidea.magnitude() * edge.magnitude()));
	if((angleb - M_PI/2) < 0.005) {
		twob = true;
	}
	//use points from objects to find the real collision point or points
	if(!twoa || !twob) {
		if(!twoa) {
			//if there is only one collision point on obj a (vertex-edge collision)
			collisionpt = top2a[0] + a->position;
		} else if(!twob) {
			//or obj b
			collisionpt = top2b[0] + b->position;
		}
		debug_layer::points.push_back(collisionpt);
		//debug_layer::lines.push_back(std::pair<vec2,vec2>(collisionpt - collidea.scale(10), collisionpt + collidea.scale(10)));
		vec2 rap, rbp, vab;
		rap = (collisionpt - a->position).leftnorm();
		rbp = (collisionpt - b->position).leftnorm();
		vab = a->velocity + rap.rightnorm().normalize().scale(a->angularvelocity) - b->velocity - rbp.rightnorm().normalize().scale(b->angularvelocity);
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

	} else {
		//edge-edge collision
		//have 4 points, need to discard outer two
		collisionpt.x = collisionpt.y = 0;
		std::vector<vec2> pts;
		pts.push_back(top2a[0]+a->position);
		pts.push_back(top2a[1]+a->position);
		pts.push_back(top2b[0]+b->position);
		pts.push_back(top2b[1]+b->position);
		vec2 min1,min2;
		double dist;
		double mindist = 10000000000;
		//this is crap
		for(unsigned int i = 0; i < pts.size(); i++) {
			for(unsigned int j = i; j < pts.size(); j++) {
				dist = (pts[i]-pts[j]).magsquared();
				if(dist < mindist) {
					min1 = pts[i];
					min2 = pts[j];
					mindist = dist;
				}
			}
		}
		debug_layer::points.push_back(min1);
		debug_layer::points.push_back(min2);
		//debug_layer::lines.push_back(std::pair<vec2,vec2>(collisionpt - collidea.scale(10), collisionpt + collidea.scale(10)));
		vec2 rap1, rbp1;
		rap1 = (min1 - a->position).leftnorm();
		rbp1 = (min1 - b->position).leftnorm();
		vec2 rap2, rbp2;
		rap2 = (min2 - a->position).leftnorm();
		rbp2 = (min2 - b->position).leftnorm();
		collidea = collidea.normalize();
		//calculate relative velocity
		vec2 vab1, vab2;
		vab1 = a->velocity + rap1.rightnorm().normalize().scale(a->angularvelocity) - b->velocity - rbp1.rightnorm().normalize().scale(b->angularvelocity);
		vab2 = a->velocity + rap2.rightnorm().normalize().scale(a->angularvelocity) - b->velocity - rbp2.rightnorm().normalize().scale(b->angularvelocity);
		vec2 impulse1, impulse2;
		//coefficient of restitution
		double e = 0.0f;
		//a more realistic model would calculate the rotational intertia, I just use the mass
		double ia, ib;
		ia = a->mass * 10;
		ib = b->mass * 10;
		//calculate magnitude and direction of impulse
		//WHAT THIS DOES NOT COUNTERACT ACCELERATION FAIL FAIL FAIL
		impulse1 = collidea.scale(vab1.scale(-(1 + e)).dot(collidea)/(collidea.magsquared()*(1/a->mass+1/b->mass)
					+(rap1.dot(collidea)*rap1.dot(collidea))/ia+(rbp1.dot(collidea)*rbp1.dot(collidea))/ib));
		impulse2 = collidea.scale(vab2.scale(-(1 + e)).dot(collidea)/(collidea.magsquared()*(1/a->mass+1/b->mass)
					+(rap2.dot(collidea)*rap2.dot(collidea))/ia+(rbp2.dot(collidea)*rbp2.dot(collidea))/ib));
		//debug_layer::lines.push_back(std::pair<vec2,vec2>(collisionpt - impulse.scale(10), collisionpt + impulse.scale(10)));
		//apply impulse to translational and rotational velocities
		a->velocity = a->velocity + (impulse1 + impulse2).scale(1/a->mass);
		b->velocity = b->velocity - (impulse1 + impulse2).scale(1/b->mass);
		//this is backward sometimes?
		//need to weight based on stuff
		a->angularvelocity = a->angularvelocity - (rap2.magnitude() * rap1.dot(impulse1.scale(1/ia)) + rap1.magnitude() * rap2.dot(impulse2.scale(1/ia)))/(rap1.magnitude()+rap2.magnitude());
		b->angularvelocity = b->angularvelocity + (rbp2.magnitude() * rbp1.dot(impulse1.scale(1/ib)) + rbp1.magnitude() * rbp2.dot(impulse2.scale(1/ib)))/(rbp1.magnitude()+rbp2.magnitude());
		//friction
		double f = 0.2f;
		//scale back the part of the velocity perpendicular to the normal to simulate friction
		//a->velocity = a->velocity - collidea.leftnorm().normalize().scale(f * a->velocity.dot(collidea.leftnorm().normalize()));
		//b->velocity = b->velocity - collidea.leftnorm().normalize().scale(f * b->velocity.dot(collidea.leftnorm().normalize()));
	}
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
	for(unsigned int z = 0; z < 10; z++) {
		for(std::map<std::pair<object*,object*>,vec2>::iterator cur = collisions.begin();
			cur != collisions.end();
			cur++) {
				move_objects((*cur).second, (*cur).first.first, (*cur).first.second);
		}
	}
	//Actually handle collisions based on the vectors we found
	for(std::map<std::pair<object*,object*>,vec2>::iterator cur = collisions.begin();
		cur != collisions.end();
		cur++) {
			handle_collision((*cur).second, (*cur).first.first, (*cur).first.second);
	}
}

void object_manager::draw(graphics *g) {
	for(unsigned int i = 0; i < objects.size(); i++) {
		objects[i]->draw(g);
	}
}
