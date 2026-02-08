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

void Molecule::set_atomElement(Element element)
{
    switch(element)
    {
        case(HYDROGEN):
            atomObject.uniform.uScale = glm::vec3(0.69f);
            atomObject.uniform.uColor = glm::vec4(0.9f, 0.9f, 0.9f, 1.0f);
            break;

        case(CARBON):
            atomObject.uniform.uScale = glm::vec3(1.0f);
            atomObject.uniform.uColor = glm::vec4(0.9f, 0.0f, 0.0f, 1.0f);
            break;

        case(NITROGEN):
            atomObject.uniform.uScale = glm::vec3(0.70f);
            atomObject.uniform.uColor = glm::vec4(0.0f, 0.9f, 0.9f, 1.0f);
            break;

        case(OXYGEN):
            atomObject.uniform.uScale = glm::vec3(0.66f);
            atomObject.uniform.uColor = glm::vec4(0.0f, 0.0f, 0.9f, 1.0f);
            break;
        
        case(FLUORINE):
            atomObject.uniform.uScale = glm::vec3(0.64f);
            atomObject.uniform.uColor = glm::vec4(1.0f, 0.02f, 0.44f, 1.0f);
            break;

        case(CHLORINE):
            atomObject.uniform.uScale = glm::vec3(1.41f);
            atomObject.uniform.uColor = glm::vec4(0.35f, 0.9f, 0.1f, 1.0f);
            break;
    }

    // ensures the bond length stays reasonable
    if (bondLength > 4 * atomObject.uniform.uScale.x)
    {
        bondLength = 3 * atomObject.uniform.uScale.x;
    }
    else if (bondLength < 2 * atomObject.uniform.uScale.x)
    {
        bondLength = 3 * atomObject.uniform.uScale.x;
    }
}

void Molecule::draw(glm::vec3 position, glm::vec3 direction, Atom* current)
{
    // begin by normalizing direction and setting the distance
    direction = glm::normalize(direction);

    // means it is the first iteration 
    if (current == nullptr)
    {
        current = atoms[0];
    }
    
    glm::vec3 nextPosition = position + (bondLength * glm::normalize(angles.tetrahedral[0]));

    // draw atom at position
    draw_atom(&atomObject, position);
}

void Molecule::draw(Compound compound)
{
    glm::vec3 currentPos = glm::vec3(0.0f);
    glm::vec3 nextPos;

    bondObject.uniform.uColor = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);

    switch(compound)
    {
        case(WATER):
        {
            set_atomElement(OXYGEN);
            draw_atom(&atomObject, currentPos);

            float bondAngle = (104.5f / 360.0f) * (2 * M_PI);
            for (int i = 0; i < 2; i++)
            {
                set_atomElement(HYDROGEN);

                // get the rotation for the atoms
                glm::vec4 tempVec = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f) * glm::rotate(glm::mat4(1.0f), (bondAngle / 2) - (i * bondAngle), glm::vec3(0.0f, 0.0f, 1.0f));
                glm::vec3 directionVec = glm::vec3(tempVec);

                nextPos = currentPos + (bondLength * glm::normalize(directionVec));

                draw_bond(&bondObject, currentPos, nextPos);
                draw_atom(&atomObject, nextPos);
            }

            break;
        }
        case(METHANE):
        {
            set_atomElement(CARBON);
            draw_atom(&atomObject, currentPos);

            for (int i = 0; i < 4; i++)
            {
                set_atomElement(HYDROGEN);

                nextPos = currentPos + (bondLength * glm::normalize(angles.tetrahedral[i]));

                draw_bond(&bondObject, currentPos, nextPos);        
                draw_atom(&atomObject, nextPos);
            }
            break;
        }
        case(ETHANE):
        {
            break;
        }
        case(PROPANE):
        {
            break;
        }
        case(CYCLOHEXANE):
        { 
            break;
        }
    }   
}
