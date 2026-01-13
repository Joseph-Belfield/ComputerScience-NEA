#pragma once

// must be included so that types below work
#include "glad/glad.h"
#include "glm/glm.hpp"
#include <string>

// forward declare classes, structs 
struct Context;

namespace render
{
    void init_SDL(Context* globalContext);

    void set_OpenGL_Attributes(Context* globalContext);

    void init_OpenGL(Context* globalContext);

    void init_ImGui(Context* globalContext);

    // *************************************************

    void vertex_specification(Context* globalContext);

    // *************************************************

    // Load a shader from a file
    // - Pass in the shader by file name. File name is passed only by reference to prevent duplication.
    std::string load_shader_from_file(const std::string& fileName);

    // Compiles veretx and fragment shaders.
    // - Shader type is a (enum) OpenGL flag indicating the type of shader it is (vertex/fragment)
    // - Source is the GLSL code for a shader passed as a C++ string
    //
    // If the shader compiles incorrectly, this function will cancel and log the appropriate error information.
    GLuint compile_shader(GLuint type, const std::string source);

    // Creates a shader program object.
    // - The function passes in the vertex and fragment shaders' GLSL code as C++ strings
    //
    // The shaders are compiled within the function, and then attatcehed to the program object. The program object is then validated.
    GLuint create_shader_program(const std::string source_vertexShader, const std::string source_fragmentShader);

    // Creates a shader program using the shaders found in the shader folder. The shader program is referenced using a unique unsigned integer value assigned as its ID.
    void create_graphics_pipeline(Context* globalContext);

    // *************************************************

    void check_events(Context* globalContext);

    // Creates a uniform matrix, and returns its GLuint ID.
    GLuint create_uniform_mat4(GLuint shaderProgram, std::string uniformName, int amount, bool enableTranspose, glm::mat4 matrix);

    // Creates a model matrix.
    // - Objects begin in local space, where they are created on their own set of axis
    // - The model matrix moves objects from local space to world space, where objects are all held relative to one shared set of axis
    //
    // The model matrix is also edited accordingly to change an objects position/rotation in world space accordingly.
    void model_matrix(Context* globalContext);

    // Creates a view matrix.
    // - The scene is viewed as if through a camera for the viewer.
    // - The view matrix rotates objects around the viewer to form the illusion of a a camera.
    void view_matrix(Context* globalContext);

    // Creates a projection matrix.
    // - The projection matrix creates the illusion of perspective
    // - It does this by changing a point's coordinates according to distance from the camera (Z-value)
    void perspective_matrix(Context* globalContext);

    // Handles tasks that must be completed before draw:
    // - OpenGL preferences
    // - Sets glViewport
    // - Sets clear color (background color)
    // - Applies transformation matrices
    void preDraw_OpenGL(Context* globalContext);

    // for drawing OpenGL data
    void draw_OpenGL(Context* globalContext);

    void draw_ImGui(Context* globalContext);

    void run_loop(Context* globalContext);

    // *************************************************

    void clean_ImGui();

    void clean_SDL(Context* globalContext);
}