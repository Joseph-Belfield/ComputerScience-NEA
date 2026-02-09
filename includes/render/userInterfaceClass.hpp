#pragma once

#include <string>

class Window
{
public:
    std::string name = "template";
    bool visible = false;
    ImGuiWindowFlags flags;
    
    Window(std::string name, ImGuiWindowFlags flags, bool show = false);

    // draw the window (this frame)
    void draw();

    // add things to windows
    void add_text(const std::string text);    // set string value at time
    void add_text(std::string* text);   // reference to another string in the code

    void add_slider(float* value, float min = 0, float max = 1);

private:
    void drawContents();
};