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


// contains information any reaction may need, plus methods for reactions
class Reaction
{
public: 
    
    // virtual allows child classes to be used wherever their parent class is mentioned instead
    virtual Molecule react(Molecule mol1, Molecule mol2) {} // tells compiler react will be a method in all future subclasses
};