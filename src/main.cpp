#include "render.hpp"
#include "defines/everythingClass.hpp"


int main(int argc, char *argv[]) {

  Context globalContext;

  // 1. initialize libraries
  render::init_SDL(&globalContext);
  render::set_OpenGL_Attributes(&globalContext);
  render::init_OpenGL(&globalContext);
  render::init_ImGui(&globalContext);

  // 2. set up geometry
  render::vertex_specification(&globalContext);

  // 3. set up shaders (at least, vertex ands fragment)
  render::create_graphics_pipeline(&globalContext);

  // 4. main run loop
  render::run_loop(&globalContext);

  // 5. cleans up
  render::clean_ImGui();
  render::clean_SDL(&globalContext);  

  return 0;
}

// enter "sh make.sh" into terminal to run program