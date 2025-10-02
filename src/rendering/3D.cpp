// change name to .hpp once done testing

#include <iostream> //iostream only for testing purposes
#include <cmath>
using namespace std;

#include <glad/glad.h>  // GLAD must be included first to work
#include <GLFW/glfw3.h>


// handling window resizing using callback function
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main()
{
    // initializes GLFW window
    glfwInit(); 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);  // glfwWidnowHint takes 2 parameters - what option should change, and what it should be configured to
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);  // sets the version of GLFW as 3.3 (MAJOR.MINOR) 
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // create window object
    GLFWwindow* window = glfwCreateWindow(800, 600, "Molecule Modeller", NULL, NULL);
    if (window == NULL)
    {
        cout << "Failed to create GLFW window" << "\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // initialize GLAD to handle function pointers before OpenGL work
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize GLAD" << "\n";
        return -1;
    }

    // sets intiial size and position of the window
    glViewport(0, 0, 800, 600);

    // tells OpenGL to call this function whenever window resized
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // render loop
    while(!glfwWindowShouldClose(window))
    {
        glfwSwapBuffers(window);    // will swap the color buffer used to render color of each pixel
        glfwPollEvents();   // checks if events are triggered (eg: inputs), updates window, calls corresponding functions
    }

    // deletes OpenGL data and ends program once while render loop ends
    glfwTerminate();
    return 0;
}