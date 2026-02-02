#pragma once

#include "render/objectData.hpp"

#include <vector>

// predfeine molecule to help atom
class Molecule;

enum Element
{
    HYDROGEN = 1,
    HELIUM,
    LITHIUM,
    BERYLLIUM,
    BORON,
    CARBON,
    NITROGEN,
    OXYGEN,
    FLUORINE
};

// Atom class, holds essential details for atoms. 
class Atom 
{ 
public:

 // _______________ ATTRIBS ______________

    // atom's element
    Element element;

    float atomicRadius;
    float bondAngle;
    glm::vec3 color;


    // these aren't really important...
    int neutrons;
    int electrons;

    uint maxBonds;
    std::vector<Atom*> bonds;   // vector of pointers to atoms
    Molecule* parent = nullptr;  // pointer to molecule atom is in


 // _______________ METHODS ______________

    // constructor!
    Atom(Element element);
};