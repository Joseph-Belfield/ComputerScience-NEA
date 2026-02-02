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

