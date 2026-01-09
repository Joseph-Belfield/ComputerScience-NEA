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
  #include "defines/atom.hpp"
  #define ATOM
#endif

#ifndef ELEMENT
  #include "defines/elements.hpp"
  #define ELEMENT
#endif

// ********************** GLOBAL VARRIABLES **********************

namespace global
{
 
  // the window the program runs in
  SDL_Window* window = nullptr;

  // the OpenGL context for the window 
  SDL_GLContext context_OpenGL = nullptr;

  // sets GLSL version (matches OpenGL version)
  const char* version_glsl;

  // the main scale of the program. relative to display size
  float mainScale;

  // screen dimensions
  int window_height;
  int window_width;

  // main loop flag
  bool flag_mainLoop = true;

  // unsigned ints as identifiers for the objects (because C-based language)
  GLuint vertexArrayObject = 0;
  GLuint vertexBufferObject = 0;
  GLuint indexBufferObject = 0;

  // unique ID for the graphics pipeline
  GLuint shaderProgram = 0;

  // offset that allows us to change values in the GPU
  float uOffset;
  float uModelMatrix;
}
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
  global::mainScale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());

  // flags for the window
  SDL_WindowFlags window_flags =  SDL_WINDOW_RESIZABLE |          // lets window be resized
                                  SDL_WINDOW_OPENGL |             // window uses OpenGL context
                                  SDL_WINDOW_HIDDEN |             // window hidden during setup
                                  SDL_WINDOW_HIGH_PIXEL_DENSITY;  // higher quality


  // creates a window assigned to 'window', errors if failed
  global::window = SDL_CreateWindow("compsci_nea", (int)(1280 * global::mainScale), (int)(800 * global::mainScale), window_flags);

  // checks if window has been created properly
  if (global::window == nullptr) 
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
  global::version_glsl = "#version 410";


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
  global::context_OpenGL = SDL_GL_CreateContext(global::window);

  // checks context has been created properly
  if (global::context_OpenGL == nullptr) 
  {
    SDL_Log("Failed to create OpenGL context.");
    exit(-1); 
  }


  // initializes GLAD
  gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);

  
  SDL_GL_MakeCurrent(global::window, global::context_OpenGL); // sets current window and context
  SDL_GL_SetSwapInterval(1); // Enable vsync
  SDL_SetWindowPosition(global::window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED); // centres window
  SDL_ShowWindow(global::window);  // reveals window once program has been initialized
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

  if (!SDL_GetWindowSizeInPixels(global::window, &global::window_width, &global::window_height))
  {
    SDL_Log("Failed to get window size!");
    exit(-1);
  }
  io.DisplaySize = ImVec2((float)global::window_width, (float)global::window_height);

  ImGui::StyleColorsDark();
  ImGuiStyle &style = ImGui::GetStyle();
  
  // Eall scales in file scale around this. errors with style to do with size are probably this.
  if (global::mainScale > 1.0f)
  {
    style.ScaleAllSizes(global::mainScale); 
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
  style.FontSizeBase = 20.0f * global::mainScale;

  io.Fonts -> AddFontDefault();
  ImFont* Arimo_Regular = io.Fonts -> AddFontFromFileTTF("fonts/Arimo-Regular.ttf", 20.0f);
  ImFont* Roboto_SemiCondensed_Italic = io.Fonts -> AddFontFromFileTTF("fonts/Roboto_SemiCondensed-Italic.ttf", 20.0f);

  ImGui_ImplSDL3_InitForOpenGL(global::window, global::context_OpenGL);
  ImGui_ImplOpenGL3_Init(global::version_glsl);
}



void vertex_specification()
{
  // use of GLfloat as it is more cross-platform (likely won't matter but best practice)
  const std::vector<GLfloat> vertexData   // lives on CPU
  {
    -0.5f, -0.5f,  0.0f,    // vertex 1
     1.0f,  0.0f,  0.0f,    // color

     0.5f, -0.5f,  0.0f,    // vertex 2
     0.0f,  1.0f,  0.0f,    // color

    -0.5f,  0.5f,  0.0f,    // vertex 3
     0.0f,  0.0f,  1.0f,    // color   

     0.5f,  0.5f,  0.0f,    // vertex 4 
     0.0f,  0.0f,  1.0f     // color
  };


  // generate Vertex Array Objects  
  glGenVertexArrays(1, &global::vertexArrayObject);             // creates an array to hold vertex data (called vertexArrayObject)
  glBindVertexArray(global::vertexArrayObject);                 // selects the array as current

  // generate Vertex Buffer Object for position
  glGenBuffers(1, &global::vertexBufferObject);                 // generates buffer
  glBindBuffer(GL_ARRAY_BUFFER, global::vertexBufferObject);    // sets buffer as current, specifies target
  glBufferData
  (
    GL_ARRAY_BUFFER,                           // specifies target
    vertexData.size() * sizeof(GLfloat),       // finds the size (in bytes) of vertex data
    vertexData.data(),                         // pointer to the array holding the data of the vector
    GL_STATIC_DRAW                             // sets intentions with data
  );                          


  // data about the order vertices should be renderd in
  const std::vector<GLuint> indexBufferData
  {
    2, 0, 1,      // triangle 1
    3, 2, 1       // triangle 2
  };

  // set up Element/Index Buffer Object (EBO / IBO) - holds the index for the order in which vertices are drawn
  glGenBuffers(1,&global::indexBufferObject);                         // generate EBO
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, global::indexBufferObject);   // sets buffer type as element buffer
  glBufferData
  (
    GL_ELEMENT_ARRAY_BUFFER,                        // target
    indexBufferData.size() * sizeof(GLuint),        // size
    indexBufferData.data(),                         // data
    GL_STATIC_DRAW                                  // usage
  );



  // setup position VAO
  glEnableVertexAttribArray(0); // enables the 0th attribute - AKA. this is the first VAO
  glVertexAttribPointer
  (
    0,                        // index into vector of VAOs
    3,                        // pieces of data (per vertex: x, y, z)
    GL_FLOAT,                 // data type
    GL_FALSE,                 // normalized?
    sizeof(GL_FLOAT) * 6,     // stride (no. of bytes) to jump from first (type) data of v1 to first (type) data of v2, etc         
    (GLvoid*)0                // pointer for offset - irrelivent as position data is in first slot
  );

  // setup color VAO
  glEnableVertexAttribArray(1); // enables the 1st attribute - AKA. this is the second VAO
  glVertexAttribPointer
  (
    1,                                 // index into vector of VAOs
    3,                                 // pieces of data (per vertex: r, g, b)
    GL_FLOAT,                          // data type
    GL_FALSE,                          // normalized?
    sizeof(GL_FLOAT) * 6,              // stride (byte offset) between firsts of same data       
    (GLvoid*)(sizeof(GL_FLOAT) * 3)    // pointer for offset - starting position for first of that data type (address)
  );


  // cleanup VAO
  glBindVertexArray(0);                 // unbind currently bound VAO

  // disable any attributes previously opened in our vertex attribute array
  glDisableVertexAttribArray(0);        // position
  glDisableVertexAttribArray(1);
}


