#include <string>  // I need strings I like strings
#include <iostream>         // 'cout' and such

#include <stdio.h>
#include <stdlib.h>     // gives malloc (allocating space in memory)

using namespace std;        // Makes libraries work!

// Includes the file with all the functions. Can just be used as normal.
// #include "./cpp_source/breakdown_functions.hpp"

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


void set_array(int value, int *array) 
{
    for (int i = 0; i < 5; i++) 
        array[i] = value;
}

int main() {

    int array[] = {0,1,2,3,4};

    for (int i = 0; i < 5; i++)
    {
        cout << array[i] << "\n";
    }

    set_array(4, array); 

    for (int i = 0; i < 5; i++)
    {
        cout << array[i] << "\n";

    }

    return 0;
}