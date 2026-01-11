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

#ifndef ATOM
  #include "defines/chemistry/atom.hpp"
  #define ATOM
#endif

#ifndef ELEMENT
  #include "defines/chemistry/elements.hpp"
  #define ELEMENT
#endif

// ********************** GLOBAL VARRIABLES **********************

#ifndef GLOBAL
  #include "defines/global.hpp"
  #define GLOBAL
#endif

// ********************** WINDOW **********************

#ifndef INIT
  #include "render/init.hpp"
  #define INIT
#endif

#ifndef VERTEXSPEC
  #include "render/vertexSpecification.hpp"
  #define VERTEXSPEC
#endif

#ifndef GRAPHICS_PIPELINE
  #include "render/graphicsPipeline.hpp"
  #define GRAPHICS_PIPELINE
#endif

// ********************** DRAW **********************

#ifndef DRAW
  #include "render/runtime/events.hpp"
  #include "render/runtime/predraw.hpp"
  #include "render/runtime/draw_ImGui.hpp"
  #include "render/runtime/draw_OpenGL.hpp"
  #define DRAW
#endif

#ifndef RUNTIME
  #include "render/run_loop.hpp"
  #define RUNTIME
#endif

// ********************** CLEAN **********************

#ifndef CLEAN
  #include "render/cleanup.hpp"
  #define CLEAN
#endif

// *************************************************

int main(int argc, char *argv[]) {

  // 1. initialize libraries
  init::init_SDL();
  init::set_OpenGL_Attributes();
  init::init_OpenGL();
  init::init_ImGui();

  // 2. set up geometry
  vertex_specification();

  // 3. set up shaders (at least, vertex and fragment)
  create_graphics_pipeline();

  // 4. main run loop
  run_loop();

  // 5. cleans up
  clean_ImGui();
  clean_SDL();  

  return 0;
}

// enter "sh make.sh" into terminal to run program