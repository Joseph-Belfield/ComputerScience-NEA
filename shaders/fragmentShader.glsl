#version 410 core

in vec4 v_vertexColors;
in vec4 uColor

out vec4 color;

void main()
{
	vec4 newColor = uColor;
	
	color = newColor;
}	