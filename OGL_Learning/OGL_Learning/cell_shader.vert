#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 atexture;
out vec2 TexCoord;
uniform mat4 projection;	
uniform mat4 translation;

void main()
{
	mat4 model = translation;
	mat4 MVP = projection * model;
   gl_Position = MVP * vec4(aPos, 1.0);
   TexCoord = atexture;
}