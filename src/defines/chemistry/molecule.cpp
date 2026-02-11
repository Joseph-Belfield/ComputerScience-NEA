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
    atom -> uniform.vert.location = position;
    atom -> draw();
    atom -> uniform.vert.location = glm::vec3(0.0f);
}

void draw_bond(Cylinder* bond, glm::vec3 position1, glm::vec3 position2)
{
    bond -> draw_between(position1, position2);
}

void draw_doubleBond(Cylinder* bond, glm::vec3 position1, glm::vec3 position2)
{
    const float originalThickness = bond -> uniform.vert.scale.x;
    float init_pos1 = position1.y;
    float init_pos2 = position2.y;

    // make the bond 2/5 as thick
    bond -> uniform.vert.scale.x *= (2.0f / 5);
    bond -> uniform.vert.scale.z *= (2.0f / 5);

    // if the vectors aren't directly above each other
    if (glm::length(position1 - position2) != abs(position1.y - position2.y) && (position1.y - position2.y) != 0)
    {   
        float changed_y = -(0.5 * originalThickness);
        changed_y += 0.3 * originalThickness;

        position1.y += changed_y;
        position2.y += changed_y;
        
        bond -> draw_between(position1, position2);

        position1.y += 0.4 * originalThickness;
        position2.y += 0.4 * originalThickness;

        bond -> draw_between(position1, position2);
    }
    else
    {
        float changed_z = -(0.5 * originalThickness);
        changed_z += 0.3 * originalThickness;

        position1.z += changed_z;
        position2.z += changed_z;
        
        bond -> draw_between(position1, position2);

        position1.z += 0.4 * originalThickness;
        position2.z += 0.4 * originalThickness;

        bond -> draw_between(position1, position2);
    }

    // reset to initial
    position1.y = init_pos1;
    position2.y = init_pos2;

    bond -> uniform.vert.scale.x = originalThickness;
    bond -> uniform.vert.scale.z = originalThickness;
}

