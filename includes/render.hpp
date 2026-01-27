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

    void run_loop(appData &appData);

    // *************************************************

    void clean_ImGui();

    void clean_SDL(appData &appData);
}