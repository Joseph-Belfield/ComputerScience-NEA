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

#ifndef ATOM
  #include "defines/chemistry/atom.hpp"
  #define ATOM
#endif

#ifndef ELEMENT
  #include "defines/chemistry/elements.hpp"
  #define ELEMENT
#endif

// ********************** GLOBAL VARRIABLES **********************

#ifndef GLOBAL
  #include "defines/global.hpp"
  #define GLOBAL
#endif

// ********************** WINDOW **********************

#ifndef INIT
  #include "window/init.hpp"
  #define INIT
#endif

#ifndef VERTEXSPEC
  #include "window/vertexSpecification.hpp"
  #define VERTEXSPEC
#endif

#ifndef GRAPHICS_PIPELINE
  #include "window/graphicsPipeline.hpp"
  #define GRAPHICS_PIPELINE
#endif

// *************************************************





void check_events()
{
  // checks for events
  SDL_Event event;

  // if there were events, do:
  while (SDL_PollEvent(&event)) {

    // ImGui processes the event
    ImGui_ImplSDL3_ProcessEvent(&event);

    // if SDL is quit, end the run loop
    if (event.type == SDL_EVENT_QUIT) 
    {
      global::flag_mainLoop = false;
    }
    // if Esc key is pressed, end the run loop
    if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_ESCAPE) 
    {
      global::flag_mainLoop = false;
    }

    if (event.key.key == SDLK_UP)
    {
      global::uOffset+=0.1f;
    }

    if (event.key.key == SDLK_DOWN)
    {
      global::uOffset-=0.1f;
    }

    if (event.key.key == SDLK_RIGHT)
    {
      global::uRotate+=1.0f;
    }

    if (event.key.key == SDLK_LEFT)
    {
      global::uRotate-=1.0f;
    }

  }

}

// model matrices move objects from their 'local' space and positions it in the 'world' space
void model_matrix()
{
  // create and adapt the matrix to adjust the following transformations
  glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, global::uOffset)); // movement
  modelMatrix = glm::rotate(modelMatrix ,glm::radians(global::uRotate), glm::vec3(0.0f, 1.0f, 0.0f));  // rotations
  modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0f, 1.0f, 1.0f));

  GLuint uLocation_modelMatrix = glGetUniformLocation(global::shaderProgram, "uModelMatrix"); // get location of uniform variable
  if (uLocation_modelMatrix >= 0)   // if that space has been allocated correctly
  {
    glUniformMatrix4fv             // create a uniform matrix
    (
    uLocation_modelMatrix,         // location
    1,                             // count 
    false,                         // transpose (bool)
    &modelMatrix[0][0]               // value
    );
  }
  else                              // else throw an error
  {
    SDL_Log("Could not find uniform location!");
    SDL_Log("Check spelling.");
    exit(-1);
  }
}


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

  GLuint uLocation_perspective = glGetUniformLocation(global::shaderProgram, "uPerspective");    // get location of uniform variable
  if (uLocation_perspective >= 0)   // if that space has been allocated correctly
  {
    glUniformMatrix4fv             // create a uniform matrix
    (
    uLocation_perspective,         // location
    1,                             // count 
    false,                         // transpose (bool)
    &perspective[0][0]             // value
    );
  }
  else                              // else throw an error
  {
    SDL_Log("Could not find uniform location!");
    SDL_Log("Check spelling.");
    exit(-1);
  }
}



// for setting OpenGL state
void preDraw_OpenGL()
{
  // disables
  glDisable(GL_DEPTH_TEST); // disables depth check - 2D scene
  glDisable(GL_CULL_FACE);  // disables checking for overlap - 2D scene

  // set size of window for OpenGL
  glViewport(0, 0, (int)global::window_width, (int)global::window_height);

  // background color
  glClearColor(0.0f, 1.0f, 1.0f, 1.0f);                   // sets background color
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);     // clears the OpenGL color and depth buffers

  glUseProgram(global::shaderProgram);            // selects program in use

  // transformation matrices
  model_matrix();         // controls position and rotation on world axis
  perspective_matrix();   // creats illusion of perspective (size changes relative to camera)

}


// for drawing OpenGL data
void draw_OpenGL()
{
  // choose VAO and VBO
  glBindVertexArray(global::vertexArrayObject);
  glBindBuffer(GL_ARRAY_BUFFER, global::vertexBufferObject);

  // draw
  glDrawElements
  (
    GL_TRIANGLES,         // shape
    6,                    // number of vertices drawn to (count repeats)
    GL_UNSIGNED_INT,      // data type
    0                     // offset into index array for first element (triangle vertex order)
  );
}



void draw_ImGui(bool* show_mainWindow, bool* show_colorPicker, bool* show_sineGraph, bool* show_scrolling, bool* show_helloWorld)
{

  ImGui::Begin("Main Window", show_mainWindow, ImGuiWindowFlags_MenuBar);  

  if (ImGui::BeginMenuBar())
  {
    if (ImGui::BeginMenu("Options"))
    {
      if (ImGui::MenuItem("Change Background Color")) {*show_colorPicker = true;}
      if (ImGui::MenuItem("Sine Graph")) {*show_sineGraph = true;}
      if (ImGui::MenuItem("Scrolling")) {*show_scrolling = true;}
      ImGui::EndMenu();
    }
    ImGui::EndMenuBar();
  }

  if (ImGui::Button("Hello World"))
  {
    *show_helloWorld = true;
  }

  ImGui::End();



  if (show_helloWorld)
  {
    ImGui::Text("Hello World!");
  }

  if (show_sineGraph)
  {
    ImGui::Begin("Sine Graph", show_sineGraph);
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
    ImGui::Begin("Scrolling", show_scrolling);
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
    ImGui::Begin("Background Color", show_colorPicker);
    // edit a color stored as 4 floats
    // ImGui::ColorEdit3("Color", (float*)&clear_color);
    ImGui::End();
  }
}



void run_loop()
{
  // window background color variables
  ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);

  // bools determining if windows should be rendered  
  bool show_mainWindow;
  bool show_helloWorld = false;
  bool show_colorPicker = false;
  bool show_sineGraph = false;
  bool show_scrolling = false;

  // ********************** RUN LOOP **********************

  while (global::flag_mainLoop) {

    check_events();

    // starts a new frame for OpenGL, SDL and ImGui
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    // ********************** DO STUFF HERE **********************

    draw_ImGui(&show_mainWindow, &show_colorPicker, &show_sineGraph, &show_scrolling, &show_helloWorld);

    preDraw_OpenGL();
    draw_OpenGL();


    // render
    ImGui::Render();                                               // renders ImGui instructions 
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());        // renders the ImGui data with OpenGL  
    SDL_GL_SwapWindow(global::window);                                     // swaps in the new frame

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
  SDL_GL_DestroyContext(global::context_OpenGL);

  // destroys SDL window
  SDL_DestroyWindow(global::window);

  // quits SDL
  SDL_Quit();
}



// *************************************************

int main(int argc, char *argv[]) {

  // 1. initialize libraries
  init::init_SDL();
  init::set_OpenGL_Attributes();
  init::init_Opencontext_OpenGL();
  init::init_ImGui();

  // 2. set up geometry
  vertex_specification();

  // 3. set up shaders (at least, vertex and fragment)
  create_graphics_pipeline();

  // 4. main run loop
  run_loop();

  // 5. cleans up
  clean_ImGui();
  clean_SDL();  

  return 0;
}

// enter "sh make.sh" into terminal to run program