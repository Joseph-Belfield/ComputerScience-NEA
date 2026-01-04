#ifndef SDL
  #include "SDL3/SDL.h"
  #include "SDL3/SDL_render.h"
  #include "SDL3/SDL_video.h"
  #define SDL
#endif

auto init_SDL(SDL_Window **window, SDL_Renderer **renderer) -> auto {
  if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
    return false;
  }

  bool success = SDL_CreateWindowAndRenderer("compsci_nea", 800, 600, SDL_WINDOW_RESIZABLE, window, renderer);

  if (success == false)
  {
    SDL_Log("errrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr");
    return false;
  }
  return true;
}