#include "render/render.hpp"

#include "render/runLoop.hpp"
#include "appData.hpp"
#include "render/objectData.hpp"

#include "SDL3/SDL.h"           // SDL main library
#include "SDL3/SDL_opengl.h"    // SDL OpenGL integration

#include "imgui.h"                              // ImGui main library
#include "backends/imgui_impl_sdl3.h"           // ImGui SDL integration
#include "backends/imgui_impl_opengl3.h"        // ImGui OpenGL integration

namespace render
{
  
    void run_loop(appData &appData)
    {

        // start program with mouse in centre of window
        SDL_WarpMouseInWindow(appData.program.window, 400.0f, 300.0f);

        // allows to check if multiple keys are pressed at once, faster refresh
        appData.event.keyState = SDL_GetKeyboardState(nullptr);

        while (appData.program.flag_mainLoop) {

            check_events(appData);

            // starts a new frame for OpenGL, SDL and ImGui
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplSDL3_NewFrame();
            ImGui::NewFrame();

            // ********************** DO STUFF HERE **********************            

            appData.camera.camera1.update_deltaTime();

            update_scene(appData);

            draw_objects(appData.window.objects, &appData.camera.camera1, appData.window.window_width, appData.window.window_height);

            appData.molecule -> init_drawInfo(&appData.camera.camera1, appData.window.window_width, appData.window.window_height);
            appData.molecule -> draw(appData.displayedMolecule);
            draw_ImGui(appData);

            if (appData.ImGui.check_false())
            {
                SDL_WarpMouseInWindow(appData.program.window, 400.0f, 300.0f);
                SDL_SetWindowRelativeMouseMode(appData.program.window, true);
            }
            else
            {
                SDL_SetWindowRelativeMouseMode(appData.program.window, false);
            }

            // render
            ImGui::Render();                                               // renders ImGui instructions 
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());        // renders the ImGui data with OpenGL  
            SDL_GL_SwapWindow(appData.program.window);                      // swaps in the new frame

        }
    }
}