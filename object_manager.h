#ifndef OBJECT_MANAGER_H_
#define OBJECT_MANAGER_H_
#include "graphics.h"
#include "object.h"

class object_manager {
private:
	static std::list<object*> objects;
public:
	void add_object(object*);
	void remove_object(object*);
	void remove_object_at(int,int);
	void update();
	void draw(graphics*);
};

#endif /*OBJECT_MANAGER_H_*/
