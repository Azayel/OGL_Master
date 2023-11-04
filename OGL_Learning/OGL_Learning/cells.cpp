#include "cells.h"
#include "Shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glad/glad.h>
#include <iostream>

cells::cells() {
	//Gen Buffers
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	glGenVertexArrays(1, &vao);

    //Textures from here
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char* data = stbi_load("missingtexture.jpeg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        textures.insert(textures.end(), { glm::vec2(1.0f, 1.0f),glm::vec2(1.0f, 0.0f),glm::vec2(0.0f, 0.0f),glm::vec2(0.0f, 1.0f) });
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    stbi_image_free(data);


    //Textures to here

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    //new
    glBindBuffer(GL_ARRAY_BUFFER, texture);
    glBufferData(GL_ARRAY_BUFFER, textures.size() * sizeof(glm::vec2), textures.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);


    Shader myShader("cell_shader.vert", "cell_shader.frag");
    this->programID = myShader.getID();


}

bool cells::add(std::vector<glm::vec3> v_add) {
    

    if (v_add.size() != 4) {
        std::cout << "std::vector.size() is not 4.\n";
        return false;
    }
    if (this->exists(v_add)) {
        return false;
    }

    std::vector<glm::vec2> texCoords = {
        glm::vec2(1.0f, 1.0f),
        glm::vec2(1.0f, 0.0f),
        glm::vec2(0.0f, 0.0f),
        glm::vec2(0.0f, 1.0f)
    };

    this->vertices.insert(this->vertices.end(), { v_add[0],v_add[1] ,v_add[2] ,v_add[3] });
    this->textures.insert(this->textures.end(), texCoords.begin(), texCoords.end());

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ARRAY_BUFFER, texture);
    glBufferData(GL_ARRAY_BUFFER, textures.size() * sizeof(glm::vec2), textures.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

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

    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

}

bool cells::exists(const std::vector<glm::vec3>& tosearch) {
    if (vertices.size() % 4 != 0) {
        std::cout << "Impossible to search if exist due to error in stack, can't be divided by 4.\n";
        return false; // Return false to indicate that the search is impossible.
    }

    for (size_t i = 0; i < vertices.size(); i += 4) {
        bool match = false;

        for (size_t j = 0; j < 4; j++) {
            if (vertices[i + j] != tosearch[j]) {
                break; 
            }
            else {
                match = true;
                break;
            }
        }

        if (match) {
            std::cout << "Found Match\n";
            return true;  // Return true as soon as a matching quad is found.
        }
    }

    std::cout << "Match not found\n";
    return false;  // If no match is found, return false.
}


unsigned int cells::get_programID(){
    return programID;
}
