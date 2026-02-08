#pragma once

#include "glad/gl.h"
#include "glm/mat4x4.hpp"

#include <string>

// predefines
class Camera;


class Shader
{
public:
    // shader program ID
    GLuint programID;

    std::string source_vertexShader;
    std::string source_fragmentShader;

    // defaults to base shaders
    uint versionVertex = 0;
    uint versionFragment = 0;

    // stores whether the shader has been compiled or not
    bool compiled = false;

    // constructor with default values
    Shader(std::string vertexFilename = "version1.vs", std::string fragmentFilename = "version1.fs");

    // secondary constructor that chooses shader's based on their version (must be manually updated)
    Shader(const uint version_vertex, const uint vertsion_fragment);

    // methods
    void use();     // use the shader

    void set_vertexSource(std::string vertexFilename);          // change the source of the vertex
    void set_fragmentSource(std::string fragmentFilename);      // change the source of the fragment

    // gets the versions of the shaders
    void get_shaderVersions();

    void compile_and_link();    // compile and link shader programs to shader program

    void set_float1(const std::string uniformName, float x);
    void set_float4(const std::string uniformName, float x, float y, float z, float w);
    void set_mat4(const std::string uniformName, int amount, bool enableTranspose, glm::mat4 matrix);

    void set_model(glm::vec3 displacement, glm::vec3 rotation, glm::vec3 scale);
    void set_perspective(float width, float height);
    void set_view(Camera* camera);
};