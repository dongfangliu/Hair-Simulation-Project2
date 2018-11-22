#pragma once
#include "../../inc/Object.h"
#include "../../inc/BoundingShapes/BoundCapsule.h"
#include "../../inc/MathUtils.h"
#include "../../inc/shader.h"

class HairStrand:Object
{
public:
	HairStrand();
	HairStrand(BoundCapsule *head,float radius,glm::vec3 startPos,int num, float length, glm::vec3 dir = glm::vec3(0,1,0));
	~HairStrand();

	BoundCapsule * head;
	
	int massPointNum;
	std::vector<glm::vec3> bodyForce;

	std::vector<glm::vec3> position;
	std::vector<glm::vec3> nextPosition;
	std::vector<glm::vec3> prePosition;

	
	std::vector<glm::vec3> velocity;


	std::vector<glm::vec3> position_draw;
	std::vector<glm::vec3> normal_draw;
	std::vector<glm::vec3> tagent_draw;

	float restLength;
	float mass = 30;
	float hairRadius = 0.1;
	void applyTransform(glm::vec3 translate, glm::vec3 rotate, glm::vec3 scale);
	void confirmUpdate();
	void Draw(Shader shader, bool update);
private:
	unsigned int VBO_position, VAO;
	unsigned int VBO_normal;
	unsigned int VBO_tagent;
	glm::vec3 orientation;
	glm::vec3 startPos;
	std::vector<glm::vec3> position_init;
	void updateVertexProperty();
};

