#include "chemistry/molecule.hpp"

#include "glm/gtc/matrix_transform.hpp"

#include <string>
#include <iostream>

Molecule::Molecule(Atom* firstAtom)
{
    firstAtom -> parent = this;
    atoms.push_back(firstAtom);
}

Molecule::Molecule(std::vector<Atom*>* atomVector)
{
    for (int i = 0; atomVector -> size(); i++)
    {
        (*atomVector)[i] -> parent = this;  // dereferences atom vector, then gets the atom i and changes its parent
        atoms.push_back((*atomVector)[i]);  // adds a copy of the atom from the pointer to the vector atoms
    }
}

// will join 2 molecules together at set atoms, or add another bond between atoms in a molecule 
void Molecule::addBond(Atom* atom1, Atom* atom2)
{
    if (atom1 -> parent == atom2 -> parent) // if the atoms share a parent (are in the same molecule)
    {
        // add an additional bond to each others' bond vectors
        atom1 -> bonds.push_back(atom2);
        atom2 -> bonds.push_back(atom1);
    }
    else
    {
        // holds atom 2's parent molecule so its saved when atom 2 is wiped 
        Molecule* temp_atom2_parent = atom2 -> parent;

        // for each atom in atom 2's parent molecule
        for (int i = 0; atom2 -> parent -> atoms.size(); i++)
        {
            // change the parent from atom 2' parent molecule to atom 1's parent molecule
            temp_atom2_parent -> atoms.at(i) -> parent = atom1 -> parent;
        }

        // append vector of atoms from molecule 2 to end of molecule 1
        atom1 -> parent -> atoms.insert(atom1 -> parent -> atoms.end(), atom2 -> parent -> atoms.cbegin(), atom2 -> parent -> atoms.cend());

        // wipes the name of molecule 1 
        atom1 -> parent -> name = "placeholder";

        // adds a bond between atom 1 and 2
        atom1 -> bonds.push_back(atom2);
        atom2 -> bonds.push_back(atom1);

        // frees the memory which held atom 2's parent molecule
        delete temp_atom2_parent;

        // prevents other parts of the program from accessing cleared memory accidentally by removing tracks
        temp_atom2_parent = nullptr;
    }
}

int Molecule::find_atom(Atom* targetAtom)
{
    int count = 0;
    for (int i = 0; i < atoms.size(); i++)
    {
        if (atoms[i] == targetAtom)
        {
            count++;
        }
    }

    return count;
}

// fills vector with pointers to any atom connected to atom1's tree
void Molecule::find_molecule_tree(Atom* currentAtom)
{
    // checks number of times the current atom is in the molecule vector
    int atomAccountedFor = find_atom(currentAtom);

    // if the atom isn't found, add the atom - just adds parameter atom at start of function run
    if (atomAccountedFor == 0)
    {
       // moleculeVector -> push_back(currentAtom);
    }

    // if that atom is found more than once, throw an error message
    else if (atomAccountedFor >= 2)
    {
        std::cout << "ERROR - atom found twice in molecule string" << "\n";
    }

    // for each atom bonded to current atom
    for (int i = 0; currentAtom -> bonds.size(); i++)
    {
        if (find_atom(currentAtom -> bonds[i]) == 0) // if the atom doesn't appear in the molecule list
        {
            find_molecule_tree(currentAtom -> bonds[i]);  // recurse the function, passing in the new branch and the molecule list as parameters
        }
    }
    
    return; // if all atoms attatched to an atom have been found, return function
}

// will remove a bond between two atoms, and if applicable, split a larger molecule into two smaller parts
Molecule* Molecule::removeBond(Atom* atom1, Atom* atom2)
{
    // maintains if atom2 has been found in atom1's bonds
    bool foundA = false;

    // iterates through atom1's bonds until it finds atom2, then removes that bond
    for (int i = 0; i < atom1 -> bonds.size() && !foundA; i++)
    {   
        // if atom 2 is found in list bonds
        if (atom1 -> bonds[i] == atom2)
        {
            // remove atom 2, set found check as true
            atom1 -> bonds.erase(atom1 -> bonds.begin() + i);
            foundA = true;
        }
    }

    // repeats process above for atom2
    bool foundB = false;

    for (int i = 0; i < atom2 -> bonds.size() && !foundB; i++)
    {
        if (atom2 -> bonds[i] == atom1)
        {
            atom2 -> bonds.erase(atom2 -> bonds.begin() + i);
            foundB = true;
        }
    }

    // creates a variable delcaring if the molecule has been split by the bond breaking - defaults to true
    bool moleculeSplit = true;

    // creates a pointer to a vector of pointers to all atoms in molecule1 (molecule attatched to atom1)
    std::vector<Atom*>* molecule1_atomVector;
    
    // fills molecule1_atomVector with all atoms attatched to atom1
    find_molecule_tree(atom1);

    // checks each atom in the vector of atoms in the molecule atom1
    for (int i = 0; molecule1_atomVector -> size(); i++)
    {
        // if one of the atoms is molecule 2, the molecule has not split
        if (molecule1_atomVector -> at(i) == atom2)
        {
            moleculeSplit = false;
        }
    }

    // if the molecule hasn't split, return a null pointer, as no new molecules were made
    if (moleculeSplit == false)
    {
        return nullptr;
    } 
    else    // else return a pointer to a new molecule created off of the atoms connected to atom2 
    {
        // sets atom1's parent molecule's atom vector as the on found for atom1 post bond breaking
        atom1 -> parent -> atoms = *molecule1_atomVector;

        // creates a new molecule vector for atom2's molecule and fills it
        std::vector<Atom*>* molecule2_atomVector;
        find_molecule_tree(atom2);

        // initializes molecule2 from it's atom vector
        Molecule* molecule2 = new Molecule(molecule2_atomVector);

        // for all atoms in molecule, set molecule2 as their parent
        for (int i = 0; i < molecule2 -> atoms.size(); i++)
        {
            molecule2 -> atoms.at(i) -> parent = molecule2;
        }

        return molecule2;
        // fix this later -> add a destructor somewhere
    }
}

