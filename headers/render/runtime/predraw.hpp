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

#ifndef CAMERA
  #include "defines/camera.hpp"
  #define CAMERA
#endif

// *************************************************

// A namespace for functions involved with transformation matrices.
namespace transform
{
  // Creates a uniform matrix, and returns its GLuint ID.
  GLuint create_uniform_mat4(GLuint shaderProgram, std::string uniformName, int amount, bool enableTranspose, glm::mat4 matrix)
  {
    GLuint uniformLocation = glGetUniformLocation(shaderProgram, uniformName.c_str());
    if (uniformLocation >= 0)
    {
      glUniformMatrix4fv
      (
        uniformLocation,
        amount,
        enableTranspose,
        &matrix[0][0]
      );

      return uniformLocation;
    }
    else
    {
      SDL_Log("Could not find uniform location!");
      SDL_Log("Check spelling.");
      exit(-1);
    }
  }


  // Creates a model matrix.
  // - Objects begin in local space, where they are created on their own set of axis
  // - The model matrix moves objects from local space to world space, where objects are all held relative to one shared set of axis
  //
  // The model matrix is also edited accordingly to change an objects position/rotation in world space accordingly.
  void model_matrix()
  {
    // create and adapt the matrix to adjust the following transformations
    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(global::uDisplacement[0], global::uDisplacement[1], global::uOffset)); // movement
    modelMatrix = glm::rotate(modelMatrix ,glm::radians(global::uRotate), glm::vec3(0.0f, 1.0f, 0.0f));  // rotations
    modelMatrix = glm::scale(modelMatrix, glm::vec3(global::uScale, global::uScale, global::uScale));

    GLuint uLocation_modelMatrix = create_uniform_mat4(global::shaderProgram, "uModelMatrix", 1, false, modelMatrix);
  }

  // Creates a view matrix.
  // - The scene is viewed as if through a camera for the viewer.
  // - The view matrix rotates objects around the viewer to form the illusion of a a camera.
  void view_matrix()
  {
    glm::mat4 viewMatrix = global::camera.get_view_matrix();

    GLuint uLocation_viewMatrix = create_uniform_mat4(global::shaderProgram, "uViewMatrix", 1, false, viewMatrix);
  }


  // Creates a projection matrix.
  // - The projection matrix creates the illusion of perspective
  // - It does this by changing a point's coordinates according to distance from the camera (Z-value)
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

    GLuint uLocation_perpective = create_uniform_mat4(global::shaderProgram, "uPerspective", 1, false, perspective);
  }
}

// A namespace used for functions associated with the main run loop.
namespace runtime
{
  // Handles tasks that must be completed before draw:
  // - OpenGL preferences
  // - Sets glViewport
  // - Sets clear color (background color)
  // - Applies transformation matrices
  void preDraw_OpenGL()
  {
    // disables
    glDisable(GL_DEPTH_TEST); // disables depth check - 2D scene
    glDisable(GL_CULL_FACE);  // disables checking for overlap - 2D scene

    // set size of window for OpenGL
    glViewport(0, 0, (int)global::window_width, (int)global::window_height);

    // background color
    glClearColor(global::clearColor.x, global::clearColor.y, global::clearColor.z, global::clearColor.w);                   // sets background color
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);     // clears the OpenGL color and depth buffers

    glUseProgram(global::shaderProgram);            // selects program in use

    // transformation matrices
    transform::model_matrix();         // controls position and rotation on world axis
    transform::view_matrix();
    transform::perspective_matrix();   // creats illusion of perspective (size changes relative to camera)

  }
}