#include <string>  // I need strings I like strings
#include <iostream>         // 'cout' and such

#include <stdio.h>
#include <stdlib.h>     // gives malloc (allocating space in memory)

using namespace std;        // Makes libraries work!

// Includes the file with all the functions. Can just be used as normal.
#include "./cpp_source/breakdown_functions.hpp"

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

int main() {


    /*
    // Creates an array where each value is 5. array is accessed through pointer to first element of array, result
    int *result = create_array(5);

    // prints each value in the array in format specified in double brackets
    for (int i = 0; i < 5; i++)
    {
        // results can be treated as the variable name for the array
        printf("result[%d] = %d\n", i, result[i]);
    }

    // sets all values in array pointed to by result to 4
    set_array(4, result); 

    for (int i = 0; i < 5; i++)
    {
        printf("result[%d] = %d\n", i, result[i]);

    }
    */


    string str = "The ape was at the apex of Ape Mountain.";
    regex reg("(ape[^ ]?)");
    print_matches(str, reg);

    return 0;
}