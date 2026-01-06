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

// *************************************************

#ifndef MATH
  #include <math.h>               // contains basic math functions (such as trig)
  #define MATH
#endif

#ifndef STD_IO
  #include <stdio.h>              // has standard input/output functions
  #define STD_IO
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

  // creates SDL window
  SDL_Window *window;
  // creates OpenGL context (manages state machine)
  SDL_GLContext GL_context;
  // sets GLSL version (matches OpenGL version)
  const char* glsl_version;
  // finds the scale of the progran


 // ********************** INITIALIZE SDL & OPENGL **********************
  
  if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) 
  {
      SDL_Log("Failed to initialize SDL.");
      return -1; 
  }

  // ******************** OPENGL ATTRIBUTES *****************************

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);

  // sets the OpenGL version (4.1)
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

  // sets the GLSL version to fit the OpenGL version
  glsl_version = "#version 410";

  // sets the type of OpenGL context (SDL)
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  // enables double buffer
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  // allows 24 bits for the color
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

  // allows 8 bits as minimum in stencil (allows for use of stencil)
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

  // ********************* CREATE WINDOW ****************************
  
  float main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
  SDL_WindowFlags window_flags = SDL_WINDOW_RESIZABLE |          // lets window be resized
                                  SDL_WINDOW_OPENGL |             // window uses OpenGL context
                                  SDL_WINDOW_HIDDEN |             // window hidden during setup
                                  SDL_WINDOW_HIGH_PIXEL_DENSITY;  // higher quality

  // creates a window assigned to 'window', errors if failed
  window = SDL_CreateWindow("compsci_nea", (int)(1280 * main_scale), (int)(800 * main_scale), window_flags);

  // ********************* CREATE CONTEXT ****************************

  // create the context for OpenGL in 'window'
  GL_context = SDL_GL_CreateContext(window);

  gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);

  SDL_GL_MakeCurrent(window, GL_context); // sets current window and context
  SDL_GL_SetSwapInterval(1); // Enable vsync
  SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED); // centres window
  SDL_ShowWindow(window);  // reveals window once program has been initialized

  // ********************** INITIALIZE IMGUI **********************

  // creates ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  // ImGui Inputs/Outputs
  ImGuiIO &io = ImGui::GetIO();
  (void)io; // initializes io

  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls

  // finds the size of the window so window can be scaled correctly
  int width, height;
  SDL_GetWindowSizeInPixels(window, &width, &height);
  io.DisplaySize = ImVec2((float)width, (float)height);

  ImGuiStyle &style = ImGui::GetStyle();

  style.ScaleAllSizes(main_scale); 
  /*  Bake a fixed style scale. (until we have a solution for dynamic style scaling, 
      changing this requires resetting Style + calling this again) makes this unnecessary. 
      We leave both here for documentation purpose) */
  style.FontSizeBase = 20.0f;

  // io.Fonts -> AddFontDefault();
  ImFont* Arimo_Regular = io.Fonts -> AddFontFromFileTTF("fonts/Arimo-Regular.ttf", 20.0f);
  ImFont* Roboto_SemiCondensed_Italic = io.Fonts -> AddFontFromFileTTF("fonts/Roboto_SemiCondensed-Italic.ttf", 20.0f);

  ImGui_ImplSDL3_InitForOpenGL(window, GL_context);
  ImGui_ImplOpenGL3_Init(glsl_version);

  // ********************** INITIALIZE VARIABLES HERE **********************

  // window background color variables
  ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);

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

    // checks for events
    SDL_Event event;

    // ********************** EVENTS **********************

    // if there were events, do:
    while (SDL_PollEvent(&event)) {

      // ImGui processes the event
      ImGui_ImplSDL3_ProcessEvent(&event);

      // if SDL is quit, end the run loop
      if (event.type == SDL_EVENT_QUIT) {
        should_run = false;
      }
      // if Esc key is pressed, end the run loop
      if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_ESCAPE) {
        should_run = false;
      }
    }

    // starts a new frame for OpenGL, SDL and ImGui
    ImGui_ImplOpenGL3_NewFrame();
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
      ImGui::ColorEdit3("Background Color", (float*)&clear_color);
      ImGui::End();
    }

    // ********************** RENDERING STAGE **********************

    // renders ImGui instructions 
    ImGui::Render();

    // sets the OpenGL viewport (what you can see)
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);    

    // clears colors
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);

    // clears the OpenGL color buffer
    glClear(GL_COLOR_BUFFER_BIT);

    // renders the ImGui data with OpenGL
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // swaps in the new frame
    SDL_GL_SwapWindow(window);
  
  }

  // ********************** CLEANUP **********************

  // shuts down ImGui + OpenGL link
  ImGui_ImplOpenGL3_Shutdown();

  // shuts down ImGui + SDL link
  ImGui_ImplSDL3_Shutdown();

  // destroys ImGui context
  ImGui::DestroyContext();

  

  // destroys SDL context 
  SDL_GL_DestroyContext(GL_context);

  // destroys SDL window
  SDL_DestroyWindow(window);

  // quits SDL
  SDL_Quit();

  return 0;
}

// enter "sh make.sh" into terminal to run program