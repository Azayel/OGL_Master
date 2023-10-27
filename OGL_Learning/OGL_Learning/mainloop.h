#pragma once


//Vertex Buffer Handling

unsigned int vertexbuffer;
unsigned int VertexArrayID;
unsigned int vertexbugger_size;

//ProgramID of the shader, required for handling with opengl

unsigned int programID;

int main(void); //<<< main function, called at startup
void updateAnimationLoop(); //<<< updates the animation loop
bool initializeWindow(); //<<< initializes the window using GLFW and GLEW
bool initializeVertexbuffer(); //<<< initializes the vertex buffer array and binds it OpenGL
bool cleanupVertexbuffer(); //<<< frees all recources from the vertex buffer
bool closeWindow(); //<<< Closes the OpenGL window and terminates GLFW