#pragma once

// *************************************************
#include "glad/gl.h"
#include "glm/glm.hpp"

#include <vector>

// forward declare classes, structs 
struct appData;
class Object;
class Camera;

// *************************************************

void check_events(appData &appData);

// *************************************************

// Updates parts of the scene not specific to the object, such as:
// - background color
// - view and perspective matrices
// - graphics pipeline
void update_scene(appData &appData);

// *************************************************

void draw_objects(std::vector<Object*> objects, Camera* camera, float width, float height);

// *************************************************

void draw_ImGui(appData &appData);