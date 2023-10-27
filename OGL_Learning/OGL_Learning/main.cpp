#include "mainloop.h";
#include <glad/glad.h>
#include <glfw3.h>
#include <iostream>

//Window
GLFWwindow* window;
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n";


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

}

int main( void )
{
    bool windowInitialized = initializeWindow();
    if (!windowInitialized) {
        return -1;
    }

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    bool vertexBufferInitialized = initializeVertexbuffer();
    if (!vertexBufferInitialized) return -1;

    //Now compile the shaders
    bool ShaderCompiled = compileShader();
    if (!ShaderCompiled) {
        return -1;
    }

    //Use The compiled shader:> Every shader and rendering call will use these shader now!
    glUseProgram(programID);
   



    while (!glfwWindowShouldClose(window))
    {
        

        processInput(window);

        //rendering loop here

        updateAnimationLoop();

        //Check and call events and swap the buffers here!
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }

    glDeleteVertexArrays(1, &VertexArrayID); 
    glDeleteBuffers(1, &vertexbuffer);  
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

    window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);

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
    

    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
    };

    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    return true;

}

bool compileShader() {

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        return false;
    }

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);


    programID = glCreateProgram();


    glAttachShader(programID, vertexShader);
    glAttachShader(programID, fragmentShader);
    glLinkProgram(programID);

    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(programID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAMM::LINK::FAILED\n" << infoLog << std::endl;
        return false;

    }

    //Delete shaders after linking and compiling 

    glDeleteShader(vertexShader); 
    glDeleteShader(fragmentShader); 

    return true;
}

void updateAnimationLoop(){

    // Clear the screen
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    /*
        One Vertex is a collection of x y z coordinates. We need to specify how what part of input to which vertex.
        Vertex 1    Vertex 2        Vertex 3
        x   y   z   x   y   z       x y z
        4B  4B  4B  4B ...
        Stride 12 bytes
        meaning vertex 2 begins at the 12th byte
    */

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // 3 meaning vec3 xyz

    glEnableVertexAttribArray(0);

    glDrawArrays(GL_TRIANGLES, 0, 3);
}