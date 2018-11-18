#pragma once

#include "HairStrand.h"

#define  Gravity -10
class HairMSSovler //Did not solve hair-hair contacts
{
public:
	HairMSSovler();
	~HairMSSovler();
	HairMSSovler(float stiffness, float damping, BoundCapsule * headCollide, glm::vec3 gravity) :stiffness(stiffness), damping(damping), headCollider(headCollide),gravity(gravity) {};
	inline void addHair(HairStrand * strand) { strands.push_back(strand); }
	glm::vec3 gravity;
	void Sovle(float timeStep);
private:
	float maxExtendPercent = 0.1;
	float stiffness = 2;
	float damping = 10;
	vector<HairStrand *> strands;
	BoundCapsule * headCollider;
	
};



HairMSSovler::HairMSSovler()
{
}


HairMSSovler::~HairMSSovler()
{
}

inline void HairMSSovler::Sovle(float timeStep)
{
	//SettingUp BodyForce
	for (int i = 0; i < strands.size(); i++) {
		HairStrand * strand = strands[i];
		glm::vec3 closetP;
		for (int i = 1; i < strand->massPointNum; i++) {
			//Set up gravity
			strand->massPoints[i].bodyForce = strand->massPoints[i].mass*gravity ;
			//Deal with collision
			if (headCollider->collide(strand->massPoints[i].position, closetP)) {
				glm::vec3 forceDir = glm::normalize(strand->massPoints[i].position - closetP);
				strand->massPoints[i].bodyForce+= forceDir * stiffness*(headCollider->radius - glm::length(strand->massPoints[i].position - closetP));
			}
		}
	}


	//solve each hair
	for (int i = 0; i < strands.size(); i++) {
		
		HairStrand * strand = strands[i];
		for (int i = 1; i < strand->massPointNum; i++) {
			HairMassPoint * prePoint = &strand->massPoints[i - 1];
			HairMassPoint * point = &strand->massPoints[i ];
			HairMassPoint * nextPoint = &strand->massPoints[i+ 1];

			glm::vec3 force = stiffness * glm::length(prePoint->nextPosition - point->position) - strand->segmentLength*glm::normalize(prePoint->nextPosition - point->position)
				+ stiffness * glm::length(nextPoint->position - point->position) - strand->segmentLength*glm::normalize(nextPoint->position - point->position)
				+point->velocity(timeStep)*(-damping)
				+ point->bodyForce;
			glm::vec3 velocity = force / point->mass*timeStep;
			point->nextPosition = point->position + velocity*timeStep;
			if (abs(glm::length(point->nextPosition - prePoint->nextPosition) - strand->segmentLength) > strand->segmentLength*maxExtendPercent) {
				float t = (strand->segmentLength*strand->segmentLength - glm::length2(prePoint->nextPosition - point->position) - glm::length2(velocity)) / (2 * glm::dot(prePoint->nextPosition - point->position, velocity));
				point->nextPosition = point->position + velocity * t;
			}
		}
		strand->confirmUpdate();
	}

}
