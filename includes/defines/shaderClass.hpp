#pragma once

#include "glad/gl.h"

#include <string>

class Shader
{
public:
    // shader program ID
    GLuint programID;

    // constructor with default values
    Shader(std::string vertexFilename = "vertexShader.glsl", std::string fragmentFilename = "fragmentShader.glsl");

    // methods
    void use();     // use the shader
};