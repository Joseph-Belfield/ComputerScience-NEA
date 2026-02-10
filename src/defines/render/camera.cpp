#include "render/camera.hpp"

#include "appData.hpp"

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


void Camera::update_deltaTime()
{
    // get time of current frame
    GLuint currentFrameTime = SDL_GetPerformanceCounter();
    
    // find difference
    deltaTime = (float)((currentFrameTime - lastFrameTime) * 1000 / (float)SDL_GetPerformanceFrequency()); 

    // set current frame as previous frame
    lastFrameTime = currentFrameTime;
}

// A function that allows the camera to follow mouse movement.
void Camera::mouseLook(int mousePositionX, int mousePositionY, float sensitivity)
{
    // store current mouse position
    glm::vec2 currentMousePosition = glm::vec2(mousePositionX, mousePositionY);

    // when the function is first called, sets an initial value of previous mouse position
    static bool firstLook = true;
    if (firstLook)
    {
        // sets the position of the mouse when scene first begins
        initialMousePosition = currentMousePosition;
        previousMousePosition = currentMousePosition;
        firstLook = false;
    }

    // finds the change in mouse position
    glm::vec2 positionDelta = previousMousePosition - currentMousePosition;
    glm::vec2 totalPositionDelta = initialMousePosition - currentMousePosition;
    
    // modifies how fast turning is
    positionDelta = positionDelta * sensitivity;


    // changes the view direction by rotating camera around the y-axis (upVector)
    viewDirection = glm::rotate(viewDirection, glm::radians(positionDelta.x), upVector);

    // calculate "x-axis" vector (average vector of x and z in 3D)
    glm::vec3 localAxisZ = glm::vec3(viewDirection.x, 0.0f, viewDirection.z);
    glm::vec3 localAxisX = glm::rotate(localAxisZ, glm::radians(90.0f), upVector);

    // changes view direction by rotating camera around x-axis
    viewDirection = glm::rotate(viewDirection, glm::radians(positionDelta.y), -localAxisX); 

    // TODO make it so that mouse delta y does not change when the user exceeds 90

    // sets current mouse position as old mouse position for next frame
    previousMousePosition = currentMousePosition;
}
 
// forward relative to the X-Z plane -> will not change Y position
void Camera::move_forward(float speed)
{
    eyePosition += (glm::vec3(viewDirection.x, 0, viewDirection.z) * speed * (deltaTime / 100));
}
void Camera::move_backward(float speed)
{
    eyePosition -= (glm::vec3(viewDirection.x, 0, viewDirection.z) * speed * (deltaTime / 100));
}

// cannot change Y as there is no Z rotation on the camera
void Camera::move_left(float speed)
{
    // get the view matrix by getting the normal vector (right-hand rule) and subtracting it to eyePos.
    glm::vec3 rightVector = glm::cross(viewDirection, upVector);
    eyePosition -= (rightVector * speed * (deltaTime / 100));
}
void Camera::move_right(float speed)
{
    // get the view matrix by getting the normal vector (right-hand rule) and adding it to eyePos.
    glm::vec3 rightVector = glm::cross(viewDirection, upVector);
    eyePosition += (rightVector * speed * (deltaTime / 100));
}

// forward relative to the X-Z plane -> will not change Y position
void Camera::move_up(float speed)
{
    eyePosition += (upVector * speed * (deltaTime / 100));
}
void Camera::move_down(float speed)
{
    eyePosition -= (upVector * speed * (deltaTime / 100));
}

