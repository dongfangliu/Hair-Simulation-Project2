#pragma once
#include "../../inc/common.h"
class BoundCapsule
{
public:
	float radius;
	glm::vec3 * start;
	glm::vec3 * end;
	BoundCapsule();
	~BoundCapsule();
	BoundCapsule(double radius,
		glm::vec3 * start,
		glm::vec3 * end) :radius(radius), start(start), end(end) {}
	bool collide(glm::vec3 p, glm::vec3 & closetP);
};

