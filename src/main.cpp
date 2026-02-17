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

  Atom atom1(OXYGEN);
  std::vector<Atom*> atoms = {&atom1};
  Molecule molecule(atoms);
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