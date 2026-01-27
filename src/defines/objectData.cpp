#include "defines/objectData.hpp"
#include "runLoop.hpp"

#include "glad/gl.h"

#include "glm/glm.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/gtc/matrix_transform.hpp"    

#include <vector>
#include <math.h>


Object::Object() {};

// Creates a model matrix.
// - Objects begin in local space, where they are created on their own set of axis
// - The model matrix moves objects from local space to world space, where objects are all held relative to one shared set of axis
//
// The model matrix is also edited accordingly to change an objects position/rotation in world space accordingly.
void Object::model_matrix(GLuint shaderProgram)
{
    // create and adapt the matrix to adjust the following transformations
    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), this -> uniform.uDisplacement); // movement

    // rotations
    modelMatrix = glm::rotate(modelMatrix ,glm::radians(this -> uniform.uRotate.x), glm::vec3(1.0f, 0.0f, 0.0f));  // X
    modelMatrix = glm::rotate(modelMatrix ,glm::radians(this -> uniform.uRotate.y), glm::vec3(0.0f, 1.0f, 0.0f));  // Y
    modelMatrix = glm::rotate(modelMatrix ,glm::radians(this -> uniform.uRotate.z), glm::vec3(0.0f, 0.0f, 1.0f));  // Z

    modelMatrix = glm::scale(modelMatrix, this -> uniform.uScale);

    GLuint location_modelMatrix = create_uniform_mat4(shaderProgram, "uModelMatrix", 1, false, modelMatrix);
}

void Object::draw_polygon(GLuint shaderProgram)
{
	// sets the objets model matrix
	this -> model_matrix(shaderProgram);

	// choose VAO and VBO
    glBindVertexArray(this -> mesh.vertexArrayObject);

    // draw
    glDrawElements
    (
        GL_TRIANGLES,                       // shape
        this -> mesh.indexData.size(),      // number of vertices drawn to (count repeats)
        GL_UNSIGNED_INT,                    // data type
        (void*)0                            // offset into index array for first element (triangle vertex order)
    );  

    // unbind VAO after shape drawn
    glBindVertexArray(0);
}

void Object::draw_lines(GLuint shaderProgram)
{
	// sets the objets model matrix
	this -> model_matrix(shaderProgram);

	// choose VAO and VBO
    glBindVertexArray(this -> mesh.vertexArrayObject);

    // draw
    glDrawElements
    (
        GL_LINES,                     	    // shape
        this -> mesh.indexData.size(),      // number of vertices drawn to (count repeats)
        GL_UNSIGNED_INT,                    // data type
        (void*)0                            // offset into index array for first element (triangle vertex order)
    );  

    // unbind VAO after shape drawn
    glBindVertexArray(0);
}

// calculates the coordinates of all points of a sphere object
std::vector<glm::vec3> calculateSphereVertices(const float radius, const GLuint stackCount, const GLuint sectorCount)
{	

	// array of all vertices in sphere (stored as vec3)
	std::vector<glm::vec3> vertices;

	for (int stackIndex = 0; stackIndex <= stackCount; stackIndex++)
	{
		// angle from ahead (range -90 -> 90)
		float A = (M_PI / 2) - (M_PI *((float)stackIndex / stackCount));

		// find the y value. trigf functions use floats rather than doubles.
		float y = radius * sinf(A);

		// find the first step of both the x and z value
		float xz = radius * cosf(A);

		for (int sectorIndex = 0; sectorIndex <= sectorCount; sectorIndex++)
		{
			// angle from straight ahead (range 0 -> 360)
			float B = 2 * M_PI * ((float)sectorIndex / sectorCount);

			float x = xz * cosf(B);
			float z = xz * sinf(B);

			// create the coordinate
			glm::vec3 coordinate = glm::vec3(x, y, z);
			
			vertices.push_back(coordinate);
		}
	}

	return vertices;
}

