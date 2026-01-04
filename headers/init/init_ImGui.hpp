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

auto init_imgui(SDL_Window *window, SDL_Renderer *renderer) -> auto {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls

  // Setup scaling
  float main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
  ImGuiStyle &style = ImGui::GetStyle();
  style.ScaleAllSizes(
      main_scale); // Bake a fixed style scale. (until we have a
                   // solution for dynamic style scaling, changing this
                   // requires resetting Style + calling this again)
                   // makes this unnecessary. We leave both here for
                   // documentation purpose)
  style.FontScaleDpi = main_scale;

  // ImFont* Roboto_SemiCondensed_Italic = io.Fonts -> AddFontFromFileTTF("fonts/Roboto_SemiCondensed-Italic.ttf", 20.0f);
  ImFont* Arimo_Regular = io.Fonts -> AddFontFromFileTTF("fonts/Arimo-Regular.ttf", 20.0f);

  ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
  ImGui_ImplSDLRenderer3_Init(renderer);
}