#pragma once
#include "../../inc/Object.h"
class HairMassPoint 
{
public:
	float mass;
	glm::vec3 bodyForce;
	glm::vec3 position;
	glm::vec3 nextPosition;
	glm::vec3 prePosition;
	HairMassPoint(float mass, glm::vec3 pos) :mass(mass), position(pos) {}
	inline glm::vec3 velocity(float deltaTime) { return (position - prePosition) / deltaTime; }
	HairMassPoint::HairMassPoint()
	{
	}


	HairMassPoint::~HairMassPoint()
	{
	}

};

