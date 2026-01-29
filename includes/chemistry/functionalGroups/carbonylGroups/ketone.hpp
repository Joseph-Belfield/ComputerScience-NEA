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

#ifndef CARBONYL
    #include "carbonyl.hpp"
    #define CARBONYL
#endif

#ifndef VECTOR
    #include <vector>
    #define VECTOR
#endif

class Ketone: public Carbonyl
{

};