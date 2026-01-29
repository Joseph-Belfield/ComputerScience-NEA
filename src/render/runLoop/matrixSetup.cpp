#include "runLoop.hpp"

#include "defines/appData.hpp"
#include "defines/render/objectData.hpp"
#include "defines/render/camera.hpp"

#include "glm/glm.hpp"                       // main GLM library
#include "glm/vec3.hpp"                      // GLM vec3 class and methods - for position data
#include "glm/mat4x4.hpp"                    // GLM 4x4 matrix class and methods - for transformations 
#include "glm/gtc/matrix_transform.hpp"      // translate function

#include <iostream>


// Creates a view matrix.
// - The scene is viewed as if through a camera for the viewer.
// - The view matrix rotates objects around the viewer to form the illusion of a a camera.
void update_viewMatrix(appData &appData)
{
    glm::mat4 viewMatrix = appData.camera.camera1.get_view_matrix();
    appData.program.defaultShader.set_mat4("uView", 1, false, viewMatrix);
}


// Creates a projection matrix.
// - The projection matrix creates the illusion of perspective
// - It does this by changing a point's coordinates according to distance from the camera (Z-value)
void update_perspectiveMatrix(appData &appData)
{
    // projection matrix (in perspective)
    glm::mat4 perspective = glm::perspective      // create perspective matrix
                            (
                                glm::radians(90.0f),                                                       // FOV (radians)
                                ((float)(appData.window.window_width) / appData.window.window_height),       // aspect ratio
                                0.1f,                                                                      // near clipping plane (min. distance)
                                100.0f                                                                     // far clipping plane (max. distance)
                            );         

    appData.program.defaultShader.set_mat4("uPerspective", 1, false, perspective);
}

