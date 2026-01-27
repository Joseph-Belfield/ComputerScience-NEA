#pragma once

#include "glad/gl.h"

#include <string>

class Shader
{
public:
    // shader program ID
    GLuint programID;

    std::string source_vertexShader;
    std::string source_fragmentShader;

    // constructor with default values
    Shader(std::string vertexFilename = "vertexShader.glsl", std::string fragmentFilename = "fragmentShader.glsl");

    // methods
    void use();     // use the shader
    void compile_and_link();    // compile and link shader programs to shader program
};