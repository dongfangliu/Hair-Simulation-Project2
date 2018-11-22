#include "HairStrand.h"


HairStrand::HairStrand()
{
}

HairStrand::HairStrand(BoundCapsule *head, float radius, glm::vec3 startPos, int num, float length, glm::vec3 dir) :Object(startPos, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0))
{
	this->head = head;
	hairRadius = radius;
	massPointNum = num; restLength = length; orientation = dir; this->startPos = startPos;
	bodyForce.resize(massPointNum);
	position_init.resize(massPointNum);
	position.resize(massPointNum);
	nextPosition.resize(massPointNum);
	prePosition.resize(massPointNum);
	velocity.resize(massPointNum);

	position_draw.resize(2 * massPointNum - 1);
	normal_draw.resize(2 * massPointNum - 1);
	tagent_draw.resize(2 * massPointNum - 1);

	for (int i = 0; i < massPointNum; i++) {
		position[i] = startPos + restLength*i*glm::normalize(orientation) ;
		position_init[i] = position[i];
		prePosition[i] = position[i];
		nextPosition[i] = position[i];
	}
	
	updateVertexProperty();

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO_position); glGenBuffers(1, &VBO_normal); glGenBuffers(1, &VBO_tagent);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_position);
	glBufferData(GL_ARRAY_BUFFER, position_draw.size() * sizeof(glm::vec3), position_draw.data(), GL_DYNAMIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_normal);
	glBufferData(GL_ARRAY_BUFFER, normal_draw.size() * sizeof(glm::vec3), normal_draw.data(), GL_DYNAMIC_DRAW);

	// position attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_tagent);
	glBufferData(GL_ARRAY_BUFFER, tagent_draw.size() * sizeof(glm::vec3), tagent_draw.data(), GL_DYNAMIC_DRAW);

	// position attribute
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(2);

	glDrawArrays(GL_LINE_STRIP, 0, position_draw.size());
	
}




HairStrand::~HairStrand()
{
}

void HairStrand::applyTransform(glm::vec3 translate, glm::vec3 rotate, glm::vec3 scale)
{
	//update the first mass point ,remain others for solver
	glm::vec3 dir =position_init[0] - head->center;
	dir = glm::rotateZ(glm::rotateY(glm::rotateX(dir, rotate.x), rotate.y), rotate.z);
	position[0] = head->center + dir + translate;
	nextPosition[0] = position[0];
}

void HairStrand::confirmUpdate()
{
	for (int i = 0; i < massPointNum; i++) {
		prePosition[i] =position[i];
		position[i] = nextPosition[i];
		nextPosition[i] = glm::vec3();
	}
}

void HairStrand::
Draw(Shader shader,bool update)
{
	shader.use();
	glBindVertexArray(VAO);
	if (update) {
		updateVertexProperty();
		glBindBuffer(GL_ARRAY_BUFFER, VBO_position);
		glBufferSubData(GL_ARRAY_BUFFER, 0, position_draw.size() * sizeof(glm::vec3), position_draw.data());

		glBindBuffer(GL_ARRAY_BUFFER, VBO_normal);
		glBufferSubData(GL_ARRAY_BUFFER, 0, normal_draw.size() * sizeof(glm::vec3), normal_draw.data());

		glBindBuffer(GL_ARRAY_BUFFER, VBO_tagent);
		glBufferSubData(GL_ARRAY_BUFFER, 0, tagent_draw.size() * sizeof(glm::vec3), tagent_draw.data());
	}
	glDrawArrays(GL_LINE_STRIP,0, position_draw.size());


}

void HairStrand::updateVertexProperty()
{
	for (int i = 0; i < massPointNum; i++) {
		position_draw[2 * i] = spline(position, i);
		if (i != massPointNum - 1) {
			position_draw[2 * i + 1] = spline(position, i + 0.5f);
		}
	}
	for (int i = 0; i < massPointNum; i++) {
		normal_draw[2 * i] = glm::normalize(position_draw[2 * i] - head->getClosetP(position_draw[2 * i]));

		if (i != massPointNum - 1) {
			normal_draw[2 * i + 1] = glm::normalize(position_draw[2 * i + 1] - head->getClosetP(position_draw[2 * i + 1]));
		}
		if (i != massPointNum - 1) {
			tagent_draw[2 * i] = glm::normalize(position_draw[2 * i + 1] - position_draw[2 * i]);
			tagent_draw[2 * i + 1] = glm::normalize(position_draw[2 * i + 2] - position_draw[2 * i + 1]);
		}
		else {
			tagent_draw[2 * i] = tagent_draw[2 * i - 1];
		}
	}
}
