#include "render.hpp"
#include "defines/contextData.hpp"

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
    // destroys SDL context 
    SDL_GL_DestroyContext(appData.window.context_OpenGL);

    // destroys SDL window
    SDL_DestroyWindow(appData.window.window);

    // quits SDL
    SDL_Quit();
    }
}