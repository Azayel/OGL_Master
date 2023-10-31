#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;
out vec3 ourCol;
uniform vec3 addPos;
uniform mat4 MVP;	
uniform mat4 camera;
void main()
{
   mat4 mvp = MVP * camera;
   gl_Position = mvp * vec4(aPos+addPos, 1.0);
   ourCol = aCol;
}