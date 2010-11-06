#include "vector.h"
#include "graphics.h"
#include "object_manager.h"
#include "object.h"
#include <iostream>

int main(int argc, char *argv[]) {
	bool done = false;
	graphics g;
	object_manager om;
	debug_layer d;
	polygon *temp = NULL;
	sf::Event e;
	sf::Clock clock;

	polygon floor;
	floor.position = vec2(320,480);
	floor.points.push_back(vec2(-320,10));
	floor.points.push_back(vec2(320,10));
	floor.points.push_back(vec2(320,-10));
	floor.points.push_back(vec2(-320,-10));
	floor.mass = 1000000000;

	while(!done) {
		clock.Reset();
		om.update();
		g.begin_update();
		om.draw(&g);
		glColor3d(1.0,1.0,1.0);
		g.end_update();
		while(g.wind.GetEvent(e)) {
			switch(e.Type) {
			case sf::Event::MouseButtonReleased:
				if(e.MouseButton.Button == sf::Mouse::Left) {
					if(temp == NULL) {
						std::cout << "adding polygon" << std::endl;
						temp = new polygon();
						temp->hasPhysics = false;
					}
					std::cout << "adding point " << e.MouseMove.X << " " << e.MouseMove.Y << std::endl;
					temp->points.push_back(vec2(g.wind.GetInput().GetMouseX(), g.wind.GetInput().GetMouseY()));
				} else if(e.MouseButton.Button == sf::Mouse::Right) {
					if(temp != NULL) {
						std::cout << "finishing poly" << std::endl;
						for(unsigned int i = 0; i < temp->points.size(); i++) {
							temp->position = temp->position + temp->points[i].scale(1.0/temp->points.size());
						}
						for(unsigned int i = 0; i < temp->points.size(); i++) {
							temp->points[i] = temp->points[i] - temp->position;
						}
						temp->hasPhysics = true;
						temp->acceleration = vec2(0, 0.5);
						temp = NULL;
					}
				} else if(e.MouseButton.Button == sf::Mouse::Middle) {
					om.remove_object_at(e.MouseMove.X, e.MouseMove.Y);
				}
				break;
			case sf::Event::KeyPressed:
				switch(e.Key.Code) {
				case sf::Key::Space:
					break;
				}
				break;
			case sf::Event::Closed:
				done = true;
				break;
			}
		}
		while(clock.GetElapsedTime() < 1.0/60);
	}
	//exit
	return 0;
}
