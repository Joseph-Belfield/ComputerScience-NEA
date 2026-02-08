#version 410 core

layout(location=0) in vec3 position;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uPerspective;

void main()
{
  vec4 newPosition = uPerspective * uView * uModel * vec4(position, 1.0f);

  gl_Position = newPosition;
}