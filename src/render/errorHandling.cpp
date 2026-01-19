#include "render.hpp"

#include "glad/gl.h"
#include "SDL3/SDL.h"

#include <iostream>
#include <string>

namespace error
{
    // write notes on reasons errors occour with these above these functions if errors ever begin to appear

    void check_SDL(bool SDL)
    {
        if (!SDL) 
        {
            std::cout << "Failed to initialize SDL." << std::endl;
            exit(-1); 
        }
    }

    void check_window(SDL_Window* window)
    {
        if (window == nullptr) 
        {
            std::cout << "Failed to create window." << std::endl;
            exit(-1); 
        }
    }

    void check_OpenGL_context(SDL_GLContext context)
    {
        if (context == nullptr) 
        {
            std::cout << "Failed to create OpenGL context." << std::endl;
            exit(-1); 
        }
    }

    // Make sure that GLAD is initialized before any OpenGL function calls!
    void check_GLAD(int GLAD)
    {
        if (!GLAD)
        {
            std::cout << "Error loading GLAD!" << std::endl;
            exit(-1);
        }
    }

    void check_windowDimensions(bool getDimensions)
    {
        if (!getDimensions)
        {
            std::cout << "Failed to get window size!" << std::endl;
            exit(-1);
        }
    }

    void check_shaderFull(std::string shaderResult)
    {
        if (shaderResult == "")
        {
            std::cout << "Shader file empty!" << std::endl;
        }
    }

    void check_shaderCompilation(GLuint shader, GLuint type)
    {
        int result;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &result);  // gets compile status, stores in result
        if (result == GL_FALSE)                                   // error checking <3
        {
            int length;                                                     
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);           // find the length of the error message
            char* errorMessages = new char[length];                             // create a C-string of that length
            glGetShaderInfoLog(shader, length, &length, errorMessages);   // log error info

            // display error messages
            if (type == GL_VERTEX_SHADER)
            {
            std::cout << "GL_VERTEX_SHADER compilation failed!" << std::endl;
            std::cout << errorMessages << std::endl;
            }
            else if(type == GL_FRAGMENT_SHADER)
            {
            std::cout << "GL_FRAGMENT_SHADER compilation failed!" << std::endl;
            std::cout << errorMessages << std::endl;
            }

            // reclaim memory
            delete[] errorMessages;

            // delete broken shader object
            glDeleteShader(shader);
            exit(1);
        }
    }

    int check_shaderProgram(GLuint program)
    {
        int result;
        char errorLog[512];
        glGetProgramiv(program, GL_LINK_STATUS, &result);
        if (!result)
        {
            glGetProgramInfoLog(program, 512, NULL, errorLog);
            std::cout << "Program link error: " << errorLog << std::endl;
        }
    }

    // Reports on OpenGL errors. Returns:
    // - error message
    // - error code
    // - error code meaning
    //
    // If objects fail to render, call throught code to find OpenGL error. Make sure to check:
    // - GLAD is initialized
    // - OpenGL context is current (and on the same thread)
    // - VAO is bound
    // - VAO is full
    // - Shader compilation
    void check_OpenGL_error()
    {
        int errorCode = glGetError();

        if (errorCode)
        {
            std::cout << "ERROR: " << errorCode << std::endl;
        }

        switch(errorCode)
        {
        case 1280:
            std::cout << "Set when an enumeration parameter is not legal." << std::endl;
            
        case 1281:
            std::cout << "Set when a value parameter is not legal." << std::endl;

        case 1282:
            std::cout << "Set when the state for a command is not legal for its given parameters." << std::endl;

        case 1283:
            std::cout << "Set when a stack pushing operation causes a stack overflow." << std::endl;

        case 1284:
            std::cout << "Set when a stack popping operation occurs while the stack is at its lowest point." << std::endl;

        case 1285:
            std::cout << "Set when a memory allocation operation cannot allocate (enough) memory." << std::endl;

        case 1286:
            std::cout << "Set when reading or writing to a framebuffer that is not complete." << std::endl;
        }
    }
}