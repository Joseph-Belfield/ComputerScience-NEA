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

// A namespace used for functions associated with the main run loop.
namespace runtime
{
  void check_events()
  {
    // checks for events
    SDL_Event event;

    // if there were events, do:
    while (SDL_PollEvent(&event)) {

      // ImGui processes the event
      ImGui_ImplSDL3_ProcessEvent(&event);

      // if SDL is quit, end the run loop
      if (event.type == SDL_EVENT_QUIT) 
      {
        global::flag_mainLoop = false;
      }
      // if Esc key is pressed, end the run loop
      if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_ESCAPE) 
      {
        global::flag_mainLoop = false;
      }

      if (event.key.key == SDLK_W)
      {
        global::uDisplacement[1]+=0.1f;
      }
      if (event.key.key == SDLK_A)
      {
        global::uDisplacement[0]-=0.1f;
      }
      if (event.key.key == SDLK_S)
      {
        global::uDisplacement[1]-=0.1f;
      }
      if (event.key.key == SDLK_D)
      {
        global::uDisplacement[0]+=0.1f;
      }

      if (event.key.key == SDLK_UP)
      {
        global::uOffset+=0.1f;
      }
      if (event.key.key == SDLK_DOWN)
      {
        global::uOffset-=0.1f;
      }

      if (event.key.key == SDLK_RIGHT)
      {
        global::uRotate+=5.0f;
      }
      if (event.key.key == SDLK_LEFT)
      {
        global::uRotate-=5.0f;
      }

      if (event.key.key == SDLK_P)
      {
        global::uScale+=0.1f;
      }
      if (event.key.key == SDLK_O)
      {
        global::uScale-=0.1f;
      }
    }
  }
}