#pragma once

#include "glad/gl.h"
#include "glm/mat4x4.hpp"

#include <string>

class Shader
{
public:
    // shader program ID
    GLuint programID;

    std::string source_vertexShader;
    std::string source_fragmentShader;

    // constructor with default values
    Shader(std::string vertexFilename = "version0.vs", std::string fragmentFilename = "version0.fs");

    // methods
    void use();     // use the shader

    void set_vertexSource(std::string vertexFilename);          // change the source of the vertex
    void set_fragmentSource(std::string fragmentFilename);      // change the source of the fragment

    void compile_and_link();    // compile and link shader programs to shader program

    void set_float1(const std::string uniformName, float x);
    void set_float4(const std::string uniformName, float x, float y, float z, float w);
    void set_mat4(const std::string uniformName, int amount, bool enableTranspose, glm::mat4 matrix);
};