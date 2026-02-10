#include "chemistry/atom.hpp"

#include <string>
#include <vector>

Atom::Atom(Element element)
{
    switch(element)
    {
        case(HYDROGEN):
        {
            element = HYDROGEN;
            atomicRadius = 0.69f;
            color = glm::vec3(0.9f);
            maxBonds = 1;
            break;
        }
        case(CARBON):
        {
            element = CARBON;
            atomicRadius = 1.0f;
            color = glm::vec3(0.9f, 0.0f, 0.0f);
            maxBonds = 4;
            bondAngle = 90.0f;
            break;
        }
        case(NITROGEN):
        {
            element = NITROGEN;
            atomicRadius = 0.7f;
            color = glm::vec3(0.0f, 0.9f, 0.9f);
            maxBonds = 3;
            bondAngle = 120.0f;
            break;
        }
        case(OXYGEN):
        {
            element = OXYGEN;
            atomicRadius = 0.66f;
            color = glm::vec3(0.0f, 0.0f, 0.9f);
            maxBonds = 2;
            bondAngle = 104.5f;
            break;
        }
        case(FLUORINE):
        {
            element = FLUORINE;
            atomicRadius = 0.64f;
            color = glm::vec3(1.0f, 0.02f, 0.44f);
            maxBonds = 1;
            break;
        }
        case(CHLORINE):
        {
            element = CHLORINE;
            atomicRadius = 1.41f;
            color = glm::vec3(0.35f, 0.9f, 0.1f);
            maxBonds = 1;
            break;
        }
    };


}

