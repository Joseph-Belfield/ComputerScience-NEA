// vertex shaders normalize vertex data to be displayed on the screen. written in GLSL.

// declaration of version (3.3) and profile (core).
#version 330 core
// declare all input vertex with "in" keyword. vec3 means a (math) vector of floats with 3 slots (for coords), named aPos.
layout (location = 0) in vec3 aPos;

void main()
{
    // position data (a 4D vector) is assigned to the gl_Positon varaible (the last item in the vector is "perspective division").
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}