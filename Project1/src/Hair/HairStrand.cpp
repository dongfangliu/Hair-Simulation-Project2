#include "HairStrand.h"



HairStrand::HairStrand()
{
}

HairStrand::HairStrand(float radius, glm::vec3 startPos, int num, float length, glm::vec3 dir) :Object(startPos, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0))
{
	hairRadius = radius;
	massPointNum = num; segmentLength = length; orientation = dir; this->startPos = startPos;
	massPoints = new HairMassPoint[massPointNum];
	colliders = new BoundCapsule[massPointNum - 1];
	for (int i = 0; i < massPointNum; i++) {
		massPoints[i].position = startPos + segmentLength*i*orientation ;
	}
	for (int i = 0; i < massPointNum-1; i++) {
		colliders[i].radius = hairRadius;
		colliders[i].start = &massPoints[i].position;
		colliders[i].end = &massPoints[i+1].position;
	}
}


HairStrand::~HairStrand()
{
	free(massPoints);
	free(colliders);
}

void HairStrand::applyTransform(glm::vec3 translate, glm::vec3 rotate, glm::vec3 scale)
{
	//update the first mass point ,remain others for solver
	glm::vec3 dir = massPoints[0].position - headCenter;
	dir = glm::rotateZ(glm::rotateY(glm::rotateX(dir, rotate.x), rotate.y), rotate.z);
	massPoints[0].nextPosition = headCenter + dir + translate;
	
}

void HairStrand::confirmUpdate()
{
	for (int i = 0; i < massPointNum; i++) {
		massPoints[i].prePosition= massPoints[i].position;
		massPoints[i].position = massPoints[i].nextPosition;
		massPoints[i].nextPosition = glm::vec3();
	}
}
