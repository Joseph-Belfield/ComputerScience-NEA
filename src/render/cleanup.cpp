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


    void clean_SDL(appData &appData)
    {

    // destroys SDL window
    SDL_DestroyWindow(appData.program.window);
    appData.program.window = nullptr;


    // delete OpenGL objects
    for (int i = 0; i < appData.window.objects.size(); i++)
    {
        glDeleteBuffers(1, &(appData.window.objects[i] -> mesh.vertexBufferObject));
        glDeleteVertexArrays(1, &(appData.window.objects[i] -> mesh.vertexArrayObject));
    }

    // delete graphics pipeline
    for (int i = 0; i < appData.shader.size(); i++)
    {
        glDeleteProgram(appData.shader[i] -> programID);
    }

    // quits SDL
    SDL_Quit();
    }
}