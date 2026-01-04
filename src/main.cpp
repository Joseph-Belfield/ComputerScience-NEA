// *************************************************

// added error protetion around includes. library includes grouped.
#ifndef MATH
  #include <math.h>
  #define MATH
#endif

// *************************************************

#ifndef SDL
  #include "SDL3/SDL.h"
  #include "SDL3/SDL_render.h"
  #include "SDL3/SDL_video.h"
  #define SDL
#endif

#ifndef IMGUI
  #include "imgui.h"
  #include "backends/imgui_impl_sdl3.h"
  #include "backends/imgui_impl_sdlrenderer3.h"
  #define IMGUI
#endif

// *************************************************

#ifndef INIT_SDL
  #include "init/init_SDL.hpp"
  #define INIT_SDL
#endif

#ifndef INIT_IMGUI
  #include "init/init_ImGui.hpp"
  #define INIT_IMGUI
#endif

// *************************************************

#ifndef ATOM
  #include "defines/atom.hpp"
  #define ATOM
#endif

#ifndef ELEMENT
  #include "defines/elements.hpp"
  #define ELEMENT
#endif

// *************************************************

int main(int argc, char *argv[]) {

  // **************************************************************
  //  ____       _
  // / ___|  ___| |_ _   _ _ __
  // \___ \ / _ \ __| | | | '_ \
  //  ___) |  __/ |_| |_| | |_) |
  // |____/ \___|\__|\__,_| .__/
  //                      |_|
  // **************************************************************

  SDL_Window *window;
  SDL_Renderer *renderer;

  if (init_SDL(&window, &renderer) == false) {
    return -1;
  }

  init_imgui(window, renderer);

  // initialize variables used inside the loop here!

  // window background color variables
  float my_color[4];
  my_color[3] = 1.0f;

  // bools determining if windows should be rendered
  bool main_window = true;
  bool hello_world = false;
  bool show_colorPicker = false;
  bool show_sineGraph = false;
  bool show_scrolling = false;

  /* **************************************************************
  //  _
  // | | ___   ___  _ __
  // | |/ _ \ / _ \| '_ \
  // | | (_) | (_) | |_) |
  // |_|\___/ \___/| .__/
  //               |_|
  // ************************************************************** */

  bool should_run{true};

  while (should_run) {
    SDL_Event e;

    if (SDL_PollEvent(&e)) {

      ImGui_ImplSDL3_ProcessEvent(&e);
      if (e.type == SDL_EVENT_QUIT) {
        should_run = false;
      }
      if (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_ESCAPE) {
        should_run = false;
      }
    }

    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    // ********************** DO STUFF HERE **********************

    ImGui::Begin("Main Window", &main_window, ImGuiWindowFlags_MenuBar);

    if (ImGui::BeginMenuBar())
    {
      if (ImGui::BeginMenu("Options"))
      {
        if (ImGui::MenuItem("Change Background Color")) {show_colorPicker = true;}
        if (ImGui::MenuItem("Sine Graph")) {show_sineGraph = true;}
        if (ImGui::MenuItem("Scrolling")) {show_scrolling = true;}
        ImGui::EndMenu();
      }
      ImGui::EndMenuBar();
    }

    if (ImGui::Button("Hello World"))
    {
      hello_world = true;
    }

    ImGui::End();



    if (hello_world)
    {
      ImGui::Text("Hello World!");
    }

    if (show_sineGraph)
    {
      ImGui::Begin("Sine Graph", &show_sineGraph);
      float samples[100];
      for (int i = 0; i < 100; i++)
      {
        samples[i] = sinf(i * 0.2f + ImGui::GetTime() * 1.5f);
      }
      ImGui::PlotLines("Samples", samples, 100);
      ImGui::End();
    }

    if (show_scrolling)
    {
      // display contents in scrolling region
      ImGui::Begin("Scrolling", &show_scrolling);
      ImGui::TextColored(ImVec4(1,1,0,1), "Important Stuff");
      ImGui::BeginChild("Scrolling");
      for (int i = 0; i < 15; i++)
      {
        ImGui::Text("Some text");
      }
      ImGui::EndChild();
      ImGui::End();
    }

    if (show_colorPicker)
    {
      // create a window with menu bar called "Color Picker"
      ImGui::Begin("Color Picker", &show_colorPicker);
      // edit a color stored as 4 floats
      ImGui::ColorEdit4("Color", my_color);
      ImGui::End();
    }

    // ********************** STOP DOING STUFF HERE **********************


    ImGui::Render();
    SDL_SetRenderDrawColor(renderer, round(255 * my_color[0]), round(255 * my_color[1]), round(255 * my_color[2]), round(255 * my_color[3]));
    SDL_RenderClear(renderer);
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
    SDL_RenderPresent(renderer);
  }

  // **************************************************************

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}

// enter "sh make.sh" into terminal to run program