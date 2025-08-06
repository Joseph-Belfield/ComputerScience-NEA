// Header files (.hpp) are files used by C++. Act as if they were just attatched to top of file included in.

#include <regex>            // Enables RegEx library.
#include <string>           // Enables strings.

#include <iostream> // Includes cout, etc

#include <map>              // Maps act like dicts. Will be used for storing data about elements.
#include <array>            // Allows use of arrays - fixed-length data structures

#include "element_definitions.h"        // A set of useful #defines. Subs in integer values for element names, etc. 

using namespace std;        // Do not have to call std every time! 


// creates an array of a certain element, then outputs pointer to array
int *array_maker(int element)
{
    static int atom[5] = {element,1,2,3,4};

    return atom;
}

void print_matchData(string str, regex reg)
{
    // stores any matches found in the regex
    smatch matches;

    // searches the string for matches with regex, then stores in matches
    while (regex_search(str, matches, reg)){

        // finds if there are any matches - outputs boolean
        cout << "is there a match : " << matches.ready() << "\n";

        // finds if there are no matches - outputs boolean
        cout << "are there no matches : " << matches.empty() << "\n";

        // finds the number of matches (size of variable matches)
        cout << "number of matches : " << matches.size() << "\n";

        // outputs the first match
        cout << "match : " << matches.str(1) << "\n";

        str = matches.suffix().str();
        cout << "\n";
    }
}

void print_matches(string str, regex reg)
{
    // sets variables currentMatch and lastMatch
    sregex_iterator currentMatch(str.begin(),str.end(),reg);
    sregex_iterator lastMatch;

    // while the match is not the last match...
    while (currentMatch != lastMatch)
    {
        // ...set match to the current match and output as a string. iterate currentMatch
        smatch match = *currentMatch;
        cout << match.str() << "\n";
        currentMatch++;
    }
}



