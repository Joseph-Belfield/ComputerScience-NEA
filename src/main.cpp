#include "render.hpp"
#include "defines/contextData.hpp"
#include <iostream>
#include <filesystem>


int main() 
{

  appData app;

  // 1. initialize libraries
  render::init_SDL(app);
  render::set_OpenGL_Attributes();
  render::init_OpenGL(app);
  render::init_ImGui(app);


  // 2. set up geometry
  render::vertex_specification(app);


  // 3. set up shaders (at least, vertex ands fragment)
  render::create_graphics_pipeline(app);

  // 4. main run loop
  render::run_loop(app);

  // 5. cleans up
  render::clean_ImGui();
  render::clean_SDL(app);  

  
  std::cout << "ERROR! -> " << glGetError() << std::endl;

  return 0;
}

// enter "sh make.sh" into terminal to run program