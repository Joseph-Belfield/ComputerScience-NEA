#include "render.hpp"
#include "defines/contextData.hpp"

// *************************************************

#include "glad/gl.h"            // OpenGL extension loader

#include "SDL3/SDL.h"           // SDL main library
#include "SDL3/SDL_opengl.h"    // SDL OpenGL integration

// *************************************************

#include <vector>              // has standard input/output functions
#include <stdlib.h>
#include <iostream>

// *************************************************

namespace render
{


    void create_mesh(objectData &objectData)
    {

        // generate Vertex Array Objects 
        glGenVertexArrays(1, &(objectData.mesh.vertexArrayObject));           // creates an array to hold vertex data (called vertexArrayObject)
        glBindVertexArray(objectData.mesh.vertexArrayObject);                 // selects the array as current


        // generate Vertex Buffer Object for position
        glGenBuffers(1, &(objectData.mesh.vertexBufferObject));               // generates buffer
        glBindBuffer(GL_ARRAY_BUFFER, objectData.mesh.vertexBufferObject);    // sets buffer as current, specifies target
        glBufferData
        (
            GL_ARRAY_BUFFER,                           // specifies target
            objectData.mesh.vertexData.size() * sizeof(GLfloat),       // finds the size (in bytes) of vertex data
            objectData.mesh.vertexData.data(),                         // pointer to the array holding the data of the vector
            GL_STATIC_DRAW                             // sets intentions with data
        );                          


        // set up Element/Index Buffer Object (EBO / IBO) - holds the index for the order in which vertices are drawn
        glGenBuffers(1,&(objectData.mesh.indexBufferObject));                         // generate EBO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, objectData.mesh.indexBufferObject);   // sets buffer type as element buffer
        glBufferData
        (
            GL_ELEMENT_ARRAY_BUFFER,                        // target
            objectData.mesh.indexData.size() * sizeof(GLuint),        // size
            objectData.mesh.indexData.data(),                         // data
            GL_STATIC_DRAW                                  // usage
        );


        // setup position VAO
        glEnableVertexAttribArray(0); // enables the 0th attribute - AKA. this is the first VAO
        glVertexAttribPointer
        (
            0,                        // index into vector of VAOs
            3,                        // pieces of data (per vertex: x, y, z)
            GL_FLOAT,                 // data type
            GL_FALSE,                 // normalized?
            sizeof(GLfloat) * 6,     // stride (no. of bytes) to jump from first (type) data of v1 to first (type) data of v2, etc         
            (GLvoid*)0                // pointer for offset - irrelivent as position data is in first slot
        );


        // setup color VAO
        glEnableVertexAttribArray(1); // enables the 1st attribute - AKA. this is the second VAO
        glVertexAttribPointer
        (
            1,                                 // index into vector of VAOs
            3,                                 // pieces of data (per vertex: r, g, b)
            GL_FLOAT,                          // data type
            GL_FALSE,                          // normalized?
            sizeof(GLfloat) * 6,              // stride (byte offset) between firsts of same data       
            (GLvoid*)(sizeof(GLfloat) * 3)    // pointer for offset - starting position for first of that data type (address)
        );


        // disable any attributes previously opened in our vertex attribute array
        // glDisableVertexAttribArray(0);        // position
        // glDisableVertexAttribArray(1);        // color

        // cleanup VAO
        glBindVertexArray(0);                 // unbind currently bound VAO
        glBindBuffer(GL_ARRAY_BUFFER, 0);     // unbind currently bound VBO

    }
}

