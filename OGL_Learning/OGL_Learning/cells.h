#pragma once
#include <vector>
#include <glm/glm.hpp>

class cells {
	unsigned int vbo;
	unsigned int vao;
	unsigned int ebo;
	std::vector<unsigned int> indices;
	std::vector<glm::vec3> vertices;
	



	void update_indices();

public:
	
	cells();
	bool add(std::vector<glm::vec3>);
	bool update();
	void draw();
	
};