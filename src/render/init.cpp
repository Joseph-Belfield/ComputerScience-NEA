#include "render.hpp"
#include "defines/contextData.hpp"

// *************************************************

#include "glad/gl.h"            // OpenGL extension loader

#include "SDL3/SDL.h"           // SDL main library

 
#include "imgui.h"                              // ImGui main library
#include "backends/imgui_impl_sdl3.h"           // ImGui SDL integration
#include "backends/imgui_impl_opengl3.h"        // ImGui OpenGL integration

// *************************************************

#include <stdlib.h>
#include <iostream>

// *************************************************

namespace render
{
    void init_SDL(appData &appData)
    {
        // initialize SDL
        if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) 
        {
            std::cout << "Failed to initialize SDL." << std::endl;
            exit(-1); 
        }

        // finds the scale of the display 
        appData.display.mainScale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());

        // flags for the window
        SDL_WindowFlags window_flags =  SDL_WINDOW_RESIZABLE |          // lets window be resized
                                        SDL_WINDOW_OPENGL |             // window uses OpenGL context
                                        SDL_WINDOW_HIDDEN |             // window hidden during setup
                                        SDL_WINDOW_HIGH_PIXEL_DENSITY;  // higher quality


        // creates a window assigned to 'window', errors if failed
        appData.window.window = SDL_CreateWindow("compsci_nea", round(1280 * appData.display.mainScale), round(800 * appData.display.mainScale), window_flags);

        // checks if window has been created properly
        if (appData.window.window == nullptr) 
        {
            std::cout << "Failed to create window." << std::endl;
            exit(-1); 
        }
    }



    void set_OpenGL_Attributes()
    {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);

        // sets the OpenGL version (4.1)
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

        // sets the type of OpenGL context (SDL)
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        // enables double buffer
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

        // allows 24 bits for the color
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

        // allows 8 bits as minimum in stencil (allows for use of stencil)
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    }



    void init_OpenGL(appData &appData)
    {
        // create the context for OpenGL in 'window'
        appData.window.context_OpenGL = SDL_GL_CreateContext(appData.window.window);

        // checks context has been created properly
        if (appData.window.context_OpenGL == nullptr) 
        {
            std::cout << "Failed to create OpenGL context." << std::endl;
            exit(-1); 
        }
        
        // initializes GLAD
       
        if (gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress) == 0)
        {
            std::cout << "Error loading GLAD!" << std::endl;
            exit(-1);
        }

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  // sets whether gl should fill polygons or render just the lines

        SDL_GL_MakeCurrent(appData.window.window, appData.window.context_OpenGL); // sets current window and context
        SDL_GL_SetSwapInterval(1); // Enable vsync
        SDL_SetWindowPosition(appData.window.window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED); // centers window
        SDL_ShowWindow(appData.window.window);  // reveals window once program has been initialized

    }



    void init_ImGui(appData &appData)
    {
        // creates ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        // ImGui Inputs/Outputs
        ImGuiIO &io = ImGui::GetIO();  (void)io; // initializes io

        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls

        if (!SDL_GetWindowSizeInPixels(appData.window.window, &(appData.display.window_width), &(appData.display.window_height)))
        {
            std::cout << "Failed to get window size!" << std::endl;
            exit(-1);
        }
        io.DisplaySize = ImVec2((float)(appData.display.window_width), (float)(appData.display.window_height));

        ImGui::StyleColorsDark();
        ImGuiStyle &style = ImGui::GetStyle();

        // Eall scales in file scale around this. errors with style to do with size are probably this.
        if (appData.display.mainScale > 1.0f)
        {
            style.ScaleAllSizes(appData.display.mainScale); 
        /*  Bake a fixed style scale. (until we have a solution for dynamic style scaling, 
            changing this requires resetting Style + calling this again) makes this unnecessary. 
            We leave both here for documentation purpose) */
        }
        else
        {
            // protection so program doesn't error
            style.ScaleAllSizes(1.0f);
        }


        // sets a base style for the fonts
        style.FontSizeBase = 20.0f * appData.display.mainScale;

        io.Fonts -> AddFontDefault();
        ImFont* Arimo_Regular = io.Fonts -> AddFontFromFileTTF("fonts/Arimo-Regular.ttf", 20.0f);
        ImFont* Roboto_SemiCondensed_Italic = io.Fonts -> AddFontFromFileTTF("fonts/Roboto_SemiCondensed-Italic.ttf", 20.0f);

        ImGui_ImplSDL3_InitForOpenGL(appData.window.window, appData.window.context_OpenGL);
        ImGui_ImplOpenGL3_Init(appData.OpenGL.version_glsl);
    }
}
