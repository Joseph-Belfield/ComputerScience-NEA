#include "render/shaderClass.hpp"
#include "errorChecking.hpp"
#include "render/camera.hpp"

#include "glad/gl.h"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"


#include <string>
#include <iostream>
#include <fstream>

// *************************************************

// Load a shader from a file
// - Pass in the shader by file name. File name is passed only by reference to prevent duplication.
std::string load_shader_from_file(const std::string& fileName)
{
    // initialize result var
    std::string result = "";    // holds shader program as single string
    std::string line = "";      // holds one line of shader file at a time

    std::ifstream shaderFile(fileName.c_str());     // opens file

    // if the file is opened successfully
    if (shaderFile.is_open())
    {
        while(std::getline(shaderFile, line))   // go through each line of the file
        {
            result += line + "\n";              // concatinate new line into result string
        }

        shaderFile.close();                     // close file when done 
    }
    else
    {
        std::cout << "Shader file not found - check path!" << std::endl;
    }

    error::check_shaderFull(result);

    return result;
}


// Compiles veretx and fragment shaders.
// - Shader type is a (enum) OpenGL flag indicating the type of shader it is (vertex/fragment)
// - Source is the GLSL code for a shader passed as a C++ string
//
// If the shader compiles incorrectly, this function will cancel and log the appropriate error information.
GLuint compile_shader(GLuint type, const std::string source)
{
    // create shader object
    GLuint shaderObject;

    // check type of shader
    if (type == GL_VERTEX_SHADER)
    {
        shaderObject = glCreateShader(GL_VERTEX_SHADER);
    }
    else if (type == GL_FRAGMENT_SHADER)
    {
        shaderObject = glCreateShader(GL_FRAGMENT_SHADER);
    }

    // turn source to a C-string
    const char* C_source = source.c_str();

    // create shader source code
    glShaderSource
    (
        shaderObject,   // shader
        1,              // amount of elements compiled (1 shader)
        &C_source,           // shader source
        nullptr         // length of string 
    );

    // compile the shader
    glCompileShader(shaderObject);
    error::check_shaderCompilation(shaderObject, type);

    return shaderObject;
}

// adds the filepath to the shader folder to the front of the shader file name
std::string add_shaderFilepath(std::string fileName)
{
    std::string fullFilepath = "../../shaders/" + fileName;
    return fullFilepath;
}


Shader::Shader(std::string vertexFilename, std::string fragmentFilename)
{
    // finds the full filepath from the file names for the shaders
    std::string vertexFilepath = add_shaderFilepath(vertexFilename);
    std::string fragmentFilepath = add_shaderFilepath(fragmentFilename);

    // gets the shader code from their files as strings
    source_vertexShader = load_shader_from_file(vertexFilepath);
    source_fragmentShader = load_shader_from_file(fragmentFilepath);
}

// Sets the shader object as the shader program in use.
void Shader::use()
{
    glUseProgram(programID);
}

// Changes the source of the vertex shader. Only works before shader compilation!
void Shader::set_vertexSource(std::string vertexFilename)
{
    if (programID == 0)
    {
        std::string vertexFilepath = add_shaderFilepath(vertexFilename);
        source_vertexShader = load_shader_from_file(vertexFilepath);
    }
    else
    {
        std::cout << "Vertex shader cannot be changed - shader program already compiled!" << std::endl;
        exit(-1);
    }
}

// Changes the source of the fragment shader. Only works before shader compilation!
void Shader::set_fragmentSource(std::string fragmentFilename)
{
    if (programID == 0)
    {
        std::string fragmentFilepath = add_shaderFilepath(fragmentFilename);
        source_fragmentShader = load_shader_from_file(fragmentFilepath);
    }
    else
    {
        std::cout << "Fragment shader cannot be changed - shader program already compiled!" << std::endl;
        exit(-1);
    }
}

// Compile and links current vertex and fragment shaders to a shader program.
void Shader::compile_and_link()
{
    programID = glCreateProgram(); // creates an empty program to be filled with shaders

    // compile shaders
    GLuint vertexShader = compile_shader(GL_VERTEX_SHADER, source_vertexShader);
    GLuint fragmentShader = compile_shader(GL_FRAGMENT_SHADER, source_fragmentShader);

    // attatch shaders to program object
    glAttachShader(programID, vertexShader);    // attatches vertex shader to the object
    glAttachShader(programID, fragmentShader);  // attatches fragment shader to the object
    glLinkProgram(programID);   // links shaders together within object

    // validate program - check for errors
    error::check_shaderProgram(programID);

    // sets compiled as true so it can be checked later 
    compiled = true;
}

void Shader::set_float1(const std::string uniformName, float x)
{
    glUniform1f(glGetUniformLocation(programID, uniformName.c_str()), x);
}

// creates a 4-float vector uniform for the shader program
void Shader::set_float4(const std::string uniformName, float x, float y, float z, float w)
{   
    glUniform4f(glGetUniformLocation(programID, uniformName.c_str()), x, y, z, w);
}

// creates a 4x4 float matrix uniform for the shader program
void Shader::set_mat4(const std::string uniformName, int amount, bool enableTranspose, glm::mat4 matrix)
{
    glUniformMatrix4fv(glGetUniformLocation(programID, uniformName.c_str()), amount, enableTranspose, &matrix[0][0]);
}

void Shader::set_model(glm::vec3 displacement, glm::vec3 rotation, glm::vec3 scale)
{
    // movement
    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), displacement); // movement

    // rotations
    modelMatrix = glm::rotate(modelMatrix , rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));  // X
    modelMatrix = glm::rotate(modelMatrix , rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));  // Y
    modelMatrix = glm::rotate(modelMatrix , rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));  // Z

	// scale
    modelMatrix = glm::scale(modelMatrix, scale);

    // sends the model matrix to the GPU
    set_mat4("uModel", 1, false, modelMatrix);
}

void Shader::set_perspective(float width, float height)
{
    // projection matrix (in perspective)
    glm::mat4 perspective = glm::perspective      // create perspective matrix
                            (
                                glm::radians(90.0f),                                                       // FOV (radians)
                                (width / height),     // aspect ratio
                                0.1f,                                                                      // near clipping plane (min. distance)
                                100.0f                                                                     // far clipping plane (max. distance)
                            );  

    set_mat4("uPerspective", 1, false, perspective);
}

void Shader::set_view(Camera &camera)
{
    glm::mat4 viewMatrix = camera.get_view_matrix();

    set_mat4("uView", 1, false, viewMatrix);
}