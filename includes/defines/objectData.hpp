#include "glad/gl.h"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

#include <vector>

class meshData
{
public:
    // unsigned ints as identifiers for the objects (because C-based language)
    GLuint vertexArrayObject = 0;
    GLuint vertexBufferObject = 0;
    GLuint indexBufferObject = 0;

    // shader program used by the object
    GLuint shaderProgram = 0;

    // use of GLfloat as it is more cross-platform (likely won't matter but best practice)
    std::vector<GLfloat> vertexData;   // lives on CPU

    // data about the order vertices should be renderd in
    std::vector<GLuint> indexData;
};

struct uniformData
{
	// object's displacement from origin (effectively coords on world axis)
	glm::vec3 uDisplacement;

	// object's rotation around each axis
	glm::vec3 uRotate;

	// growth of the object in each direction
	glm::vec3 uScale;

    // change in color values of object
    glm::vec4 uColor;
};

// for each induvidual object
class objectData
{
public:
	meshData mesh;
	uniformData uniform;

    // constructor
    objectData();
};

class Sphere: public objectData
{
public:
    // constructor
	Sphere(const float radius = (0.5f), const GLuint stacks = (18), const GLuint sectors = (9), const glm::vec4 initColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec3 initLocation = glm::vec3(1.0f, 0.0f, 0.0f), float initScale = 1.0f);
    
};