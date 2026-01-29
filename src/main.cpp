#include "render/render.hpp"
#include "appData.hpp"
#include "render/objectData.hpp"
#include "render/shaderClass.hpp"
#include "errorChecking.hpp"

#include <iostream>
#include <filesystem>
#include <vector>

int main() 
{
  // create an object to handle all important data in the program
  appData app;

  // 1. initialize libraries
  render::init_graphics(app);

  

  Sphere sphere(1.0f, 27, 27, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(2.0f, 0.0f, 0.0f), 1.0f);
  Sphere sphere2(1.0f, 27, 27, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 2.0f), 1.0f);
  ReferencePlane plane;
  Cylinder tube(0.5f, 2.0f, 50, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(-2.0f, 0.0f, 0.0f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(1.0f, 5.0f, 1.0f));
  Cube cube;

  app.window.objects = {&sphere, &plane, &sphere2, &tube, &cube};

  // 3. main run loop
  render::run_loop(app);

  // 4. cleans up
  render::clean_ImGui();
  render::clean_SDL(app);  

  error::check_OpenGL_error();

  return 0;
}

// enter "sh make.sh" into terminal to run program