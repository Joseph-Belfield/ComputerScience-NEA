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

struct VertexUniforms
{
    // version 0
	glm::vec3 location = glm::vec3(0.0f, 0.0f, 0.0f);   // object's displacement from origin (effectively coords on world axis)
	glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);         // object's rotation around each axis
	glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);          // growth of the object in each direction

};

struct FragmentUniforms
{
    // version1
    glm::vec4 uColor = glm::vec4(1.0f);     // sets color of object as one value

    // version 2
    glm::vec3 objectColor = glm::vec3(1.0f);             // color of the object (defaults to white)
    glm::vec3 lightColor = glm::vec3(1.0f);              // color of the light (defaults to white)
    float ambienceStrength = 0.5f;                       // amount of ambient light
    glm::vec3 lightPos = glm::vec3(0.0f, 5.0f, 0.0f);    // position of the light source
    float specularStrength = 0.5f;                       // strength of specular lighting

};

struct uniformData
{
	VertexUniforms vert;
    FragmentUniforms frag;
};

struct drawData
{
    Camera* camera = nullptr;
    float width = 0.0f;
    float height = 0.0f;

    bool init = false;
};

// for each induvidual object
class Object
{
public:
	meshData mesh;
	uniformData uniform;
    drawData drawInfo;

    // type of object
    Subclass subclass;

    // default constructor
    Object();

    // initializes info needed for drawing
    void init_drawInfo(Camera* camera, float width, float height);

    // draw functions
    void draw();
};

class ReferencePlane: public Object
{
public:
    ReferencePlane(GLfloat initHeight = -1.0f, glm::vec3 initColor = glm::vec3(1.0f, 1.0f, 1.0f), GLfloat initScale = 1.0f, const GLuint stripCount = 20, std::string source_vertexShader = "version1", std::string source_fragmentShader = "version1");
};

class Sphere: public Object
{
public:
    // constructor
	Sphere(std::string source_vertexShader = "version2.vs", std::string source_fragmentShader = "version2.fs", const GLfloat radius = (1.0f), const GLuint stacks = (50), const GLuint sectors = (25), const glm::vec3 initColor = glm::vec3(1.0f, 0.0f, 0.0f), const glm::vec3 initLocation = glm::vec3(0.0f, 0.0f, 0.0f), const GLfloat initScale = 1.0f);  
};

class Cylinder: public Object
{
public:
    // constructor 
    Cylinder(std::string source_vertexShader = "version1.vs", std::string source_fragmentShader = "version1.fs", const GLfloat radius = (0.25f), const GLfloat height = (1.0f), const GLuint sectors = (50), const glm::vec3 initColor = glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3 initLocation = glm::vec3(0.0f, 0.0f, 0.0f), const glm::vec3 initRotation = glm::vec3(0.0f, 0.0f, 0.0f), const glm::vec3 initScale = glm::vec3(1.0f));

    void draw_between(glm::vec3 position1, glm::vec3 position2);
};

class Cube: public Object
{
public:
    // constructor
    Cube(std::string source_vertexShader = "version1.vs", std::string source_fragmentShader = "version1.fs", const GLfloat height = (1.0f), const glm::vec3 initColor = glm::vec3(1.0f), glm::vec3 initLocation = glm::vec3(0.0f), glm::vec3 initRotation = glm::vec3(0.0f), glm::vec3 initScale = glm::vec3(1.0f));
};