void calculateSphereIndexData(std::vector<GLuint>& indexData, const GLuint stacks, const GLuint sectors)
{	
	GLint vertex1, vertex2;

	for (int stackIndex = 0; stackIndex < stacks; stackIndex++)
	{
		vertex1 = stackIndex * (sectors + 1);	// beginning of current stack
		vertex2 = vertex1 + sectors + 1;		// beginning of next stack

		for (int sectorIndex = 0; sectorIndex < sectors; sectorIndex++, vertex1++, vertex2++)
		{
			// indexData for 2 triangles per sector (except for the k1 and k2 stacks, which only have 1)

			// if not the first stack, make the first set of triangles
			if (stackIndex != 0)
			{
				indexData.push_back(vertex1);
				indexData.push_back(vertex2);
				indexData.push_back(vertex1 + 1);
			}

			// if not the last stack, make the second set of triangles
			if (stackIndex != (stacks - 1))
			{
				indexData.push_back(vertex1 + 1);
				indexData.push_back(vertex2);
				indexData.push_back(vertex2 + 1);
			}
		}
	}
}

// Creates a sphere object that can be passed to the GPU.
//
// - Radius is the radius of the sphere in local coordinates (defaults 0.5f)
// - Stacks in the number of vertical strips the sphere is divided into (defaults to 18)
// - Sectors is the number of horizontal strips the sphere is divided into (defaults to 9)
// - initColor is the starting color of the object in RGBA values (from 0 -> 1)
// - initLocation is the starting position of the sphere in world space (X, Y, Z)
// - initScale is the initial scale of the sphere (defaults to 1)
Sphere::Sphere(const GLfloat radius, const GLuint stacks, const GLuint sectors, const glm::vec4 initColor, glm::vec3 initLocation, GLfloat initScale)
{
	subclass = SPHERE;

	// clears the index data vecto
	mesh.vertexData.clear();

	// esnures vector is clear
	mesh.indexData.clear();

	std::vector<glm::vec3> vertices = calculateSphereVertices(radius, stacks, sectors); // generates vertices								
	for (int i = 0; i < vertices.size(); i++)											// fills vertexData
	{
		mesh.vertexData.push_back(vertices.at(i).x);
		mesh.vertexData.push_back(vertices.at(i).y);
		mesh.vertexData.push_back(vertices.at(i).z);
		mesh.vertexData.push_back(initColor.r);			// R
		mesh.vertexData.push_back(initColor.g);			// G
		mesh.vertexData.push_back(initColor.b);			// B
		mesh.vertexData.push_back(initColor.a);				// A
	}
	

	// generate Vertex Array Objects 
	glGenVertexArrays(1, &(mesh.vertexArrayObject));           // creates an array to hold vertex data (called vertexArrayObject)
	glBindVertexArray(mesh.vertexArrayObject);                 // selects the array as current


	// generate Vertex Buffer Object for position
	glGenBuffers(1, &(mesh.vertexBufferObject));               // generates buffer
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vertexBufferObject);    // sets buffer as current, specifies target
	glBufferData
	(
		GL_ARRAY_BUFFER,                          							// specifies target
		mesh.vertexData.size() * sizeof(GLfloat),      	 				   	// finds the size (in bytes) of vertex data
		mesh.vertexData.data(),                       		  			  	// pointer to the array holding the data of the vector
		GL_STATIC_DRAW                             							// sets intentions with data
	);
	

	// fills indexData with correct index information
	calculateSphereIndexData(mesh.indexData, stacks, sectors);


	// set up Element/Index Buffer Object (EBO / IBO) - holds the index for the order in which vertices are drawn
	glGenBuffers(1,&(mesh.indexBufferObject));                         // generate EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.indexBufferObject);   // sets buffer type as element buffer
	glBufferData
	(
		GL_ELEMENT_ARRAY_BUFFER,                        		// target
		mesh.indexData.size() * sizeof(GLuint),    		   		// size
		mesh.indexData.data(),                        			// data
		GL_STATIC_DRAW                                			// usage
	);

	
	// setup position VAO
	glEnableVertexAttribArray(0); // enables the 0th attribute - AKA. this is the first VAO
	glVertexAttribPointer
	(
		0,                        // index into vector of VAOs
		3,                        // pieces of data (per vertex: x, y, z)
		GL_FLOAT,                 // data type
		GL_FALSE,                 // normalized?
		sizeof(GLfloat) * 7,      // stride (no. of bytes) to jump from first (type) data of v1 to first (type) data of v2, etc         
		(GLvoid*)0                // pointer for offset - irrelivent as position data is in first slot
	);


	// setup color VAO
	glEnableVertexAttribArray(1); // enables the 1st attribute - AKA. this is the second VAO
	glVertexAttribPointer
	(
		1,                                 // index into vector of VAOs
		4,                                 // pieces of data (per vertex: r, g, b, a)
		GL_FLOAT,                          // data type
		GL_FALSE,                          // normalized?
		sizeof(GLfloat) * 7,               // stride (byte offset) between firsts of same data (ie: between r1 and r2)       
		(GLvoid*)(sizeof(GLfloat) * 3)     // pointer for offset - starting position for first of that data type (address)
	);


	// cleanup
	glBindVertexArray(0);                		// unbind currently bound VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);     		// unbind currently bound VBO


	// set up uniforms
	uniform.uDisplacement = initLocation;
	uniform.uScale = glm::vec3(initScale);
}