void Molecule::set_atomElement(Element element)
{
    switch(element)
    {
        case(HYDROGEN):
            atomObject.uniform.vert.scale = glm::vec3(0.69f);
            atomObject.uniform.frag.objectColor = glm::vec3(0.9f, 0.9f, 0.9f);
            break;

        case(CARBON):
            atomObject.uniform.vert.scale = glm::vec3(1.0f);
            atomObject.uniform.frag.objectColor = glm::vec3(0.9f, 0.0f, 0.0f);
            break;

        case(NITROGEN):
            atomObject.uniform.vert.scale = glm::vec3(0.70f);
            atomObject.uniform.frag.objectColor = glm::vec3(0.0f, 0.9f, 0.9f);
            break;

        case(OXYGEN):
            atomObject.uniform.vert.scale = glm::vec3(0.66f);
            atomObject.uniform.frag.objectColor = glm::vec3(0.0f, 0.0f, 0.9f);
            break;
        
        case(FLUORINE):
            atomObject.uniform.vert.scale = glm::vec3(0.64f);
            atomObject.uniform.frag.objectColor = glm::vec3(1.0f, 0.02f, 0.44f);
            break;

        case(CHLORINE):
            atomObject.uniform.vert.scale = glm::vec3(1.41f);
            atomObject.uniform.frag.objectColor = glm::vec3(0.35f, 0.9f, 0.1f);
            break;
    }

    atomObject.uniform.frag.uColor = glm::vec4(atomObject.uniform.frag.objectColor, 1.0f);
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
    glm::vec3 centre;   // centre of molecule (for lighting)
    int majorCount;     // number of large central atoms!

    bondObject.uniform.frag.uColor = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);

    switch(compound)
    {
        case(WATER):
        {
            majorCount = 1; // 1 oxygen

            set_atomElement(OXYGEN);
            draw_atom(&atomObject, currentPos);

            centre += currentPos;

            float bondAngle = (104.5f / 360.0f) * (2 * M_PI);
            bondLength = 1.5f;
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
            majorCount = 1; // 1 carbon

            set_atomElement(CARBON);
            draw_atom(&atomObject, currentPos);

            centre += currentPos;

            bondLength = 2.0f;
            for (int i = 0; i < 4; i++)
            {
                set_atomElement(HYDROGEN);

                nextPos = currentPos + (bondLength * glm::normalize(angles.tetrahedral[i]));

                draw_doubleBond(&bondObject, currentPos, nextPos);        
                draw_atom(&atomObject, nextPos);
            }

            break;
        }
        case(ETHANE):
        {
            majorCount = 2; // 1 carbon

            // draw the first carbon
            set_atomElement(CARBON);
            bondLength = 3.0f;
            draw_atom(&atomObject, currentPos);

            centre += currentPos;

            nextPos = currentPos + (bondLength * glm::normalize(angles.tetrahedral[3]));

            centre += nextPos;

            // draw the second carbon
            draw_bond(&bondObject, currentPos, nextPos);
            draw_atom(&atomObject, nextPos);

            for (int i = 0; i < 2; i++)
            {
                set_atomElement(HYDROGEN);

                // allows for first iteration around carbon 1, second iteration around carbon 2
                bondLength = 3.0f;
                currentPos = (float)i * (bondLength * glm::normalize(angles.tetrahedral[3]));

                bondLength = 2.0f;
                for (int j = 0; j < 3; j++)
                {
                    if (i == 0)
                    {
                        nextPos = currentPos + (bondLength * glm::normalize(angles.tetrahedral[j]));
                    }
                    else 
                    {
                        nextPos = currentPos - (bondLength * glm::normalize(angles.tetrahedral[j]));
                    }

                    draw_bond(&bondObject, currentPos, nextPos);
                    draw_atom(&atomObject, nextPos);
                }
            }

            break;
        }
        case(PROPANE):
        {
            majorCount = 3; // 3 carbons

            // draw the first carbon
            set_atomElement(CARBON);
            draw_atom(&atomObject, currentPos);

            centre += currentPos;

            bondLength = 2.0f;
            set_atomElement(HYDROGEN);
            for (int i = 0; i < 2; i++)
            {
                nextPos = currentPos + (bondLength * glm::normalize(angles.tetrahedral[i]));
                draw_bond(&bondObject, currentPos, nextPos);
                draw_atom(&atomObject, nextPos);
            }

            // draw the second carbon
            set_atomElement(CARBON);
            bondLength = 3.0f;
            nextPos = currentPos + (bondLength * glm::normalize(angles.tetrahedral[3]));
    
            draw_bond(&bondObject, currentPos, nextPos);
            draw_atom(&atomObject, nextPos);

            glm::vec3 tempPos = currentPos;
            currentPos = nextPos;

            centre += currentPos;

            set_atomElement(HYDROGEN);
            bondLength = 2.0f;
            for (int i = 0; i < 3; i++)
            {
                nextPos = currentPos - (bondLength * glm::normalize(angles.tetrahedral[i]));
                draw_bond(&bondObject, currentPos, nextPos);
                draw_atom(&atomObject, nextPos);
            }

            // draw the third carbon
            set_atomElement(CARBON);
            bondLength = 3.0f;
            currentPos = tempPos;
            nextPos = currentPos + (bondLength * glm::normalize(angles.tetrahedral[2]));

            draw_bond(&bondObject, currentPos, nextPos);
            draw_atom(&atomObject, nextPos);

            currentPos = nextPos;

            centre += currentPos;

            set_atomElement(HYDROGEN);
            bondLength = 2.0f;
            for (int i = 0; i < 2; i++)
            {
                nextPos = currentPos - (bondLength * glm::normalize(angles.tetrahedral[i]));
                draw_bond(&bondObject, currentPos, nextPos);
                draw_atom(&atomObject, nextPos);
            }

            nextPos = currentPos - (bondLength * glm::normalize(angles.tetrahedral[3]));
            draw_bond(&bondObject, currentPos, nextPos);
            draw_atom(&atomObject, nextPos);

            break;
        }
        case(CYCLOHEXANE):
        { 
            majorCount = 6; // 6 carbons

            // first
            set_atomElement(CARBON);
            bondLength = 3.0f;
            draw_atom(&atomObject, currentPos);

            centre += currentPos;
            set_atomElement(HYDROGEN);
            bondLength = 2.0f;
            for (int i = 0; i < 2; i++)
            {
                nextPos = currentPos + (bondLength * glm::normalize(angles.tetrahedral[i]));
                draw_bond(&bondObject, currentPos, nextPos);
                draw_atom(&atomObject, nextPos);
            }

            // second
            set_atomElement(CARBON);
            bondLength = 3.0f;
            nextPos = currentPos + (bondLength * glm::normalize(angles.tetrahedral[3]));
            draw_bond(&bondObject, currentPos, nextPos);
            draw_atom(&atomObject, nextPos);

            set_atomElement(HYDROGEN);
            bondLength = 2.0f;
            currentPos = nextPos;
            centre += currentPos;
            for (int i = 0, k = 0; i < 2; i++, k += 2)
            {
                nextPos = currentPos - (bondLength * glm::normalize(angles.tetrahedral[k]));
                draw_bond(&bondObject, currentPos, nextPos);
                draw_atom(&atomObject, nextPos);
            }

            // third 
            set_atomElement(CARBON);
            bondLength = 3.0f;
            nextPos = currentPos - (bondLength * glm::normalize(angles.tetrahedral[1]));
            draw_bond(&bondObject, currentPos, nextPos);
            draw_atom(&atomObject, nextPos);

            set_atomElement(HYDROGEN);
            bondLength = 2.0f;
            currentPos = nextPos;
            centre += currentPos;
            for (int i = 0, k = 0; i < 2; i++, k += 3)
            {
                nextPos = currentPos + (bondLength * glm::normalize(angles.tetrahedral[k]));
                draw_bond(&bondObject, currentPos, nextPos);
                draw_atom(&atomObject, nextPos);
            }

            // fourth 
            set_atomElement(CARBON);
            bondLength = 3.0f;
            nextPos = currentPos + (bondLength * glm::normalize(angles.tetrahedral[2]));
            draw_bond(&bondObject, currentPos, nextPos);
            draw_atom(&atomObject, nextPos);

            set_atomElement(HYDROGEN);
            bondLength = 2.0f;
            currentPos = nextPos;
            centre += currentPos;
            for (int i = 0; i < 2; i++)
            {
                nextPos = currentPos - (bondLength * glm::normalize(angles.tetrahedral[i]));
                draw_bond(&bondObject, currentPos, nextPos);
                draw_atom(&atomObject, nextPos);
            }

            // fifth
            set_atomElement(CARBON);
            bondLength = 3.0f;
            nextPos = currentPos - (bondLength * glm::normalize(angles.tetrahedral[3]));
            draw_bond(&bondObject, currentPos, nextPos);
            draw_atom(&atomObject, nextPos);

            set_atomElement(HYDROGEN);
            bondLength = 2.0f;
            currentPos = nextPos;
            centre += currentPos;
            for (int i = 0, k = 0; i < 2; i++, k += 2)
            {
                nextPos = currentPos + (bondLength * glm::normalize(angles.tetrahedral[k]));
                draw_bond(&bondObject, currentPos, nextPos);
                draw_atom(&atomObject, nextPos);
            }

            // sixth
            set_atomElement(CARBON);
            bondLength = 3.0f;
            nextPos = currentPos + (bondLength * glm::normalize(angles.tetrahedral[1]));
            draw_bond(&bondObject, currentPos, nextPos);
            draw_atom(&atomObject, nextPos);

            set_atomElement(HYDROGEN);
            bondLength = 2.0f;
            currentPos = nextPos;
            centre += currentPos;
            for (int i = 0, k = 0; i < 2; i++, k += 3)
            {
                nextPos = currentPos - (bondLength * glm::normalize(angles.tetrahedral[k]));
                draw_bond(&bondObject, currentPos, nextPos);
                draw_atom(&atomObject, nextPos);
            }

            // complete ring
            set_atomElement(CARBON);
            bondLength = 3.0f;
            nextPos = currentPos - (bondLength * glm::normalize(angles.tetrahedral[2]));
            draw_bond(&bondObject, currentPos, nextPos);
            break;
        }
    }   

    centre /= majorCount;
    atomObject.uniform.frag.lightPos = glm::vec3(centre.x, 5.0f, centre.z);
}
