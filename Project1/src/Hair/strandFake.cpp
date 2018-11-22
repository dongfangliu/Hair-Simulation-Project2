#include "strandFake.h"






void strandFake::updateAndDraw( Shader shader, bool update)
{
	shader.use();
	glm::vec3 ori = glm::vec3(0, 0, 1);
	for (int i = 0; i < strandsNum; i++) {
		glBindVertexArray(VAO[i]);
		if (update) {
			updateVertexProperty();
			glBindBuffer(GL_ARRAY_BUFFER, VBO_position[i]);
			glBufferSubData(GL_ARRAY_BUFFER, 0,position_draw[i].size() * sizeof(glm::vec3), position_draw[i].data());

			glBindBuffer(GL_ARRAY_BUFFER, VBO_normal[i]);
			glBufferSubData(GL_ARRAY_BUFFER, 0, normal_draw[i].size() * sizeof(glm::vec3), normal_draw[i].data());

			glBindBuffer(GL_ARRAY_BUFFER, VBO_tagent[i]);
			glBufferSubData(GL_ARRAY_BUFFER, 0, tagent_draw[i].size() * sizeof(glm::vec3), tagent_draw[i].data());
		}
		glDrawArrays(GL_LINE_STRIP, 0, position_draw[i].size());
	}
}

strandFake::strandFake(HairStrand * guided, int strandsNum,float a, float b )
{
	guidedStrand = guided;
	this->strandsNum = strandsNum;
	this->a = a; this->b = b;
	VAO.resize(strandsNum); VBO_position.resize(strandsNum); VBO_normal.resize(strandsNum); VBO_tagent.resize(strandsNum);
	position_draw.resize(strandsNum); normal_draw.resize(strandsNum); tagent_draw.resize(strandsNum);
	for (int i = 0; i < strandsNum; i++) {
		position_draw[i].resize(guided->position_draw.size());
		tagent_draw[i].resize(guided->position_draw.size());
		normal_draw[i].resize(guided->position_draw.size());
	}
	updateVertexProperty();
	for (int i = 0; i < strandsNum; i++) {
		glGenVertexArrays(1, &VAO[i]);
		glGenBuffers(1, &VBO_position[i]);
		glGenBuffers(1, &VBO_normal[i]);
		glGenBuffers(1, &VBO_tagent[i]);
		// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
		glBindVertexArray(VAO[i]);

		glBindBuffer(GL_ARRAY_BUFFER, VBO_position[i]);
		glBufferData(GL_ARRAY_BUFFER, position_draw[i].size() * sizeof(glm::vec3), position_draw[i].data(), GL_DYNAMIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_normal[i]);
		glBufferData(GL_ARRAY_BUFFER, normal_draw[i].size() * sizeof(glm::vec3), normal_draw[i].data(), GL_DYNAMIC_DRAW);

		// position attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ARRAY_BUFFER, VBO_tagent[i]);
		glBufferData(GL_ARRAY_BUFFER, tagent_draw[i].size() * sizeof(glm::vec3), tagent_draw[i].data(), GL_DYNAMIC_DRAW);

		// position attribute
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
		glEnableVertexAttribArray(2);
	}
}

void strandFake::updateVertexProperty()
{

	int massPointNum = (guidedStrand->position_draw.size() + 1) / 2;
	for (int i = 0; i < strandsNum; i++) {
		float theta = 2 * glm::pi<float>() / strandsNum*i;
		glm::quat rot;
		glm::vec3 offset = (a + b*theta)*glm::vec3(cos(theta), sin(theta), 0);
		for (int j = 0; j <guidedStrand->position_draw.size(); j++) {
			if (j != guidedStrand->position_draw.size() - 1) {
				glm::vec3 dir = glm::normalize(guidedStrand->position_draw[j + 1] - guidedStrand->position_draw[j]);
				rot = glm::rotation(glm::vec3(0, 0, 1), dir);
			}
			offset = offset*rot;
			position_draw[i][j] = guidedStrand->position_draw[j] + offset;
		}
	}
	for (int i = 0; i < strandsNum; i++) {
		for (int j = 0; j < massPointNum; j++) {
			normal_draw[i][2 * j] = glm::normalize(position_draw[i][2 * j] - guidedStrand->head->getClosetP(position_draw[i][2 * j]));
			if (j != massPointNum - 1) {
				normal_draw[i][2 * j + 1] = glm::normalize(position_draw[i][2 * j+1] - guidedStrand->head->getClosetP(position_draw[i][2 * j+1]));
			}
			if (j != massPointNum - 1) {
				tagent_draw[i][2 * j] = glm::normalize(position_draw[i][2 * j + 1] - position_draw[i][2 * j ]);
				tagent_draw[i][2 * j + 1] = glm::normalize(position_draw[i][2 * j + 2] - position_draw[i][2 * j + 1]);
			}
			else {
				tagent_draw[i][2 * j] = tagent_draw[i][2 * j- 1];
			}
		}
	}
}
