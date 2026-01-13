#include "render.hpp"
#include "defines/everythingClass.hpp"

// *************************************************

#include "glad/gl.h"            // OpenGL extension loader

#include "SDL3/SDL.h"           // SDL main library
#include "SDL3/SDL_opengl.h"    // SDL OpenGL integration

// *************************************************

#include <vector>              // has standard input/output functions
#include <stdlib.h>

// *************************************************

namespace render
{
    void vertex_specification(Context* globalContext)
    {
    
        // use of GLfloat as it is more cross-platform (likely won't matter but best practice)
        const std::vector<GLfloat> vertexData   // lives on CPU
        {
            -0.5f, -0.5f,  0.5f,    // vertex 0 - (front bottom left)
            1.0f,  0.0f,  0.0f,    // color

            0.5f, -0.5f,  0.5f,    // vertex 1 - (front bottom right)
            0.0f,  0.0f,  1.0f,    // color

            -0.5f,  0.5f,  0.5f,    // vertex 2 - (front top left)
            1.0f,  0.0f,  0.0f,    // color   

            0.5f,  0.5f,  0.5f,    // vertex 3 - (front top right)
            0.0f,  0.0f,  1.0f,    // color

            -0.5f, -0.5f, -0.5f,    // vertex 4 - (back bottom left)
            1.0f,  0.0f,  0.0f,    // color

            0.5f, -0.5f, -0.5f,    // vertex 5 - (back bottom right)
            0.0f,  0.0f,  1.0f,    // color

            -0.5f,  0.5f, -0.5f,    // vertex 6 - (back top left)
            1.0f,  0.0f,  0.0f,    // color   

            0.5f,  0.5f, -0.5f,    // vertex 7 - (back top right)
            0.0f,  0.0f,  1.0f,    // color
        };


        // generate Vertex Array Objects 
        glGenVertexArrays(1, &(globalContext -> vertexArrayObject));           // creates an array to hold vertex data (called vertexArrayObject)
        glBindVertexArray(globalContext -> vertexArrayObject);                 // selects the array as current

        // generate Vertex Buffer Object for position
        glGenBuffers(1, &(globalContext -> vertexBufferObject));               // generates buffer
        glBindBuffer(GL_ARRAY_BUFFER, globalContext -> vertexBufferObject);    // sets buffer as current, specifies target
        glBufferData
        (
            GL_ARRAY_BUFFER,                           // specifies target
            vertexData.size() * sizeof(GLfloat),       // finds the size (in bytes) of vertex data
            vertexData.data(),                         // pointer to the array holding the data of the vector
            GL_STATIC_DRAW                             // sets intentions with data
        );                          

       

        // data about the order vertices should be renderd in
        const std::vector<GLuint> indexBufferData
        {
            // face 1 - front
            2, 0, 1,      // triangle 1
            3, 2, 1,      // triangle 2

            // face 2 - back
            7, 5, 4,
            4, 6, 7,

            //face 3 - left
            6, 4, 0,
            0, 2, 6,

            //face 4 - right
            3, 1, 5,
            5, 7, 3,

            // face 5 - top
            7, 6, 2,
            2, 3, 7,

            //face 6 - bottom
            4, 0, 1,
            1, 5, 4

        };

        // set up Element/Index Buffer Object (EBO / IBO) - holds the index for the order in which vertices are drawn
        glGenBuffers(1,&(globalContext -> indexBufferObject));                         // generate EBO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, globalContext -> indexBufferObject);   // sets buffer type as element buffer

       

        glBufferData
        (
            GL_ELEMENT_ARRAY_BUFFER,                        // target
            indexBufferData.size() * sizeof(GLuint),        // size
            indexBufferData.data(),                         // data
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
            sizeof(GL_FLOAT) * 6,     // stride (no. of bytes) to jump from first (type) data of v1 to first (type) data of v2, etc         
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
            sizeof(GL_FLOAT) * 6,              // stride (byte offset) between firsts of same data       
            (GLvoid*)(sizeof(GL_FLOAT) * 3)    // pointer for offset - starting position for first of that data type (address)
        );

       

        // cleanup VAO
        glBindVertexArray(0);                 // unbind currently bound VAO

        // disable any attributes previously opened in our vertex attribute array
        glDisableVertexAttribArray(0);        // position
        glDisableVertexAttribArray(1);        // color
        
    }
}

