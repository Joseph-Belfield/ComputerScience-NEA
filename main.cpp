#include <string>  // I need strings I like strings
#include <iostream>         // 'cout' and such
using namespace std;        // Makes libraries work!

// Includes the file with all the functions. Can just be used as normal.
#include "./cpp_source/breakdown_functions.hpp"

// Python will run C++ as executable.

// Run previous functions in main. Cannot make functions in function unlike Python.
int main(int argc, char* argv[]) {

// print all command line arguments
    std::cout << "name of program: " << argv[0] << '\n' ;

    if( argc > 1 )
    {
        std::cout << "there are " << argc-1 << " (more) arguments, they are:\n" ;

        std::copy( argv+1, argv+argc, std::ostream_iterator<const char*>( std::cout, "\n" ) ) ;
    }

    return 0;
}