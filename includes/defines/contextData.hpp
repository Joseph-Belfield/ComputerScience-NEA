#pragma once

// include libraries for basic types that cannot be forward declared (ie: things other than structs, classes, enums)
#include "glad/gl.h"
#include "imgui/imgui.h"
#include "SDL3/SDL.h"
#include "defines/camera.hpp"

// forward declarations
struct SDL_Window;


struct windowData
{
  // the window the program runs in
  SDL_Window* window = nullptr;

  // the OpenGL context for the window 
  SDL_GLContext context_OpenGL = nullptr;

  // main loop flag
  bool flag_mainLoop = true;

};

struct displayData
{
  // the main scale of the program. relative to display size
  float mainScale;

  // screen dimensions
  int window_height;
  int window_width;

  // background color
  ImVec4 clearColor = ImVec4(0.0f, 0.5f, 0.5f, 1.0f);

};

struct OpenGLData
{

  // unsigned ints as identifiers for the objects (because C-based language)
  GLuint vertexArrayObject = 0;
  GLuint vertexBufferObject = 0;
  GLuint indexBufferObject = 0;

  // unique ID for the graphics pipeline
  GLuint shaderProgram = 0;

  // sets GLSL version (matches OpenGL version)
  const char* version_glsl = "#version 410";

};

struct uniformData
{
  // offset that allows us to change values in the GPU
  float uOffset = -2.0f;
  float uDisplacement[2] = {0.0f, 0.0f};
  float uRotate;
  float uScale = 1.0f;

  // matrix transformations
  float uModelMatrix;         // Matrix that transforms shapes position on world axis.
  float uViewMatrix;          // Matrix that moves shapes into camera space.
  float uPerspective;         // Matrix that creates perspective (shows movement on Z plane).
};

struct ImGuiData
{
  // bools determining if windows should be rendered  
  bool show_mainWindow;
  bool show_helloWorld = false;
  bool show_colorPicker = false;
  bool show_sineGraph = false;
  bool show_scrolling = false;  

};

struct cameraData
{
  Camera camera1;
};


struct appData
{
  windowData window;
  displayData display;
  OpenGLData OpenGL;
  ImGuiData ImGui;
  uniformData uniform;
  cameraData camera;
};