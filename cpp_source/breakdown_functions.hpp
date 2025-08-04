// Header files (.hpp) are files used by C++. Act as if they were just attatched to top of file included in.

#include <regex>            // Enables RegEx library.
#include <string>           // Enables strings.

#include <map>              // Maps act like dicts. Will be used for storing data about elements.
#include <array>            // Allows use of arrays - fixed-length data structures

#include "element_definitions.h"        // A set of useful #defines. Subs in integer values for element names, etc. 

using namespace std;        // Do not have to call std every time! 


// creates an array of a certain element, then outputs pointer to array
int *array_maker(int element)
{
    static int array[5] = {element,1,2,3,4};

    return array;
}


// creates a map called elementData
void map_maker()
{
    static map<int, int *> elementData;
}


int* spine_regex() {
    int num_regexMatch = 1; //placeholder
    return 0;
}



