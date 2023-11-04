#version 330 core
out vec4 FragColor;
in vec3 ourCol;
uniform vec3 acol;

void main()
{
    FragColor = vec4(ourCol+acol,1.0f);
}