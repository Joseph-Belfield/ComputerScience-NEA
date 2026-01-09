// *************************************************

// added error protetion around includes. library includes grouped.

#ifndef GLAD
  #include "glad/glad.h"          // OpenGL extension loader
  #define GLAD
#endif

#ifndef SDL
  #include "SDL3/SDL.h"           // SDL main library
  #include "SDL3/SDL_opengl.h"    // SDL OpenGL integration
  #define SDL
#endif

#ifndef IMGUI
  #include "imgui.h"                              // ImGui main library
  #include "backends/imgui_impl_sdl3.h"           // ImGui SDL integration
  #include "backends/imgui_impl_opengl3.h"        // ImGui OpenGL integration
  #define IMGUI
#endif

#ifndef GLM
  #include "glm/glm.hpp"                       // main GLM library
  #include "glm/vec3.hpp"                      // GLM vec3 class and methods - for position data
  #include "glm/mat4x4.hpp"                    // GLM 4x4 matrix class and methods - for transformations 
  #include "glm/gtc/matrix_transform.hpp"      // translate function
  #define GLM
#endif

// *************************************************

#ifndef MATH
  #include <math.h>               // contains basic math functions (such as trig)
  #define MATH
#endif

#ifndef STRING
  #include <string>              // has standard input/output functions
  #define STRING
#endif

#ifndef FSTREAM
  #include <fstream>             // has standard input/output functions
  #define FSTREAM
#endif

#ifndef VECTOR
  #include <vector>              // has standard input/output functions
  #define VECTOR
#endif

// *************************************************

#ifndef GLOBAL
  #include "defines/global.hpp"        
  #define GLOBAL
#endif

// *************************************************

namespace init
{
    void init_SDL()
    {
    // initialize SDL
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) 
    {
        SDL_Log("Failed to initialize SDL.");
        exit(-1); 
    }
    

    // finds the scale of the display 
    global::mainScale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());

    // flags for the window
    SDL_WindowFlags window_flags =  SDL_WINDOW_RESIZABLE |          // lets window be resized
                                    SDL_WINDOW_OPENGL |             // window uses OpenGL context
                                    SDL_WINDOW_HIDDEN |             // window hidden during setup
                                    SDL_WINDOW_HIGH_PIXEL_DENSITY;  // higher quality


    // creates a window assigned to 'window', errors if failed
    global::window = SDL_CreateWindow("compsci_nea", (int)(1280 * global::mainScale), (int)(800 * global::mainScale), window_flags);

    // checks if window has been created properly
    if (global::window == nullptr) 
    {
        SDL_Log("Failed to create window.");
        exit(-1); 
    }
    }



    void set_OpenGL_Attributes()
    {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);

    // sets the OpenGL version (4.1)
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);


    // sets the GLSL version to fit the OpenGL version
    global::version_glsl = "#version 410";


    // sets the type of OpenGL context (SDL)
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // enables double buffer
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    // allows 24 bits for the color
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    // allows 8 bits as minimum in stencil (allows for use of stencil)
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    }



    void init_Opencontext_OpenGL()
    {
    // create the context for OpenGL in 'window'
    global::context_OpenGL = SDL_GL_CreateContext(global::window);

    // checks context has been created properly
    if (global::context_OpenGL == nullptr) 
    {
        SDL_Log("Failed to create OpenGL context.");
        exit(-1); 
    }


    // initializes GLAD
    gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);

    
    SDL_GL_MakeCurrent(global::window, global::context_OpenGL); // sets current window and context
    SDL_GL_SetSwapInterval(1); // Enable vsync
    SDL_SetWindowPosition(global::window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED); // centres window
    SDL_ShowWindow(global::window);  // reveals window once program has been initialized
    }



    void init_ImGui()
    {
    // creates ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    // ImGui Inputs/Outputs
    ImGuiIO &io = ImGui::GetIO();  (void)io; // initializes io

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls

    if (!SDL_GetWindowSizeInPixels(global::window, &global::window_width, &global::window_height))
    {
        SDL_Log("Failed to get window size!");
        exit(-1);
    }
    io.DisplaySize = ImVec2((float)global::window_width, (float)global::window_height);

    ImGui::StyleColorsDark();
    ImGuiStyle &style = ImGui::GetStyle();
    
    // Eall scales in file scale around this. errors with style to do with size are probably this.
    if (global::mainScale > 1.0f)
    {
        style.ScaleAllSizes(global::mainScale); 
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
    style.FontSizeBase = 20.0f * global::mainScale;

    io.Fonts -> AddFontDefault();
    ImFont* Arimo_Regular = io.Fonts -> AddFontFromFileTTF("fonts/Arimo-Regular.ttf", 20.0f);
    ImFont* Roboto_SemiCondensed_Italic = io.Fonts -> AddFontFromFileTTF("fonts/Roboto_SemiCondensed-Italic.ttf", 20.0f);

    ImGui_ImplSDL3_InitForOpenGL(global::window, global::context_OpenGL);
    ImGui_ImplOpenGL3_Init(global::version_glsl);
    }
}