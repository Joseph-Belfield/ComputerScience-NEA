#version 410 core

in vec3 v_vertexColors;

out vec4 finalColor;

uniform uOpacity;

void main()
{
	finalColor = vec4(v_vertexColors, uOpacity);
}	