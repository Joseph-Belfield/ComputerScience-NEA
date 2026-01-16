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

    public:
        Camera();   // constructor

        glm::mat4 get_view_matrix() const;

        void mouseLook(int mousePositionX, int mousePositionY, appData &appData);

        void move_forward(float speed);
        void move_backward(float speed);
        void move_left(float speed);
        void move_right(float speed);
        void move_up(float speed);
        void move_down(float speed);
};
