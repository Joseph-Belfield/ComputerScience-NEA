#include "defines/camera.hpp"

#include "glm/glm.hpp"                       // main GLM library
#include "glm/vec3.hpp"                      // GLM vec3 class and methods - for position data
#include "glm/mat4x4.hpp"                    // GLM 4x4 matrix class and methods - for transformations 
#include "glm/gtc/matrix_transform.hpp"      // translate function


Camera::Camera()
{
    eyePosition = glm::vec3(0.0f, 0.0f, 0.0f);    // Position of the camera. Defaults to the origin.
    viewDirection = glm::vec3(0.0f, 0.0f, -1.0f); // Direction the camera is pointing. Defaults to pointing out into the world, -Z (+Z is behind the camera).
    upVector = glm::vec3(0.0f, 1.0f, 0.0f);       // Direction of up, relative to the camera. Defaults to +Y.
}

glm::mat4 Camera::get_view_matrix() const
{
    return glm::lookAt(eyePosition, viewDirection, upVector);
}

void Camera::move_forward(float speed)
{
    eyePosition -= 0.1f;
}
void Camera::move_backward(float speed)
{
    eyePosition += 0.1f;
}

