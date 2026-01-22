#include "runLoop.hpp"

#include "render.hpp"
#include "defines/appData.hpp"
#include "defines/objectData.hpp"

#include "glad/gl.h"

#include <iostream>

// Handles tasks that must be completed before draw:
// - OpenGL preferences
// - Sets glViewport
// - Sets clear color (background color)
// - Applies transformation matrices
void update_mesh(objectData &objectData)
{
    // transformation matrices
    model_matrix(objectData);         // controls position, rotation and scale on world axis
}

// Draws each induvidual mesh
void draw_mesh(objectData &objectData)
{
    // choose VAO and VBO
    glBindVertexArray(objectData.mesh.vertexArrayObject);

    // draw
    glDrawElements
    (
        GL_TRIANGLES,                       // shape
        objectData.mesh.indexData.size(),   // number of vertices drawn to (count repeats)
        GL_UNSIGNED_INT,                    // data type
        (void*)0                            // offset into index array for first element (triangle vertex order)
    );  

    // unbind VAO after shape drawn
    glBindVertexArray(0);
}