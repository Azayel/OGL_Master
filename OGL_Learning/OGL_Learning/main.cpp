#include "mainloop.h";
#include "Shader.h"
#include <glad/glad.h>
#include <glfw3.h>
#include <iostream>
#include <vector>
//Window
GLFWwindow* window;

const int width{ 800 };
const int height{ 800 };


const int cell_dimension = 5;
const int sq_size = width / cell_dimension;

float x, y;

bool vbinit = false;

std::vector<float> grid;
int clickgird[];



void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        
        
        int mousex = (xpos / width) * cell_dimension;
        int mousey = (ypos / height) * cell_dimension;
        std::cout << mousex << mousey << std::endl;
         
        
    }
}


void processInput(GLFWwindow* window) {
    int button, action;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        y += 0.001f;
    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        y -= 0.001f;
    }
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        x -= 0.001f;
    }
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        x += 0.001f;
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
    //Now compile the shaders
    //bool ShaderCompiled = compileShader();
    //if (!ShaderCompiled) {
     //   return -1;
    //}

   
    
   
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
       
    

    for (float x = -1.0f; x <= 1.0f; x += (2.0f / cell_dimension)) {
        for (float y = -1.0f; y <= 1.0f; y += (2.0f / cell_dimension)) {
            grid.insert(grid.end(), { x, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f });
            grid.insert(grid.end(), { x, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f });
            grid.insert(grid.end(), { -1.0f, y, 0.0f, 1.0f, 1.0f, 1.0f });
            grid.insert(grid.end(), { 1.0f, y, 0.0f, 1.0f, 1.0f, 1.0f });

            if (y > 1 || y < -1 || x>1 || x<-1) {
                std::cout << "is larger\n";
            }
        }
    }
    

    

    //Initialize diagonal line 
    glBindVertexArray(VertexArrayID[1]);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[1]);
    glBufferData(GL_ARRAY_BUFFER, grid.size()*sizeof(float), grid.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0,3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0),
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    

    return true;

}


void updateAnimationLoop(){

    // Clear the screen
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    
   // int uniformlocation = glGetUniformLocation(programID, "addPos");
    //glUniform3f(uniformlocation, x, y, 0);
   // glBindVertexArray(VertexArrayID[0]);
   // glDrawArrays(GL_TRIANGLES, 0, 3);

    int uniformlocation = glGetUniformLocation(programID, "addPos");
    glUniform3f(uniformlocation, x, y, 0);
    glBindVertexArray(VertexArrayID[1]);
    glDrawArrays(GL_LINES,0,grid.size());


    
    
    

}