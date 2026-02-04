#include "render/runLoop.hpp"

#include "appData.hpp"
#include "render/objectData.hpp"

#include "glad/gl.h"            // OpenGL extension loader

#include <vector>

// predefines
class Camera;

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
    appData.program.defaultShader.use();

    appData.program.defaultShader.set_perspective((float)appData.window.window_width, (float)appData.window.window_height);
    appData.program.defaultShader.set_view(appData.camera.camera1);
}

void draw_objects(std::vector<Object*> objects, Camera &camera, float width, float height)
{
    for (int i = 0; i < objects.size(); i++)
    {
        // draw the object
        objects[i] -> draw(camera, width, height);

        if (objects[i] -> subclass == SPHERE)
        {
            // objects[i] -> uniform.uRotate.y += 5.0f;
        }

    }   
}