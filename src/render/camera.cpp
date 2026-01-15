#include "defines/camera.hpp"
#include "defines/contextData.hpp"

#include "glm/glm.hpp"                       // main GLM library

// experimental features that may change later
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtc/matrix_transform.hpp"      // translate function
#include "glm/gtx/rotate_vector.hpp"         // rotate function for camera movement

#include <iostream>


Camera::Camera()
{
    eyePosition = glm::vec3(0.0f, 0.0f, 0.0f);    // Position of the camera. Defaults to the origin.
    viewDirection = glm::vec3(0.0f, 0.0f, -1.0f); // Direction the camera is pointing. Defaults to pointing out into the world, -Z (+Z is behind the camera).
    upVector = glm::vec3(0.0f, 1.0f, 0.0f);       // Direction of up, relative to the camera. Defaults to +Y.
}


glm::mat4 Camera::get_view_matrix() const
{
    return glm::lookAt
    (
        eyePosition,                    // position of camera
        eyePosition + viewDirection,    // position the camera is looking at
        upVector                        // relative up
    );
}

// A function that allows the camera to follow mouse movement.
void Camera::mouseLook(int mousePositionX, int mousePositionY, appData &appData)
{
    // store current mouse position
    glm::vec2 currentMousePosition = glm::vec2(mousePositionX, mousePositionY);

    // when the function is first called, sets an initial value of previous mouse position
    static bool firstLook = true;
    if (firstLook)
    {
        previousMousePosition = currentMousePosition;
        firstLook = false;
    }

    // finds the change in mouse position
    glm::vec2 positionDelta = previousMousePosition - currentMousePosition;
    
    // modifies how fast turning is
    positionDelta = positionDelta * appData.camera.sense;

    // changes the view direction by rotating camera around the y-axis (upVector)
    viewDirection = glm::rotate(viewDirection, glm::radians(positionDelta.x), upVector);

    

    // sets current mouse position as old mouse position for next frame
    previousMousePosition = currentMousePosition;
}
 
void Camera::move_forward(float speed)
{
    eyePosition += (viewDirection * speed);
}
void Camera::move_backward(float speed)
{
    eyePosition -= (viewDirection * speed);
}

void Camera::move_left(float speed)
{
    // get the view matrix by getting the normal vector (right-hand rule) and subtracting it to eyePos.
    glm::vec3 rightVector = glm::cross(viewDirection, upVector);
    eyePosition -= (rightVector * speed);
}
void Camera::move_right(float speed)
{
    // get the view matrix by getting the normal vector (right-hand rule) and adding it to eyePos.
    glm::vec3 rightVector = glm::cross(viewDirection, upVector);
    eyePosition += (rightVector * speed);
}

