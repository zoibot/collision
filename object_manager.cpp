#include "object_manager.h"
#include <iostream>
vec2 collisionpt(0,0);

std::list<object*> object_manager::objects;

void object_manager::add_object(object* o) {
	objects.push_back(o);
}

void object_manager::remove_object(object* o) {
	objects.(o);
}

void object_manager::remove_object_at(int x, int y) {
	for (unsigned int i = 0; i < objects.size(); i++) {
		if(objects[i]->contains_point(x,y)) {
			remove_object(objects[i]);
			break;
		}
	}
}

void object_manager::update() {
	vec2 collidea, collideb;
	for(unsigned int i = 0; i < objects.size(); i++) {
		objects[i]->update();
	}
	for(unsigned int i = 0; i < objects.size(); i++) {
		//handle collisions
		for(unsigned int j = i; j < objects.size(); j++) {
			bool edgecollision = false;
			if(i == j || objects[i]->vertices().empty() || objects[j]->vertices().empty()) continue;
			collidea = objects[i]->collide(objects[j]);
			collideb = objects[j]->collide(objects[i]);
			if(collidea.magsquared() > collideb.magsquared()) collidea = collideb;
			if(collidea.x != 0 || collidea.y != 0) { //if there is a collision
				objects[i]->position = objects[i]->position - collidea.scale(objects[j]->mass/(objects[i]->mass+objects[j]->mass));
				objects[j]->position = objects[j]->position + collidea.scale(objects[i]->mass/(objects[i]->mass+objects[j]->mass));
				vec2 maxdota = objects[i]->vertices()[0];//find point of collision
				vec2 seconda;
				bool twoa = false;
				for(unsigned int k = 1; k < objects[i]->vertices().size(); k++) {
					if(maxdota.dot(collidea) < objects[i]->vertices()[k].dot(collidea)) {
						if(abs(maxdota.dot(collidea) - objects[i]->vertices()[k].dot(collidea)) < 1) {
							seconda = maxdota;
							twoa = true;
						}
						maxdota = objects[i]->vertices()[k];
					} else if(abs(maxdota.dot(collidea) - objects[i]->vertices()[k].dot(collidea)) < 1) {
						seconda = objects[i]->vertices()[k];
						twoa = true;
					}
				}
				vec2 mindotb = objects[j]->vertices()[0];
				vec2 secondb;
				bool twob = false;
				for(unsigned int k = 1; k < objects[j]->vertices().size(); k++) {
					if(mindotb.dot(collidea) > objects[j]->vertices()[k].dot(collidea)) {
						if(abs(mindotb.dot(collidea) - objects[j]->vertices()[k].dot(collidea)) < 1) {
							secondb = mindotb;
							twob = true;
						}
						mindotb = objects[j]->vertices()[k];
					} else if(abs(mindotb.dot(collidea) - objects[j]->vertices()[k].dot(collidea)) < 1) {
						secondb = objects[j]->vertices()[k];
						twob = true;
					}
				}
				if(!twoa) {
					//if there is only one collision point on obj a (vertex-edge collision)
					collisionpt = maxdota + objects[i]->position;
				} else if(!twob) {
					//or obj b
					collisionpt = mindotb + objects[j]->position;
				} else {
					//edge-edge collision
					//have 4 points, need to discard outer two
					edgecollision = true;
					collisionpt.x = collisionpt.y = 0;
					std::vector<vec2> pts;
					pts.push_back(maxdota+objects[i]->position);
					pts.push_back(seconda+objects[i]->position);
					pts.push_back(mindotb+objects[j]->position);
					pts.push_back(secondb+objects[j]->position);
					vec2 max1,max2;
					double dist;
					double maxdist = 0;
					for(unsigned int i = 0; i < 4; i++) {
						for(unsigned int j = i; j < 4; j++) {
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
				}
				vec2 rap, rbp, vab;
				rap = (collisionpt - objects[i]->position).leftnorm().normalize();
				rbp = (collisionpt - objects[j]->position).leftnorm().normalize();
				vab = objects[i]->velocity - objects[j]->velocity;
				vec2 impulse;
				//coefficient of restitution
				double e = 0.0f;
				//a more realistic model would calculate the rotational intertia, I just use the mass
				double ia, ib;
				ia = objects[i]->mass;
				ib = objects[j]->mass;
				//calculate magnitude and direction of impulse
				impulse = collidea.scale(vab.scale(-(1 + e)).dot(collidea)/(collidea.magsquared()*(1/objects[i]->mass+1/objects[j]->mass)
							+(rap.dot(collidea)*rap.dot(collidea))/ia+(rbp.dot(collidea)*rbp.dot(collidea))/ib));
				//apply impulse to translational and rotational velocities
				objects[i]->velocity = objects[i]->velocity + impulse.scale(1/objects[i]->mass);
				objects[j]->velocity = objects[j]->velocity - impulse.scale(1/objects[j]->mass);
				if(!edgecollision) {
					objects[i]->angularvelocity = objects[i]->angularvelocity - rap.dot(impulse.scale(1/ia));
					objects[j]->angularvelocity = objects[j]->angularvelocity + rbp.dot(impulse.scale(1/ib));
				}
				//friction
				double f = 0.2f;
				//scale back the part of the velocity perpendicular to the normal to simulate friction
				objects[i]->velocity = objects[i]->velocity - collidea.leftnorm().normalize().scale(f * objects[i]->velocity.dot(collidea.leftnorm().normalize()));
				objects[j]->velocity = objects[j]->velocity - collidea.leftnorm().normalize().scale(f * objects[j]->velocity.dot(collidea.leftnorm().normalize()));
				//if(objects[i]->velocity.magsquared() < 5) objects[i]->velocity = vec2(0,0);
				//if(objects[j]->velocity.magsquared() < 5) objects[j]->velocity = vec2(0,0);
				//if(abs(objects[i]->angularvelocity) < .03) objects[i]->angularvelocity = 0;
				//if(abs(objects[j]->angularvelocity) < .03) objects[j]->angularvelocity = 0;
			}
		}
	}
}

void object_manager::draw(graphics *g) {
	for(unsigned int i = 0; i < objects.size(); i++) {
		g->draw(objects[i]);
	}
}