std::vector<glm::vec3> calculateReferencePlaneVertices(const GLuint stripCount)
{
	// create vertices
	std::vector<glm::vec3> vertices;
	// reserves enough space
	vertices.reserve(stripCount * stripCount);

	for (int x = 0; x <= stripCount; x++)
	{
		// for each x value, pushes a coord with the max and min z values
		vertices.push_back(glm::vec3(x, 0, -stripCount / 2));
		vertices.push_back(glm::vec3(x, 0, stripCount / 2));
	}

	for (int z = 0; z <= stripCount; z++)
	{
		// for each x value, pushes a coord with the max and min z values
		vertices.push_back(glm::vec3(-stripCount / 2, 0, z));
		vertices.push_back(glm::vec3(stripCount / 2, 0, z));
	}

	return vertices;
}

void calculateReferencePlaneIndexData(std::vector<GLuint>& indexData, const GLuint stripCount)
{
	for (int i = 0; i < (2 * stripCount); i++)
	{
		indexData.push_back((2 * i));
		indexData.push_back((2 * i) + 1);
	}
}

// Creates a plane of referenece using a grid system
//
// - initHeight sets the starting y-value of the plane
// - initColor sets the starting color of the plane (only rgb -> alpha predetermined)
// - initScale sets the starting distance between lines
ReferencePlane::ReferencePlane(GLfloat initHeight, const glm::vec4 initColor, GLfloat initScale, const GLuint stripCount)
{
	subclass = REFERENCE_PLANE;

	// ensures data vectors are clear
	mesh.vertexData.clear();
	mesh.indexData.clear();

	mesh.drawType = 1;

	std::vector<glm::vec3> vertices = calculateReferencePlaneVertices(stripCount);

	mesh.vertexData.reserve(stripCount * stripCount);
	for (int i = 0; i < vertices.size(); i++)
	{
		mesh.vertexData.push_back(vertices[i].x);
		mesh.vertexData.push_back(vertices[i].y);
		mesh.vertexData.push_back(vertices[i].z);
		mesh.vertexData.push_back(initColor.r);			// R
		mesh.vertexData.push_back(initColor.g);			// G
		mesh.vertexData.push_back(initColor.b);			// B
		mesh.vertexData.push_back(initColor.a);			// A
	}

	// generate Vertex Array Objects 
	glGenVertexArrays(1, &(mesh.vertexArrayObject));           // creates an array to hold vertex data (called vertexArrayObject)
	glBindVertexArray(mesh.vertexArrayObject);                 // selects the array as current


	// generate Vertex Buffer Object for position
	glGenBuffers(1, &(mesh.vertexBufferObject));               // generates buffer
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vertexBufferObject);    // sets buffer as current, specifies target
	glBufferData
	(
		GL_ARRAY_BUFFER,                          							// specifies target
		mesh.vertexData.size() * sizeof(GLfloat),      	 				   	// finds the size (in bytes) of vertex data
		mesh.vertexData.data(),                       		  			  	// pointer to the array holding the data of the vector
		GL_STATIC_DRAW                             							// sets intentions with data
	);

	mesh.indexData.reserve(stripCount * stripCount);
	calculateReferencePlaneIndexData(mesh.indexData, stripCount);

	// set up Element/Index Buffer Object (EBO / IBO) - holds the index for the order in which vertices are drawn
	glGenBuffers(1,&(mesh.indexBufferObject));                         // generate EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.indexBufferObject);   // sets buffer type as element buffer
	glBufferData
	(
		GL_ELEMENT_ARRAY_BUFFER,                        		// target
		mesh.indexData.size() * sizeof(GLuint),    		   		// size
		mesh.indexData.data(),                        			// data
		GL_STATIC_DRAW                                			// usage
	);

	
	// setup position VAO
	glEnableVertexAttribArray(0); // enables the 0th attribute - AKA. this is the first VAO
	glVertexAttribPointer
	(
		0,                        // index into vector of VAOs
		3,                        // pieces of data (per vertex: x, y, z)
		GL_FLOAT,                 // data type
		GL_FALSE,                 // normalized?
		sizeof(GLfloat) * 7,      // stride (no. of bytes) to jump from first (type) data of v1 to first (type) data of v2, etc         
		(GLvoid*)0                // pointer for offset - irrelivent as position data is in first slot
	);


	// setup color VAO
	glEnableVertexAttribArray(1); // enables the 1st attribute - AKA. this is the second VAO
	glVertexAttribPointer
	(
		1,                                 // index into vector of VAOs
		4,                                 // pieces of data (per vertex: r, g, b, a)
		GL_FLOAT,                          // data type
		GL_FALSE,                          // normalized?
		sizeof(GLfloat) * 7,               // stride (byte offset) between firsts of same data (ie: between r1 and r2)       
		(GLvoid*)(sizeof(GLfloat) * 3)     // pointer for offset - starting position for first of that data type (address)
	);

	// cleanup
	glBindVertexArray(0);                		// unbind currently bound VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);     		// unbind currently bound VBO


	// set up uniforms
	uniform.uDisplacement = glm::vec3(0.0f, initHeight, 0.0f);
	uniform.uScale = glm::vec3(5.0f);
}




