#pragma once

// *************************************************
#include "glad/gl.h"
#include "glm/glm.hpp"

// forward declare classes, structs 
struct appData;
struct Object;

// *************************************************

void check_events(appData &appData);

// *************************************************

// Creates a uniform matrix, and returns its GLuint ID.
GLuint create_uniform_mat4(GLuint shaderProgram, std::string uniformName, int amount, bool enableTranspose, glm::mat4 matrix);


// Creates a model matrix.
// - Objects begin in local space, where they are created on their own set of axis
// - The model matrix moves objects from local space to world space, where objects are all held relative to one shared set of axis
//
// The model matrix is also edited accordingly to change an objects position/rotation in world space accordingly.
void model_matrix(Object &objectData);

// For handling color uniforms

// Creates a view matrix.
// - The scene is viewed as if through a camera for the viewer.
// - The view matrix rotates objects around the viewer to form the illusion of a a camera.
void view_matrix(appData &appData);

// Creates a projection matrix.
// - The projection matrix creates the illusion of perspective
// - It does this by changing a point's coordinates according to distance from the camera (Z-value)
void perspective_matrix(appData &appData);

// *************************************************

// Updates parts of the scene not specific to the object, such as:
// - background color
// - view and perspective matrices
// - graphics pipeline
void update_scene(appData &appData);

// *************************************************

// Handles tasks that must be completed before draw:
// - OpenGL preferences
// - Sets glViewport
// - Sets clear color (background color)
// - Applies transformation matrices
void update_mesh(Object &objectData);

// for drawing OpenGL data
void draw_mesh(Object &objectData);

// *************************************************

void draw_ImGui(appData &appData);