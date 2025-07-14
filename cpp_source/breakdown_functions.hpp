// Header files (.hpp) are files used by C++. Act as if they were just attatched to top of file included in.

#include <regex>            // Enables RegEx library.
#include <string>           // Enables strings.
using namespace std;        // Strings are cool.

#include <map>              // Maps act like dicts. Will be used for storing data about elements.

#include "element_definitions.h"        // A set of useful #defines. Subs in integer values for element names, etc. 

map<int, int> mainChain(string moleculeName) {

}

// (cyclo)?(meth|eth|prop|but|pent|hex|hept|oct|non|dec|undec|dodec|tridec|tetradec|pentadec|hexadec|heptadec|octadec|nonadecan|benzene)(?:(an|ane)|(?:(?:(?:-\d+(?:,\d+)*-)(?:di|tri|tetra|penta|hexa|hepta|octa|nona|deca|undeca|dodeca|trideca|tetradeca|pentadeca|hexadeca|heptadeca|octadeca|nonadeca)?)*(?:(en|ene|yn|yne))){1,2})