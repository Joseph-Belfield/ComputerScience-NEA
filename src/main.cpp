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

  objectData object1;
  objectData object2;

  // 2. set up geometry
  render::create_mesh(object1);
  render::create_mesh(object2);

  // 3. set up shaders (at least, vertex ands fragment)
  render::create_graphics_pipeline(app);

  object1.mesh.shaderProgram = app.program.shaderProgram;
  object2.mesh.shaderProgram = app.program.shaderProgram;

  object2.uniform.uDisplacement.z += 5.0f;

  // 4. main run loop
  render::run_loop(app, object1, object2);

  // 5. cleans up
  render::clean_ImGui();
  render::clean_SDL(app, object1, object2);  

  
  std::cout << "ERROR! -> " << glGetError() << std::endl;

  return 0;
}

// enter "sh make.sh" into terminal to run program