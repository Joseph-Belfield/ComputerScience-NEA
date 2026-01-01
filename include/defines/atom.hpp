// way of keeping track of includes so if they're already present they are not repeated
#ifndef ELEMENT
    #include "elements.hpp"
    #define ELEMENT
#endif

#ifndef VECTOR
    #include <vector>
    #define VECTOR
#endif

// predfeine molecule to help atom
class Molecule;

// Atom class, holds essential details for atoms. 
struct Atom 
{ 
    const Name element;

    int neutrons;
    int electrons;

    std::vector<Atom*> bonds;   // vector of pointers to atoms
    Molecule* parent = nullptr;  // pointer to molecule atom is in
};