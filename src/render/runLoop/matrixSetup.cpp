#include "runLoop.hpp"

#include "defines/contextData.hpp"
#include "defines/camera.hpp"

#include "glm/glm.hpp"                       // main GLM library
#include "glm/vec3.hpp"                      // GLM vec3 class and methods - for position data
#include "glm/mat4x4.hpp"                    // GLM 4x4 matrix class and methods - for transformations 
#include "glm/gtc/matrix_transform.hpp"      // translate function

#include <iostream>

// Creates a uniform matrix, and returns its GLuint ID.
GLuint create_uniform_mat4(GLuint shaderProgram, std::string uniformName, int amount, bool enableTranspose, glm::mat4 matrix)
{
    GLuint uniformLocation = glGetUniformLocation(shaderProgram, uniformName.c_str());
    if (uniformLocation >= 0)
    {
        glUniformMatrix4fv
        (
            uniformLocation,
            amount,
            false,
            &matrix[0][0]
        );

        return uniformLocation;
    }
    else
    {
        std::cout << "Could not find uniform - check spelling!" << std::endl;
        exit(-1);
    }
}


// Creates a model matrix.
// - Objects begin in local space, where they are created on their own set of axis
// - The model matrix moves objects from local space to world space, where objects are all held relative to one shared set of axis
//
// The model matrix is also edited accordingly to change an objects position/rotation in world space accordingly.
void model_matrix(objectData &objectData)
{
    // create and adapt the matrix to adjust the following transformations
    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(objectData.uniform.uDisplacement[0], objectData.uniform.uDisplacement[1], objectData.uniform.uOffset)); // movement
    modelMatrix = glm::rotate(modelMatrix ,glm::radians(objectData.uniform.uRotate), glm::vec3(0.0f, 1.0f, 0.0f));  // rotations
    modelMatrix = glm::scale(modelMatrix, glm::vec3(objectData.uniform.uScale, objectData.uniform.uScale, objectData.uniform.uScale));

    GLuint location_modelMatrix = create_uniform_mat4(objectData.mesh.shaderProgram, "uModelMatrix", 1, false, modelMatrix);
}


// Creates a view matrix.
// - The scene is viewed as if through a camera for the viewer.
// - The view matrix rotates objects around the viewer to form the illusion of a a camera.
void view_matrix(appData &appData)
{
    glm::mat4 viewMatrix = appData.camera.camera1.get_view_matrix();
    GLuint location_viewMatrix = create_uniform_mat4(appData.program.shaderProgram, "uViewMatrix", 1, false, viewMatrix);
}

// Creates a projection matrix.
// - The projection matrix creates the illusion of perspective
// - It does this by changing a point's coordinates according to distance from the camera (Z-value)
void perspective_matrix(appData &appData)
{
    // projection matrix (in perspective)
    glm::mat4 perspective = glm::perspective      // create perspective matrix
                            (
                                glm::radians(45.0f),                                                       // FOV (radians)
                                (float)(appData.window.window_width / appData.window.window_height),       // aspect ratio
                                0.1f,                                                                      // near clipping plane (min. distance)
                                50.0f                                                                      // far clipping plane (max. distance)
                            );         

    GLuint location_perspective = create_uniform_mat4(appData.program.shaderProgram, "uPerspective", 1, false, perspective);
}