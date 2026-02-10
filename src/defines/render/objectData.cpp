#include "render/objectData.hpp"

#include "render/runLoop.hpp"
#include "render/shaderClass.hpp"
#include "render/camera.hpp"

#include "glad/gl.h"

#include "glm/glm.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/gtc/matrix_transform.hpp"    

#include <vector>
#include <math.h>
#include <iostream>

Object::Object() {};

void Object::init_drawInfo(Camera* camera, float width, float height)
{
	drawInfo.camera = camera;
	drawInfo.width = width;
	drawInfo.height = height;
	
	drawInfo.init = true;
}

void draw_polygon(Object* object)
{
    // draw
    glDrawElements
    (
        GL_TRIANGLES,                       // shape
        object -> mesh.indexData.size(),    // number of vertices drawn to (count repeats)
        GL_UNSIGNED_INT,                    // data type
        (void*)0                            // offset into index array for first element (triangle vertex order)
    );  
}

void draw_lines(Object* object)
{
    // draw
    glDrawElements
    (
        GL_LINES,                     	    // shape
        object -> mesh.indexData.size(),      // number of vertices drawn to (count repeats)
        GL_UNSIGNED_INT,                    // data type
        (void*)0                            // offset into index array for first element (triangle vertex order)
    );  
}

// Set the uniforms needed per vertex shader version.
void setUniforms_vertex(Object* object, uint version)
{
	switch(version)
	{
		case(0):
		{
			// vertex shader uniforms
			object -> mesh.objectShader.set_model(object -> uniform.vert.location, object -> uniform.vert.rotation, object -> uniform.vert.scale);
			object -> mesh.objectShader.set_perspective(object -> drawInfo.width, object -> drawInfo.height);
			object -> mesh.objectShader.set_view(object -> drawInfo.camera);
			break;
		}
		case(1):
		{
			// recursively call case 0!
			setUniforms_vertex(object, 0);
			break;
		}
		case(2):
		{
			setUniforms_vertex(object, 0);
			break;
		}
	}
}

// Sets the uniforms needed per fragment shader version.
void setUniforms_fragment(Object* object, uint version)
{
	switch(version)
	{
		case(0):
		{
			// no uniforms needed!
			break;
		}
		case(1):
		{
			object -> mesh.objectShader.set_float4("uColor", object -> uniform.frag.uColor);
			break;
		}
		case(2):
		{
			object -> mesh.objectShader.set_float3("uObjectColor", object -> uniform.frag.objectColor);
			object -> mesh.objectShader.set_float3("uLightColor", object -> uniform.frag.lightColor);

			object -> mesh.objectShader.set_float1("uAmbience", object -> uniform.frag.ambienceStrength);
			object -> mesh.objectShader.set_float3("uLightPos", object -> uniform.frag.lightPos);
			object -> mesh.objectShader.set_float1("uSpecularStrength", object -> uniform.frag.specularStrength);
			break;
		}
	}
}

void setup_draw(Object* object)
{
	// using this object's shader (inefficient in large projects, but doesn't matter at this scale)
	object -> mesh.objectShader.use();

	// sets the uniforms for the shader program (must be set each time new program is called!)
	setUniforms_vertex(object, object -> mesh.objectShader.versionVertex);
	setUniforms_fragment(object, object -> mesh.objectShader.versionFragment);

	// choose VAO and VBO
    glBindVertexArray(object -> mesh.vertexArrayObject);
}

void cleanup_draw()
{
	// unbind VAO after shape drawn
    glBindVertexArray(0);
}

void Object::draw()
{
	// ensure shader is initialized
	if (!mesh.objectShader.compiled)
	{
		mesh.objectShader.compile_and_link();
	}
	// ensure info needed for drawing has been passed to the object
	if (!drawInfo.init)
	{
		std::cout << "Draw info not initialized!" << std::endl;
		exit(-1);
	}

	// set up uniforms, VAO, etc 
	setup_draw(this);

	if (subclass == REFERENCE_PLANE)
	{
		draw_lines(this);
	}
	else 
	{
		draw_polygon(this);
	}

	// clean up VAO
	cleanup_draw();
}

