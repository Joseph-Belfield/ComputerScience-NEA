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

void processInput(GLFWwindow* window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}


// vertex shader GLSL code as a C string.
const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
" gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

// fragment shader GLSL code as C string.
const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
" FragColor = vec4(0.0f, 0.0f, 1.0f, 1.0f);\n"
"}\0";

int main()
{

// ===== CREATING THE WINDOW =====

 // 1. initialize GLFW window:

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
    
 // 2. create window object:

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

 // 3. initialize GLAD:

    // to check if GLAD is working, the function which gets the GLFW function pointers is passed (OS specific). If it fails, program sends error to terminal and ends.
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << "\n";
        return -1;
    }


// ===== SHADERS =====


 // 4. vertex shader compilation:

    // creating vertex shader object to link to vertex shader.
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // compile the shader.
    // glShaderSource takes in the object, no. of strings of source code and then the source code.
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // test to see if the vertex shader was compiled correctly.
    int vShader_success;
    char vShader_infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vShader_success);

    // throws error message and error details if shader not compiled correctly
    if (!vShader_success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, vShader_infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << vShader_infoLog << std::endl;
    }

 // 5. fragment shader compilation:

    // similar to vertex shader compilation.
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // checking for fragment shader compilation errors.
    int fShader_success;
    char fShader_infoLog[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fShader_success);

    if (!fShader_success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, fShader_infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << fShader_infoLog << std::endl;
    }


 // 6. shader object (holds mutliple shaders needed for project):

    unsigned int shaderProgram;
    // creates a new program, then returns its ID.
    shaderProgram = glCreateProgram();

    // attatch created shaders to program.
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    // link attatched shaders to program.
    glLinkProgram(shaderProgram);

    // check for linking errors.
    int shaderProgram_success;
    char shaderProgram_infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &shaderProgram_success);

    if (!shaderProgram_success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, shaderProgram_infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << shaderProgram_infoLog << std::endl;
    }

    // applies the shaders from here on in the code (in every shader and rendering call).
    glUseProgram(shaderProgram);

    // clean up shader objects after linking.
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


// ===== VERTEX DATA AND HANDLING


 // 8. vertex data 

    // all UNIIQUE vertexes needed (so overlapping vertices only needed once)
    float exampleVertices[] = {
        -0.5f, -0.5f, 0.0f, // bottom left
         0.5f, -0.5f, 0.0f, // bottom right
         0.5f,  0.5f, 0.0f, // top right
        -0.5f,  0.5f, 0.0f  // top left
    };
    // order in which to visit vertices for each triangle in square
    unsigned int indices[] = {
        0, 1, 3, // triangle 1
        1, 2, 3  // triangle 2
    };


 // 9. Vetex Array & Buffer Objects: 

    // creates a vertex buffer (used to send data in chunks to the GPU).
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // bind VAO first, then bind and set VBO, and then configure vertex attributes
    glBindVertexArray(VAO);

    // binds the vertex buffer (data type GL_ARRAY_BUFFER) to the vertex buffer object
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // takes the (user-defined) vertix data into the current vertex buffer object (GL_ARRAY_BUFFER).
    // STATIC means the data will be set once and is used mutliple times (static image).
    glBufferData(GL_ARRAY_BUFFER, sizeof(exampleVertices), exampleVertices, GL_STATIC_DRAW);

    // repeat above for EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // the attributes here are as follows:
        // the vertex attribute we want to configure (set to 0 earlier using "layout(locatiom = 0)").
        // the size of the vertex attribute (3 because vec3).
        // type of data.
        // should the data be normalized (needed if different data type).
        // tells the stride (space between consecutive vertex attributes)
        // offset of the where the position data begins in buffer.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    

// ===== RENDER LOOP =====


 // 10. render loop:

    // main render loop.
    while (!glfwWindowShouldClose(window))
    {
     // input:
        processInput(window);

        
     // render commands:

        // R, G , B, opcaity (Alpha)
        glClearColor(1.0f, 0.0f, 0.5f, 1.0f);
        // clears the color buffer after so the frames dont bleed into each other
        glClear(GL_COLOR_BUFFER_BIT);

        // drawing the triangle
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

     // check and call events, replace buffers:

        // the buffer effectively is just all the data about what colors should be where on the screen.
        glfwSwapBuffers(window);
        // events are user input, etc.
        glfwPollEvents();  
    }


// ===== CLEANUP ======



 // 11. end main function:

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