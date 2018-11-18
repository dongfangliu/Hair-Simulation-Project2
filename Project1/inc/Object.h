#pragma once
#include "common.h"
#include <vector>
using namespace std;
class Object
{
public:
	glm::mat4 modelMatrix;
	Object * parent;
	vector<Object *> children;
	Object();
	Object(glm::vec3 initPos, glm::vec3 initRot, glm::vec3 initScale) {
		modelMatrix = glm::translate(modelMatrix, initPos); // translate it down so it's at the center of the scene
		modelMatrix = glm::scale(modelMatrix, initScale);	// it's a bit too big for our scene, so scale it down
		modelMatrix = glm::rotate(modelMatrix, initRot.x, glm::vec3(1, 0, 0));
		modelMatrix = glm::rotate(modelMatrix, initRot.y, glm::vec3(0, 1, 0));
		modelMatrix = glm::rotate(modelMatrix, initRot.z, glm::vec3(0, 0, 1));
		modelMatrix_base = modelMatrix;
	}
	~Object();
	virtual void Scale(float x, float y, float z) {
		modelMatrix = glm::scale(modelMatrix, glm::vec3(x, y, z));
	}
	virtual void Translate(float x, float y, float z) {
		modelMatrix = glm::translate(modelMatrix, glm::vec3(x, y, z));
	}
	virtual void Rotate(float angle, glm::vec3 axis) {
		modelMatrix = glm::rotate(modelMatrix, angle, axis);
	}
	virtual void Restore() {
		modelMatrix = modelMatrix_base;
	}
	virtual void applyTransform(glm::vec3 translate, glm::vec3 rotate, glm::vec3 scale) {
		modelMatrix = modelMatrix_base;
		modelMatrix = glm::scale(modelMatrix, scale);
		modelMatrix = glm::rotate(modelMatrix, rotate.x, glm::vec3(1, 0, 0));
		modelMatrix = glm::rotate(modelMatrix, rotate.y, glm::vec3(0, 1, 0));
		modelMatrix = glm::rotate(modelMatrix, rotate.z, glm::vec3(0, 0, 1));
		modelMatrix = glm::translate(modelMatrix, translate);
		glm::decompose(modelMatrix, this->scale, rotation, translation, skew, perspective);
	}

	glm::vec3 getPosition() {
		return translation;
	}
	glm::vec3 getRotation() {
		return glm::eulerAngles(rotation);
	}
	glm::vec3 getScale() {
		return scale;
	}
private:
	glm::vec3 scale;
	glm::quat rotation;
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::mat4 modelMatrix_base;
};

