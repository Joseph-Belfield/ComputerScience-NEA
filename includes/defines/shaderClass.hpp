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

    void set_vertexSource(std::string vertexFilename);          // change the source of the vertex
    void set_fragmentSource(std::string fragmentFilename);      // change the source of the fragment

    void compile_and_link();    // compile and link shader programs to shader program
};