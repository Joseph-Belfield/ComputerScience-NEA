#pragma once

#include "glad/gl.h"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

#include "render/shaderClass.hpp"

#include <vector>

enum drawType
{
    TRIANGLE,
    LINE
};

enum Subclass
{
    SPHERE,
    REFERENCE_PLANE,
    CYLINDER,
    CUBE
};

class meshData
{
public:
    // unsigned ints as identifiers for the objects (because C-based language)
    GLuint vertexArrayObject = 0;
    GLuint vertexBufferObject = 0;
    GLuint indexBufferObject = 0;

    // use of GLfloat as it is more cross-platform (likely won't matter but best practice)
    std::vector<GLfloat> vertexData;   // lives on CPU

    // data about the order vertices should be renderd in
    std::vector<GLuint> indexData;

    Shader objectShader;
};

struct uniformData
{
	// object's displacement from origin (effectively coords on world axis)
	glm::vec3 uDisplacement = glm::vec3(0.0f, 0.0f, 0.0f);

	// object's rotation around each axis
	glm::vec3 uRotate = glm::vec3(0.0f, 0.0f, 0.0f);

	// growth of the object in each direction
	glm::vec3 uScale = glm::vec3(1.0f, 1.0f, 1.0f);

    // change in color values of object
    glm::vec3 uColor = glm::vec3(1.0f, 0.0f, 0.0f);

    float uOpacity = 1.0f;
};

// for each induvidual object
class Object
{
public:
	meshData mesh;
	uniformData uniform;

    // type of object
    Subclass subclass;

    // coordinates of object in 3D space
    glm::vec4 coordinates = glm::vec4(0.0f);

    // constructor
    Object();

    // uniform handling
    glm::mat4 update_modelMatrix();

    // draw functions
    void draw();
};

class ReferencePlane: public Object
{
public:
    ReferencePlane(GLfloat initHeight = -1.0f, glm::vec3 initColor = glm::vec3(1.0f, 1.0f, 1.0f), GLfloat initScale = 1.0f, const GLuint stripCount = 20, std::string source_vertexShader = "NULL", std::string source_fragmentShader = "NULL");
};

class Sphere: public Object
{
public:
    // constructor
	Sphere(const GLfloat radius = (0.5f), const GLuint stacks = (18), const GLuint sectors = (9), const glm::vec3 initColor = glm::vec3(1.0f, 0.0f, 0.0f), const glm::vec3 initLocation = glm::vec3(1.0f, 0.0f, 0.0f), const GLfloat initScale = 1.0f, std::string source_vertexShader = "NULL", std::string source_fragmentShader = "NULL");  
};

class Cylinder: public Object
{
public:
    // the vector that represents the cylinder in 3D space (from bottom to top, through centre)
    glm::vec3 directionVector;

    // constructor 
    Cylinder(const GLfloat radius = (0.25f), const GLfloat height = (2.0f), const GLuint sectors = (50), const glm::vec3 initColor = glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3 initLocation = glm::vec3(0.0f, 0.0f, 0.0f), const glm::vec3 initRotation = glm::vec3(0.0f, 0.0f, 0.0f), const glm::vec3 initScale = glm::vec3(1.0f), std::string source_vertexShader = "NULL", std::string source_fragmentShader = "NULL");
};

class Cube: public Object
{
public:
    // constructor
    Cube(const GLfloat height = (1.0f), const glm::vec3 initColor = glm::vec3(1.0f), glm::vec3 initLocation = glm::vec3(0.0f), glm::vec3 initRotation = glm::vec3(0.0f), glm::vec3 initScale = glm::vec3(1.0f), std::string source_vertexShader = "NULL", std::string source_fragmentShader = "NULL");
};