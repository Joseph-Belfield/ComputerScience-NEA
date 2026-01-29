#pragma once

// include libraries for basic types that cannot be forward declared (ie: things other than structs, classes, enums)
#include "glad/gl.h"
#include "SDL3/SDL.h"
#include "imgui/imgui.h"
#include "glm/vec3.hpp"

#include "defines/render/camera.hpp"
#include "defines/render/shaderClass.hpp"

#include <string>
#include <vector>

// forward declarations
struct SDL_Window;
class Object;


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

  // holds a pointer to the default shader object for the program
  Shader defaultShader;
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

  std::vector<Object*> objects;
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
  GLfloat sense = 0.5f;
  GLfloat speed = 0.5f;
};

struct eventData
{
  // checks for events
  SDL_Event event;

  // allows to check if multiple keys are pressed at once, faster refresh
  const bool* keyState;
};


// for the whole program
struct appData
{
	programData program;
	windowData window;
	ImGuiData ImGui;
	cameraData camera;
  eventData event;
};

