#version 410 core

in vec3 vNormal;
in vec3 vFragPos;

out vec4 FragColor;

uniform vec3 uObjectColor;
uniform vec3 uLightColor;
uniform float uAmbience;

void main()
{
    vec3 ambient = uLightColor * uAmbience;

    vec3 finalColor = ambient * uObjectColor;
	FragColor = vec4(finalColor, 1.0f);
}