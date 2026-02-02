#include "render/render.hpp"
#include "appData.hpp"
#include "render/objectData.hpp"
#include "render/shaderClass.hpp"
#include "errorChecking.hpp"

#include "chemistry/atom.hpp"
#include "chemistry/molecule.hpp"

#include <iostream>
#include <filesystem>
#include <vector>

int main() 
{
  // create an object to handle all important data in the program
  appData app;

  // 1. initialize libraries
  render::init_graphics(app);

  

  Sphere sphere(1.0f, 50, 50, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(2.0f, 0.0f, 0.0f), 1.0f);
  Sphere sphere2(1.0f, 50, 50, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(-2.0f, 0.0f, 0.0f), 1.0f);
  ReferencePlane plane;
  Cylinder tube(0.25f, 2.0f, 50, glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 90.0f), glm::vec3(1.0f, 2.0f, 1.0f));
  Cube cube;
  cube.uniform.uDisplacement.y += 5.0f;

  app.window.objects = {&sphere, &plane, &sphere2, &tube, &cube};

  Atom atom1(CARBON);
  Molecule molecule(&atom1);
  app.molecule = &molecule;

  // 3. main run loop
  render::run_loop(app);

  // 4. cleans up
  render::clean_ImGui();
  render::clean_SDL(app);  

  error::check_OpenGL_error();

  return 0;
}

// enter "sh make.sh" into terminal to run program