void Cylinder::draw_between(glm::vec3 position1, glm::vec3 position2)
{
	// move location of cylinder to centre of space between points
    glm::vec3 direction = position2 - position1;        // AB = B - A
    glm::vec3 location = position1 + (0.5f * direction);        // R = AB + (l * (AB))
    uniform.vert.location = location;

    // rotate cylinder accordingly
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
    uniform.vert.rotation = totalRotation;

    // set the cylinder's scale correctly
    float scale = glm::length(position2 - position1);
    uniform.vert.scale.y = scale;

    // draw
    draw();

    // reset object position
    uniform.vert.location = glm::vec3(0.0f);
    uniform.vert.rotation = glm::vec3(0.0f);
    uniform.vert.scale.y = 1.0f;
}


void init_shaders(Shader& shaderObject,std::string source_vertexShader, std::string source_fragmentShader)
{
	// attatches shaders (defaults to preset default values)
	shaderObject.set_vertexSource(source_vertexShader);
	shaderObject.set_fragmentSource(source_fragmentShader);

	// compiles attatched shaders
	shaderObject.compile_and_link();
}

void vertex_specification(meshData &mesh, bool color = false, bool normals = false, bool textures = false)
{
	// stride between data types
	uint stride = 0;

	uint position_VAO = 0;

	// all offsets begin after vertex data
	uint colorOffset = 3;	
	uint normalOffset = 3;
	uint textureOffset = 3;

	// adds stride for each of the possible catagories
	if (color)
	{
		stride += 4;
		normalOffset += 4;
		textureOffset += 4;
	}
	if (normals)
	{
		stride += 3;
		textureOffset += 3;
	}
	if (textures)
	{
		stride += 2;
	}

	if (color || normals || textures)
	{
		// for the stride added by vertices
		stride += 3;
	}



	// generate Vertex Array Objects 
	glGenVertexArrays(1, &(mesh.vertexArrayObject));           // creates an array to hold vertex data (called vertexArrayObject)
	glBindVertexArray(mesh.vertexArrayObject);                 // selects the array as current


	// generate Vertex Buffer Object for position
	glGenBuffers(1, &(mesh.vertexBufferObject));               // generates buffer
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vertexBufferObject);    // sets buffer as current, specifies object
	glBufferData
	(
		GL_ARRAY_BUFFER,                          							// specifies object
		mesh.vertexData.size() * sizeof(GLfloat),      	 				   	// finds the size (in bytes) of vertex data
		mesh.vertexData.data(),                       		  			  	// pointer to the array holding the data of the vector
		GL_STATIC_DRAW                             							// sets intentions with data
	);

	// set up Element/Index Buffer Object (EBO / IBO) - holds the index for the order in which vertices are drawn
	glGenBuffers(1,&(mesh.indexBufferObject));                         // generate EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.indexBufferObject);   // sets buffer type as element buffer
	glBufferData
	(
		GL_ELEMENT_ARRAY_BUFFER,                        		// object
		mesh.indexData.size() * sizeof(GLuint),    		   		// size
		mesh.indexData.data(),                        			// data
		GL_STATIC_DRAW                                			// usage
	);

	
	// setup position VAO
	glEnableVertexAttribArray(position_VAO); // enables the 0th attribute - AKA. this is the first VAO
	glVertexAttribPointer
	(
		position_VAO++,           // index into vector of VAOs
		3,                        // pieces of data (per vertex: x, y, z)
		GL_FLOAT,                 // data type
		GL_FALSE,                 // normalized?
		stride * sizeof(GLfloat), // stride (no. of bytes) to jump from first (type) data of v1 to first (type) data of v2, etc         
		(GLvoid*)0                // pointer for offset - irrelivent as position data is in first slot
	);

	// if object color is also held in the VAO (as opposed to uniforms)
	if (color)
	{
		// setup position VAO
		glEnableVertexAttribArray(position_VAO); // enables the 0th attribute - AKA. this is the first VAO
		glVertexAttribPointer
		(
			position_VAO++,                        		// index into vector of VAOs
			4,                        					// pieces of data (per vertex: r, g, b, a)
			GL_FLOAT,                 
			GL_FALSE,                 
			stride * sizeof(GLfloat),      		  	  					
			(GLvoid*)(sizeof(GLfloat) * colorOffset)               
		);
	}
	if (normals)
	{
		std::cout << normalOffset << std::endl;

		// setup position VAO
		glEnableVertexAttribArray(position_VAO); // enables the 0th attribute - AKA. this is the first VAO
		glVertexAttribPointer
		(
			position_VAO++,                        		// index into vector of VAOs
			3,                        					// pieces of data (per vertex: nx, ny, nz)
			GL_FLOAT,                 
			GL_FALSE,                 
			stride * sizeof(GLfloat),      		  	  					
			(GLvoid*)(sizeof(GLfloat) * normalOffset)               
		);
	}
	if (textures)
	{
		// setup position VAO
		glEnableVertexAttribArray(position_VAO); // enables the 0th attribute - AKA. this is the first VAO
		glVertexAttribPointer
		(
			position_VAO,                        		// index into vector of VAOs
			2,                        					// pieces of data (per vertex: s, t)
			GL_FLOAT,                 
			GL_FALSE,                 
			stride * sizeof(GLfloat),      		  	  					
			(GLvoid*)(sizeof(GLfloat) * textureOffset)               
		);
	}


	// cleanup
	glBindVertexArray(0);                		// unbind currently bound VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);     		// unbind currently bound VBO
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

