#include "render/runLoop.hpp"

#include "appData.hpp"
#include "render/objectData.hpp"

#include "SDL3/SDL.h"           // SDL main library
#include "SDL3/SDL_opengl.h"    // SDL OpenGL integration

#include "imgui.h"                              // ImGui main library
#include "backends/imgui_impl_sdl3.h"           // ImGui SDL integration
#include "backends/imgui_impl_opengl3.h"        // ImGui OpenGL integration

void check_events(appData &appData)
    {
        // sets mouse position as centre of window when program run initially
        static int mouseX = appData.window.window_width / 2;
        static int mouseY = appData.window.window_height / 2;

        // refreshes event check each frame (includes SDL pump events)
        while (SDL_PollEvent(&appData.event.event)) {

            // ImGui processes the event
            ImGui_ImplSDL3_ProcessEvent(&appData.event.event);

            // if SDL is quit, end the run loop
            if (appData.event.event.type == SDL_EVENT_QUIT) 
            {
                appData.program.flag_mainLoop = false;
            }
            // if Esc key is pressed, end the run loop
            if (appData.event.event.type == SDL_EVENT_KEY_DOWN && appData.event.event.key.key == SDLK_ESCAPE) 
            {
                appData.program.flag_mainLoop = false;
            }

            // open/close main menu
            if (appData.event.event.type == SDL_EVENT_KEY_DOWN && appData.event.event.key.key == SDLK_TAB)
            {
                // pressing tab flips the bool state, either revealing or hiding it
                appData.ImGui.show_mainWindow = !appData.ImGui.show_mainWindow;

                if (!(appData.ImGui.show_mainWindow || appData.ImGui.show_colorPicker))
                {
                    SDL_WarpMouseInWindow(appData.program.window, (appData.window.window_width / 2), (appData.window.window_height / 2));
                    SDL_SetWindowRelativeMouseMode(appData.program.window, true);
                }
                else
                {
                    SDL_SetWindowRelativeMouseMode(appData.program.window, false);
                }
            }

            if (!(appData.ImGui.show_mainWindow || appData.ImGui.show_colorPicker))
            {
                // view direction
                if (appData.event.event.type == SDL_EVENT_MOUSE_MOTION)
                {
                    // relative changes in position from centre each frame monitored
                    mouseX += appData.event.event.motion.xrel;
                    mouseY += appData.event.event.motion.yrel;
                    appData.camera.camera1.mouseLook(mouseX, mouseY, appData);
                }
            }
        }

        // movement
            if (appData.event.keyState[SDL_SCANCODE_W])
            {
                appData.camera.camera1.move_forward(appData.camera.speed);
            }
            if (appData.event.keyState[SDL_SCANCODE_S])
            {
                appData.camera.camera1.move_backward(appData.camera.speed);
            }
            if (appData.event.keyState[SDL_SCANCODE_A])
            {
                appData.camera.camera1.move_left(appData.camera.speed);
            }
            if (appData.event.keyState[SDL_SCANCODE_D])
            {
                appData.camera.camera1.move_right(appData.camera.speed);
            }
             if (appData.event.keyState[SDL_SCANCODE_O])
            {
                appData.camera.camera1.move_up(appData.camera.speed);
            }
            if (appData.event.keyState[SDL_SCANCODE_P])
            {
                appData.camera.camera1.move_down(appData.camera.speed);
            }
    }