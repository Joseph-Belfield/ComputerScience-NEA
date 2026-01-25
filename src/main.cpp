#include "render.hpp"
#include "defines/appData.hpp"
#include "defines/objectData.hpp"

#include <iostream>
#include <filesystem>
#include <vector>


int main() 
{

  appData app;

  // 1. initialize libraries
  render::init_SDL(app);
  render::set_OpenGL_Attributes();
  render::init_OpenGL(app);
  render::init_ImGui(app);

  Sphere sphere(0.5f, 27, 27, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f);
  Sphere sphere2(0.5f, 27, 27, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 5.0f), 1.0f);

  app.window.objects = {&sphere, &sphere2};

  // 3. set up shaders (at least, vertex ands fragment)
  render::create_graphics_pipeline(app);

  sphere.uniform.uDisplacement.z -= 5.0f;
  sphere.uniform.uDisplacement.z += 5.0f;

  // 4. main run loop
  render::run_loop(app);

  // 5. cleans up
  render::clean_ImGui();
  render::clean_SDL(app);  

  error::check_OpenGL_error();

  return 0;
}

// enter "sh make.sh" into terminal to run program