#pragma once
#include "../../inc/common.h"
struct frameData {
	glm::vec3 translate;
	glm::vec3 rotate;
	glm::vec3 scale;
};

class kFrame
{
public:
	int index;
	frameData data;
	kFrame();
	kFrame(int frame, glm::vec3 translate, glm::vec3 rotate, glm::vec3 scale);
	~kFrame();

};

