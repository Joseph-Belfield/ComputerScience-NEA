#include <string>  // I need strings I like strings
#include <iostream>         // 'cout' and such
#include <map>              // Allows for maps to be used

#include <stdio.h>
#include <stdlib.h>     // gives malloc (allocating space in memory)

using namespace std;        // Makes libraries work!

// Includes the file with all the functions. Can just be used as normal.
#include "./cpp_source/____.hpp"

// Python will run C++ as executable.

// Run previous functions in main. Cannot make functions in function unlike Python.


// Previous code: 

/*
int main(int argc, char* argv[])
{
    // print all command line arguments
    std::cout << "name of program: " << argv[0] << '\n' ;

    if( argc > 1 )
    {
        std::cout << "there are " << argc-1 << " (more) arguments, they are:\n" ;

        std::copy( argv+1, argv+argc, std::ostream_iterator<const char*>( std::cout, "\n" ) ) ;
    }

    return 0;
}

*/

// return type is a pointer to an array
int *create_array(int value)
{
    static int array[5];        // creates an array with the lifetime of the entire program

    // assigns each slot in the array a value 
    for (int i = 0; i < 5; i++) 
    {
        array[i] = value;
    }

    return array;  // returns a pointer to the first item in the array
}

// since the function does not return anything, just modifies an already existing array, return type is void
void set_array(int value, int *array) 
{
    for (int i = 0; i < 5; i++) 
        array[i] = value;
}

int main() 
{
    string moleculeName;   // link this to input in python GUI later
    
    // create the map
    map<int , int[]> atomList;

    return 0;
}