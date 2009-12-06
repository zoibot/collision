#include "vector.h"
#include "graphics.h"
#include "object_manager.h"
#include "object.h"
#include <iostream>

extern vec2 collisionpt;

int main(int argc, char *argv[]) {
	bool done = false;
	graphics g;
	object_manager om;
	SDL_Event e;
	polygon *temp = NULL;

	polygon floor;
	floor.position = vec2(320,480);
	floor.points.push_back(vec2(-320,10));
	floor.points.push_back(vec2(320,10));
	floor.points.push_back(vec2(320,-10));
	floor.points.push_back(vec2(-320,-10));
	floor.mass = 1000000000;

	while(!done) {
		om.update();
		g.begin_update();
		om.draw(&g);
		glColor3d(1.0,0.0,0.0);
		g.draw(collisionpt);
		glColor3d(1.0,1.0,1.0);
		g.end_update();
		if(SDL_PollEvent(&e)) {
			switch(e.type) {
		case SDL_MOUSEBUTTONUP:
			if(e.button.button == SDL_BUTTON_LEFT) {
				if(temp == NULL) {
					std::cout << "adding polygon" << std::endl;
					temp = new polygon();
				}
				std::cout << "adding point " << e.motion.x << " " << e.motion.y << std::endl;
				temp->points.push_back(vec2(e.motion.x, e.motion.y));
			} else if(e.button.button == SDL_BUTTON_RIGHT) {
				if(temp != NULL) {
					std::cout << "finishing poly" << std::endl;
					for(int i = 0; i < temp->points.size(); i++) {
						temp->position = temp->position + temp->points[i].scale(1.0/temp->points.size());
					}
					for(int i = 0; i < temp->points.size(); i++) {
						temp->points[i] = temp->points[i] - temp->position;
					}
					temp->acceleration = vec2(0, 0.5);
					temp = NULL;
				}
			} else if(e.button.button == SDL_BUTTON_MIDDLE) {
				om.remove_object_at(e.motion.x, e.motion.y);
			}
			break;
		case SDL_KEYDOWN:
		case SDL_QUIT:
			done = true;
			break;
			}
		}
		//SDL_Delay(100);
	}
	//exit
	return 0;
}
