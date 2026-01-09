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

// ********************** DRAW **********************

#ifndef DRAW
  #include "render/draw/events.hpp"
  #include "render/draw/predraw.hpp"
  #include "render/draw/draw_ImGui.hpp"
  #include "render/draw/draw_OpenGL.hpp"
  #define DRAW
#endif

// *************************************************

void run_loop()
{
  // window background color variables
  ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);

  // bools determining if windows should be rendered  
  bool show_mainWindow;
  bool show_helloWorld = false;
  bool show_colorPicker = false;
  bool show_sineGraph = false;
  bool show_scrolling = false;

  // ********************** RUN LOOP **********************

  while (global::flag_mainLoop) {

    runtime::check_events();

    // starts a new frame for OpenGL, SDL and ImGui
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    // ********************** DO STUFF HERE **********************

    runtime::draw_ImGui(&show_mainWindow, &show_colorPicker, &show_sineGraph, &show_scrolling, &show_helloWorld);

    runtime::preDraw_OpenGL();
    runtime::draw_OpenGL();


    // render
    ImGui::Render();                                               // renders ImGui instructions 
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());        // renders the ImGui data with OpenGL  
    SDL_GL_SwapWindow(global::window);                                     // swaps in the new frame

  }
}