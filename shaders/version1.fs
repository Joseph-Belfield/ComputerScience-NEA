#version 410 core

in vec3 v_vertexColors;

out vec4 finalColor;

uniform vec4 uColor;

void main()
{
	finalColor = uColor;
}	