std::vector<glm::vec3> calculateCylinderVertices(const GLfloat radius, const GLfloat height, const GLuint sectorCount)
{
	// array of all vertices in sphere (stored as vec3)
	std::vector<glm::vec3> vertices;

	// to generate the top and bottom circle
	for (int i = 0; i < 2; i++)
	{
		// for top and bottom of circle
		float y = -(height / 2) + (i * height);

		vertices.push_back(glm::vec3(0.0f, y, 0.0f));	// centre of the circle

		for (int sectorIndex = 0; sectorIndex <= sectorCount; sectorIndex++)
		{
			// angle from straight ahead (range 0 -> 360)
			float angle = 2 * M_PI * ((float)sectorIndex / sectorCount);

			float x =  radius * cosf(angle);
			float z =  radius * sinf(angle);

			// create the coordinate
			glm::vec3 coordinate = glm::vec3(x, y, z);
			
			vertices.push_back(coordinate);
		}
	}

	return vertices;
}


void calculateCylinderIndexData(std::vector<GLuint>& indexData, GLuint sectorCount)
{
	GLuint baseCentre = 0;
	GLuint topCentre = sectorCount + 2;

	// i is the iterator, k is the value
	for (int i = 0, k = 1; i < sectorCount; i++, k++)
	{
		if (i < sectorCount - 1)
		{
			// BASE
			indexData.push_back(baseCentre);
			indexData.push_back(k);
			indexData.push_back(k + 1);

			// TOP
			indexData.push_back(topCentre);
			indexData.push_back(topCentre + k);
			indexData.push_back(topCentre + k + 1);

			// SIDE
			indexData.push_back(k);
			indexData.push_back(k + 1);
			indexData.push_back(topCentre + k + 1);

			indexData.push_back(topCentre + k + 1);
			indexData.push_back(topCentre + k);
			indexData.push_back(k);
		}
		else	// wrap around layer
		{
			
			// BASE
			indexData.push_back(baseCentre);
			indexData.push_back(sectorCount);
			indexData.push_back(baseCentre + 1);

			// TOP
			indexData.push_back(topCentre);
			indexData.push_back(topCentre + sectorCount);
			indexData.push_back(topCentre + 1);

			// SIDE
			indexData.push_back(baseCentre + sectorCount);
			indexData.push_back(baseCentre + 1);
			indexData.push_back(topCentre + 1);

			indexData.push_back(topCentre + 1);
			indexData.push_back(topCentre + sectorCount);
			indexData.push_back(baseCentre + sectorCount);

		}
	}
}


