#pragma once

#include "glad/gl.h"
#include "SDL3/SDL.h"

#include <string>

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