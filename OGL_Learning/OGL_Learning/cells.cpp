#include "cells.h"
#include <glad/glad.h>
#include <iostream>

cells::cells() {
	//Gen Buffers
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	glGenVertexArrays(1, &vao);
	

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);


}

bool cells::add(std::vector<glm::vec3> v_add) {

    if (v_add.size() != 4) {
        std::cout << "std::vector.size() is not 4.\n";
        return false;
    }

    this->vertices.insert(this->vertices.end(), { v_add[0],v_add[1] ,v_add[2] ,v_add[3] });
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    this->update_indices();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

}


void cells::update_indices() {

    if(indices.size()==0){
        indices.insert(indices.end(), { 0, 1, 2, 2, 3, 0 });
    }
    else {
        unsigned int baseIndex = vertices.size() - 4;
        for (unsigned int i = 0; i < 6; i++) {
            indices.push_back(baseIndex + indices[i]);
        }
    }

    
}

void cells::draw() {


    glBindVertexArray(vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

}