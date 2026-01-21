#include "runLoop.hpp"

#include "defines/contextData.hpp"


#include "glad/gl.h"


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
        GL_TRIANGLES,         // shape
        36,                    // number of vertices drawn to (count repeats)
        GL_UNSIGNED_INT,      // data type
        0                     // offset into index array for first element (triangle vertex order)
    );

    // unbind VAO after shape drawn
    glBindVertexArray(0);
}