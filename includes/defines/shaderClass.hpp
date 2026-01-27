#pragma once

#include "glad/gl.h"

#include <string>

class Shader
{
public:
    // shader program ID
    GLuint programID;

    // constructor
    Shader(std::string vertexFilename, std::string fragmentFilename);

    // methods
    void use();     // use the shader
};