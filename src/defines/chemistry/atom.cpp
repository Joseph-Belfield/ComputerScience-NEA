#include "chemistry/atom.hpp"

#include <string>
#include <vector>

Atom::Atom(Element element)
{
    switch(element)
    {
        case(HYDROGEN):
            element = HYDROGEN;
            atomicRadius = 1.0f;
            color = glm::vec3(0.9f);
            maxBonds = 1;
            break;
        
        case(CARBON):
            element = CARBON;
            atomicRadius = 1.0f;
            color = glm::vec3(0.9f, 0.0f, 0.0f);
            maxBonds = 4;
            bondAngle = 90.0f;
            break;

        case(OXYGEN):
            element = OXYGEN;
            atomicRadius = 1.0f;
            color = glm::vec3(0.0f, 0.0f, 0.9f);
            maxBonds = 2;
            bondAngle = 104.5f;
            break;
    };


}

// Draws the atom! Very simple currently.
void Atom::draw_atom()
{
    atomObject.draw();
}


glm::vec3 find_bondVector(Atom* atom1, Atom* atom2)
{
    
}

// draws all bonds connected to an atom
void Atom::draw_bond(Atom* target)
{
    // have a reference cylinder
    // move and scale it, draw it, then move back and revert changes
    // repeat until no more bonds

    for (int i = 0; i < bonds.size(); i++)
    {
        
    }
}