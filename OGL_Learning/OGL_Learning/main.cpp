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


const int cell_dimension = 20;
const int sq_size = width / cell_dimension;

//Keep track of moved grid coordinates
float gridx, gridy;
float mousemovx{0}; 
float mousemovy{ 0 };
bool drag = false;
std::vector<float> grid;
int clickgird[];

//placed rectangles
std::vector<float> rectangles;
std::vector<int> rect_indices;


unsigned int EBO;
//glm::mat4 projection = glm::ortho(0.0f, width, height, 0.0f, -1.0f, 1.0f);
float rad = 45.0f;
glm::mat4 projection = glm::perspective(glm::radians(rad),1.0f,0.1f,1000.0f);
glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 1000.0f);
glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::mat4 camera = glm::lookAt(cameraPosition, cameraTarget, cameraUp);



struct GridSquare {
    glm::vec3 verticies[4];
};

std::vector<GridSquare> grid_square;



struct RectangleVertex {
    float x, y, z;
};

std::vector<RectangleVertex> rectangle_verticies;
std::vector<unsigned int> rectangle_indicies;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}






void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        
        //May God Save me for i do not know if i have the strength to go forther. 
        //The idea is to transform the mouseclick into a ray which shoots of into the grid and determine if an intersection of the ray and a plane exists?????? HELP
        //this will certainly kill my brain. Too bad.

        //transform the ray from screenspace to world space 2d to 3d. ?
        //Meaning http://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices/
        //Homogenous coordinates to camera coordinates to world space coordinates
        //W=1; ? Its a vector isnt it?

        glm::vec4 ray_clip = glm::vec4((float)(2.0 * xpos / width - 1.0), (float)(1.0 - 2.0 * ypos / height), -1.0, 1.0);
        //we create a ray from the click of a mouse to ndc, z=-1 points into screen, 1.0 homogenous coordinate its a vector


        //get inverse projection because mvp*camera

        //https://antongerdelan.net/opengl/raycasting.html

        //WELP Now i need to determine an intersection of the square plane but I NEVER SAVED THE GRID VERTICIES. They are permanently stored and drawn from the buffer.............
        //structs here we go
        //created struct gridsquare which is stored in vector grid_square;
        //this struct has all bottom right coordinates of the square and its height and width? Why i am storing them I do not know

        for (const GridSquare& square : grid_square) {
            



        }

        std::cout << xpos << " " << ypos << std::endl;



        double cell_width = width / (double)cell_dimension;
        int column = (int)((xpos- gridx) / cell_width);
        int row = (int)((ypos - gridy) / cell_width);
        
        
        
        
        
        
        
        std::cout << column << " " << row << " " << cell_width <<  std::endl;
        

        

       

        //if (!(column >= 0 && column < cell_dimension && row >= 0 && row < cell_dimension)) return;
           
        
        //1. Get the 4 Vertex Coordiates
        float xlt = column * cell_width;
        float ylt = row * cell_width;
        float xrt = xlt + cell_width;
        float yrt = ylt;
        float xlb = xlt;
        float ylb = ylt + cell_width;
        float xrb = xlb + cell_width;
        float yrb = ylb;

        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        rectangle_verticies.insert(rectangle_verticies.end(), { 
            RectangleVertex{xlt, ylt, 0.0f}, 
            RectangleVertex{xrt, yrt, 0.0f},
            RectangleVertex{xrb, yrb, 0.0f},
            RectangleVertex{xlb, ylb, 0.0f}
        });

        std::cout << "XLT: " << xlt << " ylt: " << ylt << std::endl;

        unsigned int baseIndex = rectangle_verticies.size() - 4; 
        for (unsigned int i = 0; i < 6; i++) {
            rect_indices.push_back(baseIndex + indices[i]);
        }
        
        glBindVertexArray(VertexArrayID[0]);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[0]);

        glBufferData(GL_ARRAY_BUFFER, rectangle_verticies.size() * sizeof(RectangleVertex), rectangle_verticies.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        


        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, rect_indices.size() * sizeof(unsigned int), rect_indices.data(), GL_STATIC_DRAW);

    }
    else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        drag = true;
    }
    else if(action == GLFW_RELEASE) {
        drag = false;
    }
}


void processInput(GLFWwindow* window) {
    
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        gridy -= 10.0f;
    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        gridy += 10.0f;
    }
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        gridx += 10.0f;
    }
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        gridx -= 10.0f;
    }
    else if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
        rad += 1.0f;
    }
    else if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
        rad -= 1.0f;
    }
    
    

}

void scroll_callback(GLFWwindow* window , double xoffset, double yoffset){
    rad -= yoffset*3;
    if (rad < 1.0f) rad = 1.0f;
    if (rad > 120.0f) rad = 120.0f;
    projection = glm::perspective(glm::radians(rad), 1.0f, 0.1f, 1000.0f);

}



int main( void )
{
    gridx={ 0 };
    gridy={ 0 };
    bool windowInitialized = initializeWindow();
    if (!windowInitialized) {
        return -1;
    }

    
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glViewport(0, 0, width, height);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);
    bool vertexBufferInitialized = initializeVertexbuffer();
    if (!vertexBufferInitialized) return -1;

   
    
   
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
    

    for (float x = 0.0; x < cell_dimension-1; x++) {
        for (float y = 0.0; y < cell_dimension-1; y++) {

            GridSquare s;

            float x0 = x * (width / cell_dimension);
            float y0 = y * (height / cell_dimension);
            float x1 = (x + 1) * (width / cell_dimension);
            float y1 = y0;
            float x2 = x1;
            float y2 = (y + 1) * (height / cell_dimension);
            float x3 = x0;
            float y3 = x2;

            s.verticies[0] = glm::vec3(x0, y0, 0.0f);
            s.verticies[1] = glm::vec3(x1, y1, 0.0f);
            s.verticies[2] = glm::vec3(x2, y2, 0.0f);
            s.verticies[3] = glm::vec3(x3, y3, 0.0f);
            grid_square.push_back(s);
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

    //get Projectionmatrix and pass it into the fragment shader
    int uniformlocation;
    uniformlocation = glGetUniformLocation(programID, "MVP");
    glUniformMatrix4fv(uniformlocation, 1, GL_FALSE, &projection[0][0]);
    
   

    uniformlocation = glGetUniformLocation(programID, "camera");
    glUniformMatrix4fv(uniformlocation, 1, GL_FALSE, &camera[0][0]);

    
    //draw Grid
    uniformlocation = glGetUniformLocation(programID, "acol");
    glUniform3f(uniformlocation, 0.0f, 0.0f, 0.0f);
    uniformlocation = glGetUniformLocation(programID, "addPos");
    glUniform3f(uniformlocation, gridx, gridy, 0);
    
    
    glBindVertexArray(VertexArrayID[1]);
    glDrawArrays(GL_LINES,0,grid.size());
    

    //Draw Rectangles
    uniformlocation = glGetUniformLocation(programID, "acol");
    glUniform3f(uniformlocation, 0.0f, 0.0f, 1.0f);
    glBindVertexArray(VertexArrayID[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glDrawElements(GL_TRIANGLES, rect_indices.size(), GL_UNSIGNED_INT, 0);
    
    
    

}