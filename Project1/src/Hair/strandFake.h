#pragma once
#include"HairStrand.h"
class strandFake {
public:
	int strandsNum;
	float a, b;
	std::vector<unsigned int> VBO_position, VAO;
	std::vector<unsigned int> VBO_normal;
	std::vector<unsigned int> VBO_tagent;

	std::vector<std::vector<glm::vec3>> position_draw;
	std::vector<std::vector<glm::vec3>> normal_draw;
	std::vector<std::vector<glm::vec3>> tagent_draw;
	void updateAndDraw(Shader shader, bool update);
	strandFake(HairStrand * guided,int strandsNum,float a, float b);
	void updateVertexProperty();
private:
	HairStrand * guidedStrand;
};
