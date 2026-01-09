#version 410 core

layout(location=0) in vec3 position;
layout(location=1) in vec3 vertexColors;

uniform mat4 uModelMatrix;

out vec3 v_vertexColors;

void main()
{
  v_vertexColors = vertexColors;

  vec4 newPosition = uModelMatrix * vec4(position, 1.0f);

  gl_Position = vec4(newPosition.x, newPosition.y, newPosition.z, 1.0f);
}