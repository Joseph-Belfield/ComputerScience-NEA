// Header files (.hpp) are files used by C++. Act as if they were just attatched to top of file included in.

#include <regex>            // Enables RegEx library.
#include <string>           // Enables strings.

#include <map>              // Maps act like dicts. Will be used for storing data about elements.
#include <array>            // Allows use of arrays - fixed-length data structures

#include "element_definitions.h"        // A set of useful #defines. Subs in integer values for element names, etc. 

using namespace std;        // Do not have to call STD every time! 




int* spine_regex() {
    int num_regexMatch = 1; //placeholder
    return 0;
}

map<int, int[5]> moleculeSpine(string moleculeName) {   // The map should contain a unique ID, and then data array.

    map<int, int[5]> atom1 = { {10,[1,11,12,13,14]} }       // unsure of how this works

}

