#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;
out vec3 ourCol;
uniform vec3 addPos;
uniform mat4 MVP;	
void main()
{
   gl_Position = MVP * vec4(aPos+addPos, 1.0);
   ourCol = aCol;
}