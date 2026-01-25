#pragma once

// must be included so that types below work
#include "glad/gl.h"
#include "glm/glm.hpp"
#include "SDL3/SDL.h"

#include <string>
#include <vector>

// forward declare classes, structs 
struct appData;
struct Object;

namespace render
{
    void init_SDL(appData &appData);

    void set_OpenGL_Attributes();

    void init_OpenGL(appData &appData);

    void init_ImGui(appData &appData);

    // *************************************************

    // Load a shader from a file
    // - Pass in the shader by file name. File name is passed only by reference to prevent duplication.
    std::string load_shader_from_file(const std::string& fileName);

    // Compiles veretx and fragment shaders.
    // - Shader type is a (enum) OpenGL flag indicating the type of shader it is (vertex/fragment)
    // - Source is the GLSL code for a shader passed as a C++ string
    //
    // If the shader compiles incorrectly, this function will cancel and log the appropriate error information.
    GLuint compile_shader(GLuint type, const std::string source);

    // Creates a shader program object.
    // - The function passes in the vertex and fragment shaders' GLSL code as C++ strings
    //
    // The shaders are compiled within the function, and then attatcehed to the program object. The program object is then validated.
    GLuint create_shader_program(const std::string source_vertexShader, const std::string source_fragmentShader);

    // Creates a shader program using the shaders found in the shader folder. The shader program is referenced using a unique unsigned integer value assigned as its ID.
    void create_graphics_pipeline(appData &appData);

    // *************************************************

    void run_loop(appData &appData);

    // *************************************************

    void clean_ImGui();

    void clean_SDL(appData &appData);
}

namespace error
{
    void check_SDL(bool SDL);
    void check_window(SDL_Window* window);
    void check_OpenGL_context(SDL_GLContext context);
    void check_GLAD(int GLAD);
    void check_windowDimensions(bool getDimensions);

    void check_shaderFull(std::string shaderResult);
    void check_shaderCompilation(GLuint shader, GLuint type);
    void check_shaderProgram(GLuint program);

    void check_OpenGL_error();
    void check_boundVAO();
}