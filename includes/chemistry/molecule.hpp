#pragma once

#include "chemistry/atom.hpp"

#include <vector>

// prefefining atom so that it can be referenced by molecule
struct Atom;

// A class for effectively anything rendered - functional groups will be a subclass
class Molecule
{
public: 
    std::vector<Atom*> atoms;     // vector of pointers to atoms in molecule
    std::string name = "";

    // the 3D shapes which will be drawn to represent atoms and bonds
    Sphere atomObject;
    Cylinder bondObject;


    // Constructor class. Enter the first atom of the molecule, will tell child who parent, will tell parent who child.
    Molecule(Atom* firstAtom);
    Molecule(std::vector<Atom*>* atomVector);


    // will join 2 molecules together at set atoms, or add another bond between atoms in a molecule 
    void addBond(Atom* atom1, Atom* atom2);
    Molecule* removeBond(Atom* atom1, Atom* atom2);

    int find_atom(Atom* targetAtom);
    void find_molecule_tree(Atom* currentAtom);

    void draw(Camera &camera, float windowWidth, float windowHeight, glm::vec3 position = glm::vec3(0.0f), glm::vec3 direction = glm::vec3(0.0f, 1.0f, 0.0f), Atom* current = nullptr);
};