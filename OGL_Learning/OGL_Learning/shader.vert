#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;
out vec3 ourCol;
uniform vec3 addPos;
uniform mat4 projection;	
uniform mat4 view;
uniform mat4 translationmatrix;
uniform mat4 scalematrix;

void main()
{
	mat4 model = translationmatrix * scalematrix;
   gl_Position = projection * view * model * vec4(aPos+addPos, 1.0);
   ourCol = aCol;
}