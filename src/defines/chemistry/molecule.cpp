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

void draw_bond(Object* bond, glm::vec3 position1, glm::vec3 position2)
{
    // move centre of bond to centre of space between points
    glm::vec3 direction = position2 - position1;        // AB = B - A
    glm::vec3 bondPosition = position1 + (0.5f * direction);        // R = AB + (l * (AB))
    bond -> uniform.uDisplacement = bondPosition;

    // rotate bond accordingly
    float rotationAbout_x = std::acosf(direction.y / glm::length(direction));   // latitude
    if (abs(direction.y) == glm::length(direction))
    {
        rotationAbout_x = 0.0f;
    }

    // imagine a cartesian graph, where z is the x axis, and x is the y axis (looking down)
    glm::vec2 xz = glm::vec2(direction.x, direction.z); // about y
    float rotationAbout_y = std::acosf(direction.z / glm::length(xz));          // longitude
    if (direction.x < 0)
    {
        rotationAbout_y = (2 * M_PI) - rotationAbout_y;
    }
    else if (glm::length(xz) == 0)  // catch a divide by 0 error
    {
        rotationAbout_y = 0.0f;
    }
       
    glm::vec3 totalRotation = glm::vec3(rotationAbout_x, rotationAbout_y, 0.0f);
    bond -> uniform.uRotate = totalRotation;

    // set the bond's scale correctly
    float scale = glm::length(position2 - position1);
    bond -> uniform.uScale.y = scale;

    // draw bond
    bond -> draw();

    // reset bond position
    bond -> uniform.uDisplacement = glm::vec3(0.0f);
    bond -> uniform.uRotate = glm::vec3(0.0f);
    bond -> uniform.uScale.y = 1.0f;
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