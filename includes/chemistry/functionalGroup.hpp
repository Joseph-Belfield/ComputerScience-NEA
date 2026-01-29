#ifndef ATOM
    #include "atom.hpp"
    #define ATOM
#endif

#ifndef MOLECULE
    #include "molecule.hpp"
    #define MOLECULE
#endif

#ifndef VECTOR
    #include <vector>
    #define VECTOR
#endif


class Functional_Group
{
public:
    // holds pointers to the atoms 
    std::vector<Atom*> constituantAtoms;

    // virtual allows child classes to be used wherever their parent class is mentioned instead
    // finds the first alcohol group it can in a molecule, and returns a pointer to the O
    virtual Atom* findFunctionalGroup(Molecule* molecule) {} // tells compiler react will be a method in all future subclasses

    // finds the first alcohol group it can in a molecule, and returns a pointer to a new alcohol object made of these atoms
    virtual Functional_Group* findFunctionalGroup(Molecule* molecule, bool create) {}

    // finds all alcohol groups it can in a molecule, and returns a vector of alcohol object pointers
    virtual Functional_Group* findAllFunctionalGroup(Molecule* molecule) {}
};