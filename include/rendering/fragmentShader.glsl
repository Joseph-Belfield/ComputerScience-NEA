// fragment shaders calculate the color output of the pixels on the screen.

#version 330 core
// output values declared with "out" keyword, stored in varaible named FragColor.
out vec4 FragColor;

void main()
{
    // FragColor set using a 4D vertex which stores colors as RGBA
    FragColor = vec4(0.0f, 0.0f, 1.0f, 1.0f);
}