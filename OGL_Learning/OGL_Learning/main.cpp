#include "mainloop.h";
#include "Shader.h"
#include <glad/glad.h>
#include <glfw3.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Window
GLFWwindow* window;

const float width{ 1000 };
const float height{ 1000 };


const int cell_dimension = 5;
const int sq_size = width / cell_dimension;

float x, y;

bool vbinit = false;
bool clicked = false;
std::vector<float> grid;
int clickgird[];
std::vector<float> rectangles;
std::vector<int> rect_indices;
unsigned int EBO;
glm::mat4 projection = glm::ortho(0.0f, width, 0.0f, height, -1.0f, 1.0f);

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        double cell_width = width / (double)cell_dimension;
        int column = (int)(xpos / cell_width);
        int row = (int)(ypos / cell_width);
        std::cout << column << " " << row << " " << cell_width <<  std::endl;
        
    }
}


void processInput(GLFWwindow* window) {
    
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        y += 10.0f;
    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        y -= 10.0f;
    }
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        x -= 10.0f;
    }
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        x += 10.0f;
    }
    

}

int main( void )
{
    x={ 0 };
    y={ 0 };
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

   
    
   
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); WIRE FRAME
    Shader myShader("shader.vert", "shader.frag");
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
        for (float y = 0.0f; y <= height; y += (height / cell_dimension)) {
            grid.insert(grid.end(), { x, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f });
            grid.insert(grid.end(), { x, height, 0.0f, 1.0f, 0.0f, 0.0f });
            grid.insert(grid.end(), { 0.0f, y, 0.0f, 1.0f, 0.0f, 0.0f });
            grid.insert(grid.end(), { width, y, 0.0f, 1.0f, 0.0f, 0.0f });

        }
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
    int uniformlocation;
    uniformlocation = glGetUniformLocation(programID, "MVP");
    glUniformMatrix4fv(uniformlocation, 1, GL_FALSE, &projection[0][0]);
    

    uniformlocation = glGetUniformLocation(programID, "acol");
    glUniform3f(uniformlocation, 0.0f, 0.0f, 0.0f);
    uniformlocation = glGetUniformLocation(programID, "addPos");
    glUniform3f(uniformlocation, x, y, 0);
    glBindVertexArray(VertexArrayID[1]);
    glDrawArrays(GL_LINES,0,grid.size());


    
    
    

}