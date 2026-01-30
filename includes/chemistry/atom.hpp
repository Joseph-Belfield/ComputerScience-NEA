#pragma once

#include "chemistry/elements.hpp"
#include "render/objectData.hpp"

#include <vector>

// predfeine molecule to help atom
class Molecule;

// Atom class, holds essential details for atoms. 
struct Atom 
{ 
    Element element;

    int neutrons;
    int electrons;

    std::vector<Atom*> bonds;   // vector of pointers to atoms
    Molecule* parent = nullptr;  // pointer to molecule atom is in

    // the visual object representing the atom
    Sphere atomObject;

    // constructor!
    Atom();

    void draw_atom();
};