// Constructor for a cylinder object.
//
// - radius is the radius of the cylinder
// - length is the length of the cylinder
// - sectors is the number of triangles the circle of the cylinder is made up of (and hence the detail)
Cylinder::Cylinder(const GLfloat radius, const GLfloat height, const GLuint sectorCount, const glm::vec4 initColor, glm::vec3 initLocation, const glm::vec3 initRotation, const glm::vec3 initScale)
{
	subclass = CYLINDER;

	mesh.vertexData.clear();
	mesh.indexData.clear();

	// draws triangles
	mesh.drawType = 0;

	std::vector<glm::vec3> vertices = calculateCylinderVertices(radius, height, sectorCount);
	for (int i = 0; i < vertices.size(); i++)
	{
		mesh.vertexData.push_back(vertices[i].x);
		mesh.vertexData.push_back(vertices[i].y);
		mesh.vertexData.push_back(vertices[i].z);
		mesh.vertexData.push_back(initColor.r);			// R
		mesh.vertexData.push_back(initColor.g);			// G
		mesh.vertexData.push_back(initColor.b);			// B
		mesh.vertexData.push_back(initColor.a);				// A
	}

	// generate Vertex Array Objects 
	glGenVertexArrays(1, &(mesh.vertexArrayObject));           // creates an array to hold vertex data (called vertexArrayObject)
	glBindVertexArray(mesh.vertexArrayObject);                 // selects the array as current


	// generate Vertex Buffer Object for position
	glGenBuffers(1, &(mesh.vertexBufferObject));               // generates buffer
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vertexBufferObject);    // sets buffer as current, specifies target
	glBufferData
	(
		GL_ARRAY_BUFFER,                          							// specifies target
		mesh.vertexData.size() * sizeof(GLfloat),      	 				   	// finds the size (in bytes) of vertex data
		mesh.vertexData.data(),                       		  			  	// pointer to the array holding the data of the vector
		GL_STATIC_DRAW                             							// sets intentions with data
	);

	// calculate index data for cylinder
	calculateCylinderIndexData(mesh.indexData, sectorCount);

	// set up Element/Index Buffer Object (EBO / IBO) - holds the index for the order in which vertices are drawn
	glGenBuffers(1,&(mesh.indexBufferObject));                         // generate EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.indexBufferObject);   // sets buffer type as element buffer
	glBufferData
	(
		GL_ELEMENT_ARRAY_BUFFER,                        		// target
		mesh.indexData.size() * sizeof(GLuint),    		   		// size
		mesh.indexData.data(),                        			// data
		GL_STATIC_DRAW                                			// usage
	);

	
	// setup position VAO
	glEnableVertexAttribArray(0); // enables the 0th attribute - AKA. this is the first VAO
	glVertexAttribPointer
	(
		0,                        // index into vector of VAOs
		3,                        // pieces of data (per vertex: x, y, z)
		GL_FLOAT,                 // data type
		GL_FALSE,                 // normalized?
		sizeof(GLfloat) * 7,      // stride (no. of bytes) to jump from first (type) data of v1 to first (type) data of v2, etc         
		(GLvoid*)0                // pointer for offset - irrelivent as position data is in first slot
	);


	// setup color VAO
	glEnableVertexAttribArray(1); // enables the 1st attribute - AKA. this is the second VAO
	glVertexAttribPointer
	(
		1,                                 // index into vector of VAOs
		4,                                 // pieces of data (per vertex: r, g, b, a)
		GL_FLOAT,                          // data type
		GL_FALSE,                          // normalized?
		sizeof(GLfloat) * 7,               // stride (byte offset) between firsts of same data (ie: between r1 and r2)       
		(GLvoid*)(sizeof(GLfloat) * 3)     // pointer for offset - starting position for first of that data type (address)
	);

	// cleanup
	glBindVertexArray(0);                		// unbind currently bound VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);     		// unbind currently bound VBO


	// set up uniforms
	uniform.uDisplacement = initLocation;
	uniform.uRotate = initRotation;
	uniform.uScale = initScale;
}