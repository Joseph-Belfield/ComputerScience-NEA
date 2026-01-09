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

namespace transform
{
  // model matrices move objects from their 'local' space and positions it in the 'world' space
  void model_matrix()
  {
    // create and adapt the matrix to adjust the following transformations
    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, global::uOffset)); // movement
    modelMatrix = glm::rotate(modelMatrix ,glm::radians(global::uRotate), glm::vec3(0.0f, 1.0f, 0.0f));  // rotations
    modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0f, 1.0f, 1.0f));

    GLuint uLocation_modelMatrix = glGetUniformLocation(global::shaderProgram, "uModelMatrix"); // get location of uniform variable
    if (uLocation_modelMatrix >= 0)   // if that space has been allocated correctly
    {
      glUniformMatrix4fv             // create a uniform matrix
      (
      uLocation_modelMatrix,         // location
      1,                             // count 
      false,                         // transpose (bool)
      &modelMatrix[0][0]               // value
      );
    }
    else                              // else throw an error
    {
      SDL_Log("Could not find uniform location!");
      SDL_Log("Check spelling.");
      exit(-1);
    }
  }


  void perspective_matrix()
  {
    // projection matrix (in perspective)
    glm::mat4 perspective = glm::perspective                                             // create perspective matrix
                            (
                              45.0f,                                                     // FOV (radians)
                              (float)(global::window_width / global::window_height),     // aspect ratio
                              0.1f,                                                      // near clipping plane (min. distance)
                              10.0f                                                      // far clipping plane (max. distance)
                            );         

    GLuint uLocation_perspective = glGetUniformLocation(global::shaderProgram, "uPerspective");    // get location of uniform variable
    if (uLocation_perspective >= 0)   // if that space has been allocated correctly
    {
      glUniformMatrix4fv             // create a uniform matrix
      (
      uLocation_perspective,         // location
      1,                             // count 
      false,                         // transpose (bool)
      &perspective[0][0]             // value
      );
    }
    else                              // else throw an error
    {
      SDL_Log("Could not find uniform location!");
      SDL_Log("Check spelling.");
      exit(-1);
    }
  }
}

namespace runtime
{
  // for setting OpenGL state
  void preDraw_OpenGL()
  {
    // disables
    glDisable(GL_DEPTH_TEST); // disables depth check - 2D scene
    glDisable(GL_CULL_FACE);  // disables checking for overlap - 2D scene

    // set size of window for OpenGL
    glViewport(0, 0, (int)global::window_width, (int)global::window_height);

    // background color
    glClearColor(0.0f, 1.0f, 1.0f, 1.0f);                   // sets background color
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);     // clears the OpenGL color and depth buffers

    glUseProgram(global::shaderProgram);            // selects program in use

    // transformation matrices
    transform::model_matrix();         // controls position and rotation on world axis
    transform::perspective_matrix();   // creats illusion of perspective (size changes relative to camera)

  }
}