std::vector<glm::vec3> calculateSphereNormals(const float radius, const GLuint stackCount, const GLuint sectorCount)
{
	std::vector<glm::vec3> normals;		// vector to hold normals
	float lengthInv = 1.0f / radius;	// vertex normal

	float nx;
	float ny;
	float nz;

	std::vector<glm::vec3> vertices = calculateSphereVertices(radius, stackCount, sectorCount);
		
	for (int i = 0; i < vertices.size(); i++)
	{
		nx = vertices[i].x * lengthInv;
		ny = vertices[i].y * lengthInv;
		nz = vertices[i].z * lengthInv;

		normals.push_back(glm::vec3(nx, ny, nz));
	}

	return normals;
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
Sphere::Sphere(std::string source_vertexShader, std::string source_fragmentShader, const GLfloat radius, const GLuint stacks, const GLuint sectors, const glm::vec3 initColor, glm::vec3 initLocation, GLfloat initScale)
{
	subclass = SPHERE;
	init_shaders(mesh.objectShader, source_vertexShader, source_fragmentShader);

	// clears the index
	mesh.vertexData.clear();

	// ensures vector is clear
	mesh.indexData.clear();

	std::vector<glm::vec3> vertices = calculateSphereVertices(radius, stacks, sectors); // generates vertices	
	std::vector<glm::vec3> normals = calculateSphereNormals(radius, stacks, sectors);  	// generates normals							
	for (int i = 0; i < vertices.size(); i++)											// fills vertexData
	{
		mesh.vertexData.push_back(vertices[i].x);
		mesh.vertexData.push_back(vertices[i].y);
		mesh.vertexData.push_back(vertices[i].z);

		mesh.vertexData.push_back(normals[i].x);
		mesh.vertexData.push_back(normals[i].y);
		mesh.vertexData.push_back(normals[i].z);
	}

	// fills indexData with correct index information
	calculateSphereIndexData(mesh.indexData, stacks, sectors);

	// sets up VAO, VBO and IBO
	vertex_specification(mesh, false, true);

	// set up uniforms
	uniform.vert.location = initLocation;
	uniform.vert.scale = glm::vec3(initScale);

	uniform.frag.uColor = glm::vec4(initColor, 1.0f);
	uniform.frag.objectColor = initColor;
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
ReferencePlane::ReferencePlane(GLfloat initHeight, const glm::vec3 initColor, GLfloat initScale, const GLuint stripCount, std::string source_vertexShader, std::string source_fragmentShader)
{
	subclass = REFERENCE_PLANE;
	init_shaders(mesh.objectShader, source_vertexShader, source_fragmentShader);

	// ensures data vectors are clear
	mesh.vertexData.clear();
	mesh.indexData.clear();


	std::vector<glm::vec3> vertices = calculateReferencePlaneVertices(stripCount);

	mesh.vertexData.reserve(stripCount * stripCount);
	for (int i = 0; i < vertices.size(); i++)
	{
		mesh.vertexData.push_back(vertices[i].x);
		mesh.vertexData.push_back(vertices[i].y);
		mesh.vertexData.push_back(vertices[i].z);
	}

	calculateReferencePlaneIndexData(mesh.indexData, stripCount);

	vertex_specification(mesh);

	// set up uniforms
	uniform.vert.location = glm::vec3(0.0f, initHeight, 0.0f);
	uniform.vert.scale = glm::vec3(5.0f);
	uniform.frag.uColor = glm::vec4(initColor, 1.0f);
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
Cylinder::Cylinder(std::string source_vertexShader, std::string source_fragmentShader, const GLfloat radius, const GLfloat height, const GLuint sectorCount, const glm::vec3 initColor, glm::vec3 initLocation, const glm::vec3 initRotation, const glm::vec3 initScale)
{
	subclass = CYLINDER;
	init_shaders(mesh.objectShader, source_vertexShader, source_fragmentShader);

	mesh.vertexData.clear();
	mesh.indexData.clear();


	std::vector<glm::vec3> vertices = calculateCylinderVertices(radius, height, sectorCount);
	for (int i = 0; i < vertices.size(); i++)
	{
		mesh.vertexData.push_back(vertices[i].x);
		mesh.vertexData.push_back(vertices[i].y);
		mesh.vertexData.push_back(vertices[i].z);
	}


	// calculate index data for cylinder
	calculateCylinderIndexData(mesh.indexData, sectorCount);

	// sets up VAO, VBO and IBO
	vertex_specification(mesh);

	// set up uniforms
	uniform.vert.location = initLocation;
	uniform.vert.rotation = initRotation;
	uniform.vert.scale = initScale;
	
	uniform.frag.uColor = glm::vec4(initColor, 1.0f);
	uniform.frag.objectColor = initColor;
}


std::vector<glm::vec3> calculateCubeVertices(GLfloat height)
{
	std::vector<glm::vec3> vertices;
	GLfloat h = height / 2;

	vertices.push_back(glm::vec3(h, h, h));		// front top left
	vertices.push_back(glm::vec3(-h, h, h));	// front top right
	vertices.push_back(glm::vec3(h, -h, h));	// front bottom left
	vertices.push_back(glm::vec3(-h, -h, h));	// front bottom right

	vertices.push_back(glm::vec3(h, h, -h));	// back top left
	vertices.push_back(glm::vec3(-h, h, -h));	// back top right
	vertices.push_back(glm::vec3(h, -h, -h));	// back bottom left
	vertices.push_back(glm::vec3(-h, -h, -h));	// back bottom right

	return vertices;
}

void calculateCubeIndexData(std::vector<GLuint> &indexData)
{
	// FRONT
	indexData.push_back(1);
	indexData.push_back(0);
	indexData.push_back(2);

	indexData.push_back(2);
	indexData.push_back(3);
	indexData.push_back(1);

	// BACK
	indexData.push_back(4);
	indexData.push_back(5);
	indexData.push_back(7);

	indexData.push_back(7);
	indexData.push_back(6);
	indexData.push_back(4);

	// TOP
	indexData.push_back(5);
	indexData.push_back(4);
	indexData.push_back(0);

	indexData.push_back(0);
	indexData.push_back(1);
	indexData.push_back(5);

	// BASE
	indexData.push_back(2);
	indexData.push_back(3);
	indexData.push_back(7);

	indexData.push_back(7);
	indexData.push_back(6);
	indexData.push_back(2);

	// LEFT
	indexData.push_back(0);
	indexData.push_back(4);
	indexData.push_back(6);

	indexData.push_back(6);
	indexData.push_back(2);
	indexData.push_back(0);

	// RIGHT
	indexData.push_back(3);
	indexData.push_back(1);
	indexData.push_back(5);

	indexData.push_back(5);
	indexData.push_back(7);
	indexData.push_back(3);
}

Cube::Cube(std::string source_vertexShader, std::string source_fragmentShader, const GLfloat height, const glm::vec3 initColor, glm::vec3 initLocation, glm::vec3 initRotation, glm::vec3 initScale)
{
	subclass = CUBE;
	init_shaders(mesh.objectShader, source_vertexShader, source_fragmentShader);

	mesh.vertexData.clear();
	mesh.indexData.clear();

	std::vector<glm::vec3> vertices = calculateCubeVertices(height);
	for (int i = 0; i < 8; i++)
	{
		mesh.vertexData.push_back(vertices[i].x);
		mesh.vertexData.push_back(vertices[i].y);
		mesh.vertexData.push_back(vertices[i].z);
	}

	calculateCubeIndexData(mesh.indexData);

	// VAO, VBO, IBO
	vertex_specification(mesh);

	// set up uniforms
	uniform.vert.location = initLocation;
	uniform.vert.rotation = initRotation;
	uniform.vert.scale = initScale;
	uniform.frag.uColor = glm::vec4(initColor, 1.0f);
}