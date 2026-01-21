#include "runLoop.hpp"

#include "defines/contextData.hpp"

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

        // checks for events
        SDL_Event event;

        // if there were events, do:
        while (SDL_PollEvent(&event)) {

            // ImGui processes the event
            ImGui_ImplSDL3_ProcessEvent(&event);

            // if SDL is quit, end the run loop
            if (event.type == SDL_EVENT_QUIT) 
            {
                appData.program.flag_mainLoop = false;
            }
            // if Esc key is pressed, end the run loop
            if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_ESCAPE) 
            {
                appData.program.flag_mainLoop = false;
            }

            // open/close main menu
            if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_TAB)
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
                if (event.type == SDL_EVENT_MOUSE_MOTION)
                {
                    // relative changes in position from centre each frame monitored
                    mouseX += event.motion.xrel;
                    mouseY += event.motion.yrel;
                    appData.camera.camera1.mouseLook(mouseX, mouseY, appData);
                }
            }

            // movement
            if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_W)
            {
                appData.camera.camera1.move_forward(1.0f);
            }
            if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_S)
            {
                appData.camera.camera1.move_backward(1.0f);
            }
            if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_A)
            {
                appData.camera.camera1.move_left(1.0f);
            }
            if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_D)
            {
                appData.camera.camera1.move_right(1.0f);
            }
             if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_O)
            {
                appData.camera.camera1.move_up(1.0f);
            }
            if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_P)
            {
                appData.camera.camera1.move_down(1.0f);
            }

        }
    }