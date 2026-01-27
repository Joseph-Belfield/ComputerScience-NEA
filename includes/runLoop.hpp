#pragma once

// *************************************************
#include "glad/gl.h"
#include "glm/glm.hpp"

#include <vector>

// forward declare classes, structs 
struct appData;
struct Object;


// *************************************************

void check_events(appData &appData);

// *************************************************

// Creates a uniform matrix, and returns its GLuint ID.
GLuint create_uniform_mat4(GLuint shaderProgram, std::string uniformName, int amount, bool enableTranspose, glm::mat4 matrix);

// Creates a uniform for 4 floats, returns its GLuint ID.
GLuint create_uniform_float4(GLuint shaderProgram, std::string uniformName, glm::vec4 vec4);

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

void draw_objects(std::vector<Object*> objects);

// *************************************************

void draw_ImGui(appData &appData);