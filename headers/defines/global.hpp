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

// ********************** GLOBAL VARRIABLES **********************
namespace global
{

  // the window the program runs in
  SDL_Window* window = nullptr;

  // the OpenGL context for the window 
  SDL_GLContext context_OpenGL = nullptr;

  // sets GLSL version (matches OpenGL version)
  const char* version_glsl;

  // the main scale of the program. relative to display size
  float mainScale;

  // screen dimensions
  int window_height;
  int window_width;

  // main loop flag
  bool flag_mainLoop = true;

  // unsigned ints as identifiers for the objects (because C-based language)
  GLuint vertexArrayObject = 0;
  GLuint vertexBufferObject = 0;
  GLuint indexBufferObject = 0;

  // unique ID for the graphics pipeline
  GLuint shaderProgram = 0;

  // offset that allows us to change values in the GPU
  float uOffset = -2.0f;
  float uRotate;

  // matrix transformations
  float uModelMatrix;         // matrix that transforms shapes position on world axis
  float uPerspective;         // matrix that creates perspective (shows movement on Z plane)
}