#include "./prefix-stem-suffix.hpp"

// all entries start with an empty array with 5 slots
// edit element arrays using void function similar to below:


// check how many atoms are needed, then add that many rows to the map in main
int atomNum_stem(string stem) 
{
    smatch match;

    regex reg(R"(regex)");

    int atom_num;       

    return atom_num;

}




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