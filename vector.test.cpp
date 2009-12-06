#include "vector.h"
#include <iostream>

int testvectors() {
	vec2 a(1,2);
	vec2 b(2,4);
	std::cout << a.dot(b) << std::endl;
	a = b;
	std::cout << a.scale(0.5).text() << std::endl;
	return 0;
}
