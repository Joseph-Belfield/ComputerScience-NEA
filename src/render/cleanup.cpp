#include "render.hpp"
#include "defines/appData.hpp"
#include "defines/objectData.hpp"

// *************************************************

#include "SDL3/SDL.h"           // SDL main library
#include "SDL3/SDL_opengl.h"    // SDL OpenGL integration
 
#include "imgui.h"                              // ImGui main library
#include "backends/imgui_impl_sdl3.h"           // ImGui SDL integration
#include "backends/imgui_impl_opengl3.h"        // ImGui OpenGL integration

// *************************************************

namespace render
{
    void clean_ImGui()
    {
    // shuts down ImGui + OpenGL link
    ImGui_ImplOpenGL3_Shutdown();

    // shuts down ImGui + SDL link
    ImGui_ImplSDL3_Shutdown();

    // destroys ImGui context
    ImGui::DestroyContext();
    }


    void clean_SDL(appData &appData, objectData &objectData1, objectData &objectData2)
    {

    // destroys SDL window
    SDL_DestroyWindow(appData.program.window);
    appData.program.window = nullptr;

    // delete OpenGL objects
    glDeleteBuffers(1, &(objectData1.mesh.vertexBufferObject));
    glDeleteVertexArrays(1, &(objectData1.mesh.vertexArrayObject));

    glDeleteBuffers(1, &(objectData2.mesh.vertexBufferObject));
    glDeleteVertexArrays(1, &(objectData2.mesh.vertexArrayObject));

    // delete graphics pipeline
    glDeleteProgram(appData.program.shaderProgram);

    // quits SDL
    SDL_Quit();
    }
}