// load a shader from a file, passing in reference to file name
std::string load_shader_from_file(const std::string& fileName)
{
  // initialize result var
  std::string result = "";    // holds shader program as single string

  std::string line = "";  // holds one line of shader file at a time
  std::ifstream shaderFile(fileName.c_str());     // opens file

  // if the file is opened successfully
  if (shaderFile.is_open())
  {
    while(std::getline(shaderFile, line))   // go through each line of the file
    {
      result += line + "\n";                // concatinate new line into result string
    }
    shaderFile.close();                     // close file when done 
  }

  return result;
}



GLuint compile_shader(GLuint type, const std::string source)
{
  // create shader object
  GLuint shaderObject;

  // check type of shader
  if (type == GL_VERTEX_SHADER)
  {
    shaderObject = glCreateShader(GL_VERTEX_SHADER);
  }
  else if (type == GL_FRAGMENT_SHADER)
  {
    shaderObject = glCreateShader(GL_FRAGMENT_SHADER);
  }

  // turn source to a C-string
  const char* src = source.c_str();

  // create shader source code
  glShaderSource
  (
    shaderObject,   // shader
    1,              // amount of elements compiled (1 shader)
    &src,           // shader source
    nullptr         // length of string 
  );

  // compile the shader
  glCompileShader(shaderObject);

  // error checking
  int result;
  glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &result);  // gets compile status, stores in result

  // if shaders failed to compile
  if (result == GL_FALSE)
  {
    int length;                                                     
    glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &length);           // find the length of the error message
    char* errorMessages = new char[length];                             // create a C-string of that length
    glGetShaderInfoLog(shaderObject, length, &length, errorMessages);   // log error info

    // display error messages
    if (type == GL_VERTEX_SHADER)
    {
    SDL_Log("GL_VERTEX_SHADER compilation failed! \n");
    SDL_Log("%s", errorMessages);
    }
    else if(type == GL_FRAGMENT_SHADER)
    {
      SDL_Log("GL_FRAGMENT_SHADER compilation failed! \n");
      SDL_Log("%s", errorMessages);
    }

    // reclaim memory
    delete[] errorMessages;

    // delete broken shader object
    glDeleteShader(shaderObject);

    return 0;
  }

  return shaderObject;
}



GLuint create_shader_program(const std::string source_vertexShader, const std::string source_fragmentShader)
{
  GLuint programObject = glCreateProgram(); // creates an empty program to be filled with shaders

  // compile shaders
  GLuint vertexShader = compile_shader(GL_VERTEX_SHADER, source_vertexShader);
  GLuint fragmentShader = compile_shader(GL_FRAGMENT_SHADER, source_fragmentShader);

  // attatch shaders to program object
  glAttachShader(programObject, vertexShader);    // attatches vertex shader to the object
  glAttachShader(programObject, fragmentShader);
  glLinkProgram(programObject);   // links shaders together within object

  // validate program - check for errors
  glValidateProgram(programObject); 

  return programObject;
}



void create_graphics_pipeline()
{
  std::string source_vertexShader = load_shader_from_file("./shaders/vertexShader.glsl");
  std::string source_fragmentShader = load_shader_from_file("./shaders/fragmentShader.glsl");

  global::shaderProgram = create_shader_program(source_vertexShader, source_fragmentShader);
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
  
  glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, global::uOffset, 0.0f));

  GLuint uLocation_ModelMatrix = glGetUniformLocation(global::shaderProgram, "uModelMatrix");
  if (uLocation_ModelMatrix >= 0)
  {
    glUniformMatrix4fv
    (
    uLocation_ModelMatrix,         // location
    1,                             // count 
    false,                         // transpose (bool)
    &translate[0][0]               // value
    );
  }
  else
  {
    SDL_Log("Could not find uniform location!");
    SDL_Log("Check spelling.");
    exit(-1);
  }

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
  init_SDL();
  set_OpenGL_Attributes();
  init_Opencontext_OpenGL();
  init_ImGui();

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