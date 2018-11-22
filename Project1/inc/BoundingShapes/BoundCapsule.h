#pragma once
#include "../../inc/common.h"
#include "../../inc/Object.h"
#include "../../inc/shader.h"
class BoundCapsule:Object
{
public:
	float radius;
	glm::vec3  start;
	glm::vec3 center;
	glm::vec3  end;
	BoundCapsule();
	~BoundCapsule();
	BoundCapsule(float  radius,
		glm::vec3  start,
		glm::vec3  end);
	bool collide(glm::vec3 p, glm::vec3 & closetP);
	glm::vec3 getClosetP(glm::vec3 p);
	void applyTransform(glm::vec3 translate, glm::vec3 rotate, glm::vec3 scale);
	void Draw(Shader shader);
	glm::vec3 getRealCenter() { return dataDraw[1]; }
private :
	glm::vec3 dataDraw[3];
	unsigned int VAO, VBO;
};

