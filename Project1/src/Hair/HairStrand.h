#pragma once
#include "HairMassPoint.h"
#include "../../inc/Object.h"
#include "../BoundingShapes/BoundCapsule.h"


class HairStrand:Object
{
public:
	HairStrand();
	HairStrand(float radius,glm::vec3 startPos,int num, float length, glm::vec3 dir = glm::vec3(0, 0, 1));
	~HairStrand();
	void applyTransform(glm::vec3 translate, glm::vec3 rotate, glm::vec3 scale);
	void confirmUpdate();

	int massPointNum;

	HairMassPoint * massPoints;
	BoundCapsule * colliders;
	float segmentLength;
private:
	float hairRadius=0.1;
	glm::vec3 orientation = glm::vec3(0,0,1);
	glm::vec3 startPos;
	glm::vec3 headCenter;
};

 