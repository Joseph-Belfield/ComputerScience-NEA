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

#ifndef STRING
  #include <string.h>              // has standard input/output functions
  #define STRING
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

// ********************** GLOBAL VARRIABLES **********************

// the window the program runs in
SDL_Window* window = nullptr;

// the OpenGL context for the window 
SDL_GLContext context_OpenGL = nullptr;

// sets GLSL version (matches OpenGL version)
const char* glsl_version;

// the main scale of the program. relative to display size
float main_scale;

int window_height;
int window_width;

// bool that controls if the main run loop is active
bool runtime_loop_active = true;

// *************************************************


void init_SDL()
{
  // initialize SDL
  if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) 
  {
    SDL_Log("Failed to initialize SDL.");
    exit(-1); 
  }
  

  // finds the scale of the display 
  main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());

  // flags for the window
  SDL_WindowFlags window_flags = SDL_WINDOW_RESIZABLE |          // lets window be resized
                                  SDL_WINDOW_OPENGL |             // window uses OpenGL context
                                  SDL_WINDOW_HIDDEN |             // window hidden during setup
                                  SDL_WINDOW_HIGH_PIXEL_DENSITY;  // higher quality


  // creates a window assigned to 'window', errors if failed
  window = SDL_CreateWindow("compsci_nea", (int)(1280 * main_scale), (int)(800 * main_scale), window_flags);

  // checks if window has been created properly
  if (window == nullptr) 
  {
    SDL_Log("Failed to create window.");
    exit(-1); 
  }
}



void set_OpenGL_Attributes()
{
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
}



void init_Opencontext_OpenGL()
{
  // create the context for OpenGL in 'window'
  context_OpenGL = SDL_GL_CreateContext(window);

  // checks context has been created properly
  if (context_OpenGL == nullptr) 
  {
    SDL_Log("Failed to create OpenGL context.");
    exit(-1); 
  }


  // initializes GLAD
  gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);

  
  SDL_GL_MakeCurrent(window, context_OpenGL); // sets current window and context
  SDL_GL_SetSwapInterval(1); // Enable vsync
  SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED); // centres window
  SDL_ShowWindow(window);  // reveals window once program has been initialized
}


void init_ImGui()
{
  // creates ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  // ImGui Inputs/Outputs
  ImGuiIO &io = ImGui::GetIO();  (void)io; // initializes io

  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls

  if (!SDL_GetWindowSizeInPixels(window, &window_width, &window_height))
  {
    SDL_Log("Failed to get window size!");
    exit(-1);
  }
  io.DisplaySize = ImVec2((float)window_width, (float)window_height);

  ImGui::StyleColorsDark();
  ImGuiStyle &style = ImGui::GetStyle();
  
  // Eall scales in file scale around this. errors with style to do with size are probably this.
  if (main_scale > 1.0f)
  {
    style.ScaleAllSizes(main_scale); 
   /*  Bake a fixed style scale. (until we have a solution for dynamic style scaling, 
    changing this requires resetting Style + calling this again) makes this unnecessary. 
    We leave both here for documentation purpose) */
  }
  else
  {
    // protection so program doesn't error
    style.ScaleAllSizes(1.0f);
  }


  // sets a base style for the fonts
  style.FontSizeBase = 20.0f * main_scale;

  io.Fonts -> AddFontDefault();
  //ImFont* Arimo_Regular = io.Fonts -> AddFontFromFileTTF("fonts/Arimo-Regular.ttf", 20.0f);
  //ImFont* Roboto_SemiCondensed_Italic = io.Fonts -> AddFontFromFileTTF("fonts/Roboto_SemiCondensed-Italic.ttf", 20.0f);

  ImGui_ImplSDL3_InitForOpenGL(window, context_OpenGL);
  ImGui_ImplOpenGL3_Init(glsl_version);
}



void check_events()
{
  // checks for events
  SDL_Event event;

  // if there were events, do:
  while (SDL_PollEvent(&event)) {

    // ImGui processes the event
    ImGui_ImplSDL3_ProcessEvent(&event);

    // if SDL is quit, end the run loop
    if (event.type == SDL_EVENT_QUIT) {
      runtime_loop_active = false;
    }
    // if Esc key is pressed, end the run loop
    if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_ESCAPE) {
      runtime_loop_active = false;
    }
  }
}



void clean_ImGui()
{
  // shuts down ImGui + OpenGL link
  ImGui_ImplOpenGL3_Shutdown();

  // shuts down ImGui + SDL link
  ImGui_ImplSDL3_Shutdown();

  // destroys ImGui context
  ImGui::DestroyContext();
}



void clean_SDL()
{
  // destroys SDL context 
  SDL_GL_DestroyContext(context_OpenGL);

  // destroys SDL window
  SDL_DestroyWindow(window);

  // quits SDL
  SDL_Quit();
}



// *************************************************

int main(int argc, char *argv[]) {

  // ********************** INITIALIZE LIBRARIES HERE **********************

  init_SDL();

  set_OpenGL_Attributes();
  init_Opencontext_OpenGL();

  init_ImGui();

  // ********************** INITIALIZE VARIABLES HERE **********************

  // window background color variables
  ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);

  // bools determining if windows should be rendered
  bool main_window = true;
  bool hello_world = false;
  bool show_colorPicker = false;
  bool show_sineGraph = false;
  bool show_scrolling = false;

  // ********************** RUN LOOP **********************

  while (runtime_loop_active) {

    check_events();

    // starts a new frame for OpenGL, SDL and ImGui
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    // ********************** DO STUFF HERE **********************

    ImGui::Begin("Main Window", &main_window, ImGuiWindowFlags_MenuBar);

    // sets the OpenGL viewport (what you can see)
    

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

    float whatSDLthinkthescaleis = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
    ImGui::Text("%f", whatSDLthinkthescaleis);

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
      ImGui::Begin("Background Color", &show_colorPicker);
      // edit a color stored as 4 floats
      ImGui::ColorEdit3("Color", (float*)&clear_color);
      ImGui::End();
    }

    // ********************** RENDERING STAGE **********************

    // renders ImGui instructions 
    ImGui::Render();

    glViewport(0, 0, (int)window_width, (int)window_height);

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

  // cleans up ImGui
  clean_ImGui();

  // cleans up SDL
  clean_SDL();

  return 0;
}

// enter "sh make.sh" into terminal to run program