void draw_atom(Object* atom, glm::vec3 position, Camera &camera, float windowWidth, float windowHeight)
{
    atom -> uniform.uDisplacement += position;
    atom -> draw(camera, windowWidth, windowHeight);
    atom -> uniform.uDisplacement += position;
}

void draw_bond(Object* bond, glm::vec3 position1, glm::vec3 position2, Camera &camera, float windowWidth, float windowHeight)
{
    // move centre of bond to centre of space between points
    glm::vec3 direction = position2 - position1;        // AB = B - A
    glm::vec3 bondPosition = position1 + (0.5f * direction);        // R = P + (l * D)
    bond -> uniform.uDisplacement += bondPosition;

    // rotate bond accordingly
    float difference_x = position2.x - position1.x;
    float difference_y = position2.y - position1.y;
    float difference_z = position2.z - position1.z;

    // probably a divide by 0 error
    float rotationAbout_x = atanf(difference_y / difference_z);
    float rotationAbout_y = atanf(difference_z / difference_x);

    glm::vec3 totalRotation = glm::vec3(rotationAbout_x, rotationAbout_y, 0.0f);
    bond -> uniform.uRotate += totalRotation;

    // draw bond
    bond -> draw(camera, windowWidth, windowHeight);

    // reset bond position
    bond -> uniform.uDisplacement -= bondPosition;
    bond -> uniform.uDisplacement -= totalRotation;
}


/*
 - position is the centre of the sphere. imagine it as the position in world space
 - direction is the direction of the bond/vertex from this central position. this is local to the sphere.

 - find 4 equidistant points around the centre of the sphere is the same math as:
    -> finding 4 equidistant points on a sphere
    -> finding the 4 vertices of a tetrahedron

 - given all direction vectors should be unit vectors anyway, we can use the math needed to find 4 vertices of a unit sphere
    -> sphere, radius 1
    -> centre, C = (x,y,z)
    -> vertex 1, V1 = normalize(x1, y1, z1)

    -> V2 can be gotten by rotating V1 109.45 degrees around a perpendicular axis
        >> can be found with the cross product of V1 and any non-perpendicular vector

    -> V2, V3 and V4 all lie on a plane perpendicular to V1, and are eqidistant from each other
        >> can I just rotate V2 60 degrees (per point) around V1 to get the other vertices?
    -> talked to miss

    effectively, find the transformations on the atom, revert it, generate the points, then reapply



 - to draw the bonds, I want a function which when given two coordinates in 3D space draws a bond between them.
    
*/

glm::vec3 get_perpendicular(glm::vec3 initial)
{
    glm::vec4 tempVector = glm::rotate(glm::mat4(1.0f), glm::radians(10.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::vec4(initial, 0);
    glm::vec3 arbitrayVector = glm::vec3(tempVector.x, tempVector.y, tempVector.z);

    return glm::cross(initial, arbitrayVector);
}


void Molecule::draw(Camera &camera, float windowWidth, float windowHeight, glm::vec3 position, glm::vec3 direction, Atom* current)
{
    // begin by normalizing direction and setting the distance
    direction = glm::normalize(direction);
    float lambda = 5.0f;

    // means it is the first iteration 
    if (current == nullptr)
    {
        // first iteration edge case code here + setup code?
    }

    // draw atom at position
    draw_atom(&atomObject, position, camera, windowWidth, windowHeight);

    // draw first bond
    glm::vec3 nextPosition = position + (lambda * direction);
    draw_bond(&bondObject, glm::vec3(0.0f), glm::vec3(5.0f, 5.0f, 5.0f), camera, windowWidth, windowHeight);

}