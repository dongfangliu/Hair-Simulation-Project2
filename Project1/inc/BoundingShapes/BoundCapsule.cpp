#include "BoundCapsule.h"



BoundCapsule::BoundCapsule()
{
}


BoundCapsule::~BoundCapsule()
{

}

BoundCapsule::BoundCapsule(float radius, glm::vec3 start, glm::vec3 end) :radius(radius), start(start), end(end), center(0.5f*(start + end)) {
	dataDraw[0] = start; dataDraw[1] = center; dataDraw[2] = end;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(dataDraw), dataDraw, GL_DYNAMIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(0);
}

bool BoundCapsule::collide(glm::vec3 p,glm::vec3 & closetP)
{
	closetP = getClosetP(p);
	return glm::length(p-closetP)<=radius;
}
glm::vec3  BoundCapsule::getClosetP(glm::vec3 p) {
	glm::vec3 ap = p - dataDraw[0];
	glm::vec3 ab = dataDraw[2] - dataDraw[0];
	float projLen = glm::dot(ap, ab) / glm::length(ab);

	if (projLen < 0) projLen = 0;
	if (projLen > 1) projLen = 1;
	return dataDraw[0] + projLen * ab;
}

void BoundCapsule::applyTransform(glm::vec3 translate, glm::vec3 rotate, glm::vec3 scale)
{
	dataDraw[0] = center + glm::rotateZ(glm::rotateY(glm::rotateX((start - center), rotate.x), rotate.y), rotate.z) + translate;
	dataDraw[2] = center + glm::rotateZ(glm::rotateY(glm::rotateX((end - center), rotate.x), rotate.y), rotate.z) + translate;
	dataDraw[1] = center + translate;
}
void BoundCapsule::Draw(Shader shader)
{
	shader.use();
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(dataDraw), dataDraw, GL_DYNAMIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_LINE_STRIP, 0,3);
}
