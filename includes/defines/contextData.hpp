#pragma once

// include libraries for basic types that cannot be forward declared (ie: things other than structs, classes, enums)
#include "glad/gl.h"
#include "SDL3/SDL.h"
#include "imgui/imgui.h"
#include "glm/vec3.hpp"

#include "defines/camera.hpp"

#include <vector>

// forward declarations
struct SDL_Window;


struct programData
{
  // the window the program runs in
  SDL_Window* window = nullptr;

  // the OpenGL context for the window 
  SDL_GLContext context_OpenGL = nullptr;

  // main loop flag
  bool flag_mainLoop = true;

  // sets GLSL version (matches OpenGL version)
  const char* version_glsl = "#version 410";

  // unique ID for the graphics pipeline
  GLuint shaderProgram = 0;
};

struct windowData
{
  // the main scale of the program. relative to display size
  float mainScale;

  // screen dimensions
  int window_height;
  int window_width;

  // background color
  ImVec4 clearColor = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
};

class ImGuiData
{
public:

  bool show_mainWindow = false;
  bool show_helloWorld = false;
  bool show_colorPicker = false;
  bool show_sineGraph = false;
  bool show_scrolling = false;
};

struct cameraData
{
  Camera camera1;
  float sense = 0.5f;
};

struct meshData
{
  // unsigned ints as identifiers for the objects (because C-based language)
  GLuint vertexArrayObject = 0;
  GLuint vertexBufferObject = 0;
  GLuint indexBufferObject = 0;

  GLuint shaderProgram = 0;

  // use of GLfloat as it is more cross-platform (likely won't matter but best practice)
  const std::vector<GLfloat> vertexData   // lives on CPU
  {
    -0.5f, -0.5f,  0.5f,    // vertex 0 - (front bottom left)
    1.0f,  0.0f,  0.0f,    // color

    0.5f, -0.5f,  0.5f,    // vertex 1 - (front bottom right)
    0.0f,  0.0f,  1.0f,    // color

    -0.5f,  0.5f,  0.5f,    // vertex 2 - (front top left)
    1.0f,  0.0f,  0.0f,    // color   

    0.5f,  0.5f,  0.5f,    // vertex 3 - (front top right)
    0.0f,  0.0f,  1.0f,    // color



    -0.5f, -0.5f, -0.5f,    // vertex 4 - (back bottom left)
    1.0f,  0.0f,  0.0f,    // color

    0.5f, -0.5f, -0.5f,    // vertex 5 - (back bottom right)
    0.0f,  0.0f,  1.0f,    // color

    -0.5f,  0.5f, -0.5f,    // vertex 6 - (back top left)
    1.0f,  0.0f,  0.0f,    // color   

    0.5f,  0.5f, -0.5f,    // vertex 7 - (back top right)
    0.0f,  0.0f,  1.0f,    // color
  };

  // data about the order vertices should be renderd in
  const std::vector<GLuint> indexData
  {
    // face 1 - front
    2, 0, 1,      // triangle 1
    3, 2, 1,      // triangle 2


    // face 2 - back
    7, 5, 4,
    4, 6, 7,

    //face 3 - left
    6, 4, 0,
    0, 2, 6,

    //face 4 - right
    3, 1, 5,
    5, 7, 3,

    // face 5 - top
    7, 6, 2,
    2, 3, 7,

    //face 6 - bottom
    4, 0, 1,
    1, 5, 4
  };  
};

struct uniformData
{
  // offset that allows us to change values in the GPU

  // object's displacement from origin (effectively coords on world axis)
  glm::vec3 uDisplacement = glm::vec3(0.0f, 0.0f, 0.0f);

  // object's rotation around each axis
  glm::vec3 uRotate = glm::vec3(0.0f, 0.0f, 0.0f);

  // growth of the object in each direction
  glm::vec3 uScale = glm::vec3(1.0f, 1.0f, 1.0f);
};

// for the whole program
struct appData
{
  programData program;
  windowData window;
  ImGuiData ImGui;
  cameraData camera;
};

// for each induvidual object
struct objectData
{
  meshData mesh;
  uniformData uniform;
};