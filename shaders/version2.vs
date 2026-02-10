#version 410 core

layout(location=0) in vec3 aPosition;
layout(location=1) in vec3 aNormal;

out vec3 vNormal;
out vec3 vFragPos;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uPerspective;
uniform vec3 uLightPos;

void main()
{
    vNormal = aNormal;
    vFragPos = vec3(uModel * vec4(aPosition, 1.0f))

    gl_Position = uPerspective * uView * uModel * vec4(aPosition, 1.0f);;
}