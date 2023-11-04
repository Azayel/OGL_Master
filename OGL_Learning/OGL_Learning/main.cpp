#include "mainloop.h";
#include "Shader.h"
#include "cells.h"

#include <glad/glad.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glfw3.h>

cells* mycells;

//Window
GLFWwindow* window;

const float width{ 1000 };
const float height{ 1000 };
float zoomFactor = 1.0f;

const int cell_dimension = 20;
const int sq_size = width / cell_dimension;


bool vbinit = false;
bool clicked = false;
std::vector<float> grid;
int clickgird[];
std::vector<float> rectangles;
std::vector<int> rect_indices;
unsigned int EBO;

glm::mat4 projection = glm::ortho(0.0f, width, 0.0f, height, -1.0f, 1.0f);
glm::vec3 transformation = glm::vec3(0.0f,0.0f,0.0f);
glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f),transformation);




struct RectangleVertex {
    float x, y, z;
};

std::vector<RectangleVertex> rectangle_verticies;
std::vector<unsigned int> rectangle_indicies;


glm::vec3 positions_to_world_from_screen(glm::vec3 coord) {
    glm::vec3 ndc_vec3 = glm::vec3(((2.0f * coord.x) / width - 1.0f), (1.0f - (2.0f * coord.y) / height), 0.0f);
    glm::vec4 homogeneous_ndc_vec4 = glm::vec4(ndc_vec3, 1.0f); //1.0f Due to beeing a position in space
    glm::vec4 inverse_projection_point = glm::inverse(projection) * homogeneous_ndc_vec4;
    inverse_projection_point = inverse_projection_point - glm::vec4(transformation,1.0f);   //1.0f "...."
    std::cout << "x: " << inverse_projection_point.x << " y: " << inverse_projection_point.y << "\n";
    return glm::vec3(inverse_projection_point.x, inverse_projection_point.y, inverse_projection_point.z);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        
        glfwGetCursorPos(window, &xpos, &ypos);
        std::cout << xpos << " " << ypos << std::endl;

        
        double cell_width = width / (double)cell_dimension;
        glm::vec3 gridcheck = positions_to_world_from_screen(glm::vec3(xpos, ypos, 0.0f));
        int column = glm::floor((gridcheck.x / cell_width));
        int row = glm::floor((gridcheck.y / cell_width));
        std::cout << column << " " << row << " aaand " << cell_width << std::endl;
        if (!(column >= 0 && column < cell_dimension && row >= 0 && row < cell_dimension)) return;

        float xlt = column * cell_width;
        float ylt = row * cell_width;
        float xrt = xlt + cell_width;
        float yrt = ylt;
        float xlb = xlt;
        float ylb = ylt + cell_width;
        float xrb = xlb + cell_width;
        float yrb = ylb;
        

        glm::vec3 lt = glm::vec3(xlt, ylt, 0.0f);
        glm::vec3 rt = glm::vec3(xrt, yrt, 0.0f);
        glm::vec3 rb = glm::vec3(xrb, yrb, 0.0f);
        glm::vec3 lb = glm::vec3(xlb, ylb, 0.0f);

   
        std::cout << "lt: " << lt.x << " " << lt.y << "\n";
        std::vector<glm::vec3> addVector = { lt ,rt,rb,lb };
        mycells->add(addVector);
    }
}








void processInput(GLFWwindow* window) {
    
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        transformation = glm::vec3{ transformation.x,transformation.y+=1.0f,transformation.z };
        translationMatrix = glm::translate(glm::mat4(1.0f), transformation);
    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        transformation = glm::vec3{ transformation.x,transformation.y -= 1.0f,transformation.z };
        translationMatrix = glm::translate(glm::mat4(1.0f), transformation);
    }
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        transformation = glm::vec3{ transformation.x -= 1.0f,transformation.y,transformation.z };
        translationMatrix = glm::translate(glm::mat4(1.0f), transformation);
    }
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        transformation = glm::vec3{ transformation.x += 1.0f ,transformation.y,transformation.z };
        translationMatrix = glm::translate(glm::mat4(1.0f), transformation);
    }
    

}

int main( void )
{

    bool windowInitialized = initializeWindow();
    if (!windowInitialized) {
        return -1;
    }

    
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glViewport(0, 0, width, height);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    bool vertexBufferInitialized = initializeVertexbuffer();
    if (!vertexBufferInitialized) return -1;
    vbinit = true;
    mycells = new cells();
   
    
   
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); WIRE FRAME
    Shader myShader("grid_shader.vert", "grid_shader.frag");
    myShader.use();
    programID = myShader.getID();

    while (!glfwWindowShouldClose(window))
    {
        

        processInput(window);

        //rendering loop here
        
        updateAnimationLoop();

        //Check and call events and swap the buffers here!
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }

    glDeleteVertexArrays(1, VertexArrayID); 
    glDeleteBuffers(1, vertexbuffer);  
    glDeleteProgram(programID);

    delete mycells;

    glfwTerminate();
    return 0;
}

bool initializeWindow() {
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n"); 
        
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    window = glfwCreateWindow(width, height, "LearnOpenGL", NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        
        glfwTerminate();
        return false;
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);


    return true;

   
}


bool initializeVertexbuffer() {
    
    
    glGenVertexArrays(2, VertexArrayID);
    //glGenBuffers(1, &elementbufferobject);
    glGenBuffers(2, vertexbuffer);
    glGenBuffers(1, &EBO);
    
    
    

    

    
    for (float x = 0; x <= width; x += (width / cell_dimension)) {
        grid.insert(grid.end(), { x, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f });
        grid.insert(grid.end(), { x, height, 0.0f, 1.0f, 0.0f, 0.0f });
    }

    for (float y = 0; y <= height; y += (height / cell_dimension)) {
        grid.insert(grid.end(), { 0.0f, y, 0.0f, 1.0f, 0.0f, 0.0f });
        grid.insert(grid.end(), { width, y, 0.0f, 1.0f, 0.0f, 0.0f });
    }
    

    
    
    //Initialize grid 
    glBindVertexArray(VertexArrayID[1]);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[1]);
    glBufferData(GL_ARRAY_BUFFER, grid.size()*sizeof(float), grid.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0,3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0),
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    //Initialize placed 
    

    return true;

}


void updateAnimationLoop(){

   

    // Clear the screen
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    draw_grid();
    draw_cells();
    

}


void draw_grid() {
    //Shader grid
    glUseProgram(programID);
    int uniformlocation;
    uniformlocation = glGetUniformLocation(programID, "translation");
    glUniformMatrix4fv(uniformlocation, 1, GL_FALSE, &translationMatrix[0][0]);

    uniformlocation = glGetUniformLocation(programID, "projection");
    glUniformMatrix4fv(uniformlocation, 1, GL_FALSE, &projection[0][0]);



    uniformlocation = glGetUniformLocation(programID, "acol");
    glUniform3f(uniformlocation, 0.0f, 0.0f, 0.0f);

    glBindVertexArray(VertexArrayID[1]);
    glDrawArrays(GL_LINES, 0, grid.size());
}

void draw_cells() {
    int uniformlocation;
    //Shader cells
    glUseProgram(mycells->get_programID());
    uniformlocation = glGetUniformLocation(programID, "translation");
    glUniformMatrix4fv(uniformlocation, 1, GL_FALSE, &translationMatrix[0][0]);

    uniformlocation = glGetUniformLocation(programID, "projection");
    glUniformMatrix4fv(uniformlocation, 1, GL_FALSE, &projection[0][0]);
    mycells->draw();
}