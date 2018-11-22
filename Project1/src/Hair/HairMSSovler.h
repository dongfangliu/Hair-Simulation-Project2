#pragma once

#include "HairStrand.h"
#include "../../inc/BoundingShapes/BoundCapsule.h"
class HairMSSovler //Did not solve hair-hair contacts
{
public:
	HairMSSovler();
	~HairMSSovler();
	HairMSSovler(vector<HairStrand *> * hairs,BoundCapsule * headCollide, glm::vec3 gravity, float stiffness = 10, float damping =1) :
		strands(hairs),stiffness(stiffness), damping(damping), headCollider(headCollide),gravity(gravity) {
		for (int j = 0; j < strands->size(); j++) {
			HairStrand * strand = (*strands)[j];
			for (int i = 1; i < strand->massPointNum; i++) {
				strand->bodyForce[i] = strand->mass*gravity;
			}
		}
	
	};
	inline void addHair(HairStrand * strand) { strands->push_back(strand); }
	glm::vec3 gravity;
	void Sovle(float timeStep);
private:
	float maxExtendPercent = 0.1;
	float stiffness;
	float damping;

	vector<HairStrand *> * strands;
	BoundCapsule * headCollider;
	float epsilonPercent = 0.01;// headCollide
	float collideStiffness = 200;
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
	//for (int j = 0; j < strands->size(); j++) {
	//	HairStrand * strand = (*strands)[j];
	//	for (int i = 1; i < strand->massPointNum; i++) {
	//		//Set up gravity
	//		strand->bodyForce[i] = strand->mass*gravity ;
	//	}
	//}

	//solve each hair
	for (int j = 0; j < strands->size(); j++) {
		HairStrand * strand = (*strands)[j];
		for (int i = 1; i < strand->massPointNum; i++) {
			float extendRatio = (glm::length(strand->position[i] - strand->position[i - 1]) - strand->restLength) / strand->restLength;
			if (abs(extendRatio) > maxExtendPercent) {
				float length;
				if (extendRatio > 0) {
					length = (1 + maxExtendPercent)*strand->restLength;
				}
				else {
					length = (1 -maxExtendPercent)*strand->restLength;
				}
				strand->position[i] = strand->position[i - 1] + length*glm::normalize( strand->position[i]- strand->position[i - 1] );
			}
		}
		for (int i = 1; i < strand->massPointNum; i++) {
			glm::vec3 l = strand->position[i] - strand->position[i - 1]; glm::vec3 ld = (strand->velocity[i] - strand->velocity[i - 1]);
			glm::vec3 force = -stiffness * (glm::length(l) - strand->restLength)*glm::normalize(l);
			force += strand->bodyForce[i];
			force += (-damping) *glm::dot(ld, glm::normalize(l) )*glm::normalize(l)*timeStep;
			if (i != strand->massPointNum - 1) {
				l = strand->position[i] - strand->position[i + 1]; glm::vec3 ld = (strand->velocity[i] - strand->velocity[i + 1]);
				force += -stiffness * (glm::length(l) - strand->restLength)*glm::normalize(l);
				force += (-damping) *glm::dot(ld, glm::normalize(l))*glm::normalize(l)*timeStep;
			}
			//force /= strand->restLength;
			//float extendRatio = (glm::length(strand->nextPosition[i] - strand->nextPosition[i - 1]) - strand->restLength) / strand->restLength;
			//if (abs(extendRatio) > maxExtendPercent) {
			//	/*glm::vec3 a = strand->position[i] - strand->nextPosition[i - 1];
			//	float A = glm::dot(v, v);
			//	float B = 2 * glm::dot(a, v); float C;
			//	if (extendRatio > 0) {
			//	C = glm::dot(a, a) - (1 + maxExtendPercent)*strand->restLength*(1 + maxExtendPercent)*strand->restLength;
			//	}
			//	else {
			//	C = glm::dot(a, a) - (1 - maxExtendPercent)*strand->restLength*(1 - maxExtendPercent)*strand->restLength;
			//	}
			//	float t = max((1 / (2 * A))*(-B + sqrt(B*B - 4 * A*C)),(1 / (2 * A))*(-B - sqrt(B*B - 4 * A*C)));
			//	strand->nextPosition[i] = strand->position[i] + v * t;*/
			//	float length = strand->restLength;
			//	if (extendRatio > 0) {
			//		length = (1 + maxExtendPercent)*strand->restLength;
			//	}
			//	else {
			//		length = (1 - maxExtendPercent)*strand->restLength;
			//	}
			//	glm::vec3 pos = strand->nextPosition[i - 1] + length*glm::normalize(strand->nextPosition[i] - strand->nextPosition[i - 1]);
			//	glm::vec3 offset = pos - strand->nextPosition[i];
			//	strand->nextPosition[i] = pos;
			//}
			glm::vec3 closetP = glm::vec3(0);
			//Deal with collision
			if (headCollider->collide(strand->position[i], closetP)) {
				glm::vec3 forceDir = glm::normalize(strand->position[i] - closetP);
				force+= collideStiffness*abs(headCollider->radius - glm::length(strand->position[i] - closetP))* forceDir;
				strand->velocity[i] = strand->velocity[i] + force / strand->mass*timeStep;
			}else if(glm::length(strand->position[i] - closetP)<=headCollider->radius*(1+epsilonPercent)){
				strand->velocity[i] = glm::vec3(0);
			}
			else {
				strand->velocity[i] = strand->velocity[i] + force / strand->mass*timeStep;
			}
			strand->nextPosition[i] = strand->position[i] + strand->velocity[i] * timeStep;
		}

		strand->confirmUpdate();
	}

}
