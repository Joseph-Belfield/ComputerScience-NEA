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
  void draw_ImGui()
  {
    ImGui::Begin("Main Window", &(global::show_mainWindow), ImGuiWindowFlags_MenuBar);  

    if (ImGui::BeginMenuBar())
    {
      if (ImGui::BeginMenu("Options"))
      {
        if (ImGui::MenuItem("Change Background Color")) {global::show_colorPicker = true;}
        if (ImGui::MenuItem("Sine Graph")) {global::show_sineGraph = true;}
        if (ImGui::MenuItem("Scrolling")) {global::show_scrolling = true;}
        ImGui::EndMenu();
      }
      ImGui::EndMenuBar();
    }

    if (ImGui::Button("Hello World"))
    {
      global::show_helloWorld = true;
    }

    ImGui::End();



    if (global::show_helloWorld)
    {
      ImGui::Text("Hello World!");
    }

    if (global::show_sineGraph)
    {
      ImGui::Begin("Sine Graph", &(global::show_sineGraph));
      float samples[100];
      for (int i = 0; i < 100; i++)
      {
        samples[i] = sinf(i * 0.2f + ImGui::GetTime() * 1.5f);
      }
      ImGui::PlotLines("Samples", samples, 100);
      ImGui::End();
    }

    if (global::show_scrolling)
    {
      // display contents in scrolling region
      ImGui::Begin("Scrolling", &(global::show_scrolling));
      ImGui::TextColored(ImVec4(1,1,0,1), "Important Stuff");
      ImGui::BeginChild("Scrolling");
      for (int i = 0; i < 15; i++)
      {
        ImGui::Text("Some text");
      }
      ImGui::EndChild();
      ImGui::End();
    }

    if (global::show_colorPicker)
    {
      // create a window with menu bar called "Color Picker"
      ImGui::Begin("Background Color", &(global::show_colorPicker));
      // edit a color stored as 4 floats
      ImGui::ColorEdit3("Color", (float*)&global::clearColor);
      ImGui::End();
    }
  }
}