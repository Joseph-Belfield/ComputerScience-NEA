#include "runLoop.hpp"

#include "defines/appData.hpp"
#include "defines/objectData.hpp"

#include "glad/gl.h"            // OpenGL extension loader

#include <vector>

void update_scene(appData &appData)
{
    // disables
    // glDisable(GL_DEPTH_TEST); // disables depth check - 2D scene
    // glDisable(GL_CULL_FACE);  // disables checking for overlap - 2D scene
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    // set size of window for OpenGL
    glViewport(0, 0, (int)(appData.window.window_width), (int)(appData.window.window_height));


    // background color
    glClearColor(appData.window.clearColor.x, appData.window.clearColor.y, appData.window.clearColor.z, appData.window.clearColor.w);                   // sets background color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     // clears the OpenGL color and depth buffers

    // selects program in use
    appData.shader[0] -> use();

    view_matrix(appData);                      // Makes a camera work!
    perspective_matrix(appData);               // creats illusion of perspective (size changes relative to camera)
}

void draw_objects(std::vector<Object*> objects, GLuint shaderProgram)
{
    for (int i = 0; i < objects.size(); i++)
    {
        if (objects[i] -> mesh.drawType == 0)
        {
            objects[i] -> draw_polygon(shaderProgram);
            if (objects[i] -> subclass == SPHERE)
            {
                objects[i] -> uniform.uRotate.y += 0.5f;
            }
        }
        else if (objects[i] -> mesh.drawType == 1)
        {
            objects[i] -> draw_lines(shaderProgram);
        }
    }   
}