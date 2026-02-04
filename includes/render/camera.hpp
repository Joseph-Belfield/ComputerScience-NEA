#pragma once

#include "glm/glm.hpp"

// predefines
struct appData;

// Used to create a camera object for the viewer to interact throught the scene through.
class Camera
{
    private:
        glm::vec3 eyePosition;
        glm::vec3 viewDirection;
        glm::vec3 upVector;

        glm::vec2 previousMousePosition;
        glm::vec2 initialMousePosition;

        float deltaTime = 0.0f;         // delta time in milliseconds
        uint lastFrameTime = 0.0f;

    public:
        Camera();   // constructor

        glm::mat4 get_view_matrix() const;

        void update_deltaTime();

        void mouseLook(int mousePositionX, int mousePositionY, float sensitivity);

        void move_forward(float speed);
        void move_backward(float speed);
        void move_left(float speed);
        void move_right(float speed);
        void move_up(float speed);
        void move_down(float speed);
};
