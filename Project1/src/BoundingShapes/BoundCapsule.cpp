#include "BoundCapsule.h"



BoundCapsule::BoundCapsule()
{
}


BoundCapsule::~BoundCapsule()
{
}

bool BoundCapsule::collide(glm::vec3 p,glm::vec3 & closetP)
{
	glm::vec3 ap = p - *start;
	glm::vec3 ab = *end - *start;
	float projLen = glm::dot(ap, ab)/glm::length(ab);
	
	if (projLen < 0) projLen = 0;
	if (projLen > 1) projLen = 1;
	 closetP = *start + projLen * ab;
	return glm::length(p-closetP)<=radius;
}
