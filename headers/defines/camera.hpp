#ifndef GLM
  #include "glm/glm.hpp"                       // main GLM library
  #include "glm/vec3.hpp"                      // GLM vec3 class and methods - for position data
  #include "glm/mat4x4.hpp"                    // GLM 4x4 matrix class and methods - for transformations 
  #include "glm/gtc/matrix_transform.hpp"      // translate function
  #define GLM
#endif

// Used to create a camera object for the viewer to interact throught the scene through.
class Camera
{
private:
    glm::vec3 eyePosition = glm::vec3(0.0f, 0.0f, 0.0f);    // Position of the camera. Defaults to the origin.
    glm::vec3 viewDirection = glm::vec3(0.0f, 0.0f, -1.0f); // Direction the camera is pointing. Defaults to pointing out into the world, -Z (+Z is behind the camera).
    glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);       // Direction of up, relative to the camera. Defaults to +Y.
  
public:

    // Constructor
    Camera() {};

    // Get's the view matrix to convert world space into camera/view space. Constructed using all 3 members.
    glm::mat4 get_view_matrix() 
    const                               // Will not be modified, so const correct (best practice),
    {
        return glm::lookAt(eyePosition, viewDirection, upVector);
    }


    void move_forward()
    {

    }

    void move_backward()
    {
        
    }

    void move_left()
    {
        
    }

    void move_right()
    {
        
    }
};