#include "defines/objectData.hpp"

#include "glad/gl.h"
#include "glm/glm.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

#include <vector>
#include <math.h>


objectData::objectData() {};

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

void calculateIndexData(std::vector<GLuint>& indexData, const GLuint stacks, const GLuint sectors)
{	
	GLint vertex1, vertex2;

	for (int stackIndex = 0; stackIndex < stacks; stackIndex++)
	{
		vertex1 = stackIndex * (sectors + 1);	// beginning of current stack
		vertex2 = vertex1 + sectors + 1;		// beginning of next stack

		for (int sectorIndex = 0; sectorIndex < sectors; sectorIndex++, vertex1++, vertex2++)
		{
			// indices for 2 triangles per sector (except for the top and bottom stacks, which only have 1)

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
Sphere::Sphere(const float radius, const GLuint stacks, const GLuint sectors, const glm::vec4 initColor, glm::vec3 initLocation, float initScale)
{

	std::vector<glm::vec3> vertices = calculateSphereVertices(radius, stacks, sectors); // generates vertices
	mesh.vertexData.clear();															// esnures vector is clear
	for (int i = 0; i < vertices.size(); i++)											// fills vertexData
	{
		mesh.vertexData.push_back(vertices.at(i).x);
		mesh.vertexData.push_back(vertices.at(i).y);
		mesh.vertexData.push_back(vertices.at(i).z);
		mesh.vertexData.push_back(initColor.x);
		mesh.vertexData.push_back(initColor.y);
		mesh.vertexData.push_back(initColor.z);
		mesh.vertexData.push_back(initColor.a);
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
	

	// clears the index data vector
	mesh.indexData.clear();

	// fills indexData with correct index information
	calculateIndexData(mesh.indexData, stacks, sectors);


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
	uniform.uColor = initColor;
}

// ReferencePlane::ReferencePlane(float lineGap, )
