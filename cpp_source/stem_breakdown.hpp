#include "./prefix-stem-suffix.hpp"

// create the map, then add entries using a recusive array here
// all entries start with an empty array with 5 slots
// edit element arrays using void function similar to below:

/*

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

*/