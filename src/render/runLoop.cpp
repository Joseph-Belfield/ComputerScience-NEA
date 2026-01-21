#include "render.hpp"

#include "runLoop.hpp"
#include "defines/contextData.hpp"

#include "SDL3/SDL.h"           // SDL main library
#include "SDL3/SDL_opengl.h"    // SDL OpenGL integration

#include "imgui.h"                              // ImGui main library
#include "backends/imgui_impl_sdl3.h"           // ImGui SDL integration
#include "backends/imgui_impl_opengl3.h"        // ImGui OpenGL integration

namespace render
{
  
    void run_loop(appData &appData, objectData &objectData)
    {

        // start program with mouse in centre of window
        SDL_WarpMouseInWindow(appData.program.window, (appData.window.window_width / 2), (appData.window.window_height / 2));

        // hides cursor, mouse position is constrained to window
        SDL_SetWindowRelativeMouseMode(appData.program.window, true);

        while (appData.program.flag_mainLoop) {

            check_events(appData);

            // starts a new frame for OpenGL, SDL and ImGui
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplSDL3_NewFrame();
            ImGui::NewFrame();

            // ********************** DO STUFF HERE **********************            

            update_scene(appData);
            update_mesh(objectData);
            draw_mesh(objectData);

            draw_ImGui(appData);

            objectData.uniform.uRotate += 0.5f;

            // render
            ImGui::Render();                                               // renders ImGui instructions 
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());        // renders the ImGui data with OpenGL  
            SDL_GL_SwapWindow(appData.program.window);                      // swaps in the new frame

        }
    }
}