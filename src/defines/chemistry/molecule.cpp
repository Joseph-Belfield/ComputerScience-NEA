#include "chemistry/molecule.hpp"

#include "glm/gtc/matrix_transform.hpp"

#include <string>
#include <iostream>
#include <cmath>
#include <vector>

Molecule::Molecule(std::vector<Atom*> &atomVec)
{
    atoms = atomVec;    // adds atoms to the molecule

    for (int i = 0; i < atoms.size(); i++)
    {
        atoms[i] -> parent = this;    // make this molecule the parent
    }
}

void Molecule::init_drawInfo(Camera* camera, float width, float height)
{
    atomObject.init_drawInfo(camera, width ,height);
    bondObject.init_drawInfo(camera, width, height);
}

void draw_atom(Object* atom, glm::vec3 position)
{
    atom -> uniform.uDisplacement = position;
    atom -> draw();
    atom -> uniform.uDisplacement = glm::vec3(0.0f);
}

void draw_bond(Cylinder* bond, glm::vec3 position1, glm::vec3 position2)
{
    bond -> draw_between(position1, position2);
}

void Molecule::draw(glm::vec3 position, glm::vec3 direction, Atom* current)
{
    // begin by normalizing direction and setting the distance
    direction = glm::normalize(direction);
    float lambda = 5.0f;

    // means it is the first iteration 
    if (current == nullptr)
    {
        current = atoms[0];
    }
    
    glm::vec3 nextPosition = position + (lambda * glm::normalize(angles.tetrahedral[0]));

    // draw atom at position
    draw_atom(&atomObject, position);
}

void Molecule::draw_CH4(glm::vec3 position)
{
    float lambda = 3.0f;
    glm::vec3 nextPosition;

    atomObject.uniform.uColor = glm::vec4(1.0f, 0.0f, 0.0, 1.0f);
    bondObject.uniform.uColor = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);

    // draw atom at position
    draw_atom(&atomObject, position);
    
    for (int i = 0; i < 4; i++)
    {
        nextPosition = position + (lambda * glm::normalize(angles.tetrahedral[i]));
        draw_bond(&bondObject, position, nextPosition);
        draw_atom(&atomObject, nextPosition);
    }
}