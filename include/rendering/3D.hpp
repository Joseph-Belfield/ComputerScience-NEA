#ifndef GLAD
    #include <glad/glad.h>
    #define GLAD
#endif

#ifndef GLFW
    #include <GLFW/glfw3.h>
    #define GLFW
#endif

#ifndef IOSTREAM
    #include <iostream>
    #define IOSTREAM
#endif


    
// callback function which resizes windows dimensions.
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main()
{

 // initialize GLFW window:

    // initialize GLFW.
    glfwInit();

    // glfwWindowHint() takes in two parameters: the option to configure, and what we want it set to (effectively choosing settings).

    // sets the GLFW version to 3.3 (major.minor).
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // sets the OpenGL profile to the core profile.
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif
    
 // create window object:

    // creates window object called window, dimensions 800x600, window titled "ChemS_Paint". No monitor is set, as it is only needed for a full screen window.
    GLFWwindow* window = glfwCreateWindow(800, 600, "ChemS_Paint", NULL, NULL);

    // a check to see if creating a window worked - if no window object is present, the code prints error message in terminal, then terminates and ends the program.
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << "\n";
        glfwTerminate();
        return -1;
    }

    // sets the current context to the window that was just made.
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

 // initialize GLAD:

    // to check if GLAD is working, the function which gets the GLFW function pointers is passed (OS specific). If it fails, program sends error to terminal and ends.
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << "\n";
        return -1;
    }

 // render loop:

    // main render loop.
    while (!glfwWindowShouldClose(window))
    {
        // the buffer effectively is just all the data about what colors should be where on the screen.
        glfwSwapBuffers(window);
        // events are user input, etc.
        glfwPollEvents();  
    }

 // end main function:

    // cleans up GLFW resources.
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}