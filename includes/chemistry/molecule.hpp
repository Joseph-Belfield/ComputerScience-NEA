#ifndef ATOM
    #include "atom.hpp"
    #define ATOM
#endif

#ifndef VECTOR
    #include <vector>
    #define VECTOR
#endif

#ifndef STRING
    #include <string>
    #define STRING
#endif

// prefefining atom so that it can be referenced by molecule
struct Atom;

// A class for effectively anything rendered - functional groups will be a subclass
class Molecule
{
public: 
    std::vector<Atom*> atoms;     // vector of pointers to atoms in molecule

    std::string name = "";

    // Constructor class. Enter the first atom of the molecule, will tell child who parent, will tell parent who child.
    Molecule(Atom* firstAtom)
    {
        firstAtom -> parent = this;
        atoms.push_back(firstAtom);
    }

    Molecule(std::vector<Atom*>* atomVector)
    {
        for (int i = 0; atomVector -> size(); i++)
        {
            (*atomVector)[i] -> parent = this;  // dereferences atom vector, then gets the atom i and changes its parent
            atoms.push_back((*atomVector)[i]); // adds a copy of the atom from the pointer to the vector atoms
        }
    }
};