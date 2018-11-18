#include "kFrame.h"



kFrame::kFrame()
{
	index = -1;
	data.scale = glm::vec3(1);
	data.rotate = glm::vec3(0,0,0);
	data.translate = glm::vec3(0, 0, 0);
}

kFrame::kFrame(int frame, glm::vec3 translate, glm::vec3 rotate, glm::vec3 scale)
{
	this->index = frame;
	this->data.translate = translate;
	this->data.rotate = rotate;
	this->data.scale = scale;
}



kFrame::~kFrame()
{
}
