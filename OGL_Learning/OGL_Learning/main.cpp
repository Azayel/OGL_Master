#include "mainloop.h";
#include <glad/glad.h>
#include <glfw3.h>
#include <iostream>

//Window
GLFWwindow* window;
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aCol;\n"
"out vec3 ourCol;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0);\n"
"   ourCol = aCol;\n"
"}\n";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 ourColor;\n"
"in vec3 ourCol;\n"
"void main()\n"
"{\n"
"    FragColor = ourColor;\n"
"}\n";

const char* fragmentShaderSource2 = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourCol;\n"
"void main()\n"
"{\n"
"    FragColor = vec4(ourCol,1.0f);\n"
"}\n";

float x, y;

bool vbinit = false;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        y += 0.001;
    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        y -= 0.001;
    }
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        x -= 0.001;
    }
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        x += 0.001;
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

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    bool vertexBufferInitialized = initializeVertexbuffer();
    if (!vertexBufferInitialized) return -1;
    vbinit = true;
    //Now compile the shaders
    bool ShaderCompiled = compileShader();
    if (!ShaderCompiled) {
        return -1;
    }

   
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); WIRE FRAME


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
    
    
    glGenVertexArrays(2, VertexArrayID);
    //glGenBuffers(1, &elementbufferobject);
    glGenBuffers(2, vertexbuffer);
       
    

    float firstTriangle[]{
        -0.5f,0.0f,0.0f,
        0.0f,0.0f,0.0f,
        0.0f,0.5f,0.0f
    };

    float seccondTriangle[]{
        0.0f,0.0f,0.0f, 1.0f, 0.0f, 0.0f,
        0.5f,0.0f,0.0f, 0.0f, 1.0f, 0.0f,
        0.0f,0.5f,0.0f, 0.0f, 0.0f, 1.0f,
    };
    
    
    //Initializing first triangle
    glBindVertexArray(VertexArrayID[0]);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    //Initialize seccond tirangle
    glBindVertexArray(VertexArrayID[1]);
    glBindBuffer(GL_ARRAY_BUFFER,vertexbuffer[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(seccondTriangle), seccondTriangle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); 
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    
    

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

    unsigned int fragmentShader2;
    fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader2, 1, &fragmentShaderSource2, NULL);
    glCompileShader(fragmentShader2);


    programID = glCreateProgram();
    programIDYellow = glCreateProgram();

    glAttachShader(programID, vertexShader);
    glAttachShader(programID, fragmentShader);
    glLinkProgram(programID);

    glAttachShader(programIDYellow, vertexShader);
    glAttachShader(programIDYellow, fragmentShader2);
    glLinkProgram(programIDYellow);

    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(programID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAMM::LINK::FAILED\n" << infoLog << std::endl;
        return false;

    }

    //Delete shaders after linking and compiling 

    glDeleteShader(vertexShader); 
    glDeleteShader(fragmentShader); 
    glDeleteShader(fragmentShader2);

    return true;
}

void updateAnimationLoop(){

    // Clear the screen
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    float timeValue = glfwGetTime();
    //float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
    float redValue = (cos(timeValue) / 2.0f) + 0.5f;

    int vertexColorLocation = glGetUniformLocation(programID, "ourColor");
    glUseProgram(programID);
    glUniform4f(vertexColorLocation, redValue, 0.0f, 0.0f, 1.0f);
    glBindVertexArray(VertexArrayID[0]);
    glDrawArrays(GL_TRIANGLES, 0, 3);


    
 
    glUseProgram(programIDYellow);
    glBindVertexArray(VertexArrayID[1]);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    

}