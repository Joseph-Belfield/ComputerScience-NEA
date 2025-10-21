#ifndef ATOM
    #include "atom.hpp"
    #define ATOM
#endif

#ifndef MOLECULE
    #include "molecule.hpp"
    #define MOLECULE
#endif

#ifndef FUNCTIONAL_GROUP
    #include "functionalGroup.hpp"
    #define FUNCTIONAL_GROUP
#endif

#ifndef VECTOR
    #include <vector>
    #define VECTOR
#endif

// finds a carbonyl(C=O) group (in ketones, aldehydes and carbox acids, etc)
class Carbonyl: public Functional_Group
{
public:

};