#include "runLoop.hpp"
#include "defines/contextData.hpp"

#include "glad/gl.h"            // OpenGL extension loader

void update_scene(appData &appData)
    {
        // disables
        // glDisable(GL_DEPTH_TEST); // disables depth check - 2D scene
        // glDisable(GL_CULL_FACE);  // disables checking for overlap - 2D scene
        glEnable(GL_DEPTH_TEST);

        // set size of window for OpenGL
        glViewport(0, 0, (int)(appData.window.window_width), (int)(appData.window.window_height));


        // background color
        glClearColor(appData.window.clearColor.x, appData.window.clearColor.y, appData.window.clearColor.z, appData.window.clearColor.w);                   // sets background color
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);     // clears the OpenGL color and depth buffers

        // selects program in use
        glUseProgram(appData.program.shaderProgram); 

        view_matrix(appData);                      // Makes a camera work!
        perspective_matrix(appData);               // creats illusion of perspective (size changes relative to camera)
    }