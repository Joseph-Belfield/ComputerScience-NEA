#pragma once

#include "imgui/imgui.h"

#include <string>

class Window
{
public:
    Window(std::string name = "template", ImGuiWindowFlags flags = 0, bool show = false);

    // draw the window (this frame)
    void draw();
    void hide();

    void change_name(std::string newName);

    // add things to windows
    void set_text(const std::string text);    // set string value at time

    void set_slider(float* value, std::string name, float min = 0, float max = 1);
    void set_sliderRange(float min, float max);

    void set_colorPicker();

private:
    std::string name;
    bool visible;
    ImGuiWindowFlags flags;

    bool showText = false;
    bool showSlider = false;
    bool showColorPicker = false;

    std::string text;

    float* slider = nullptr;
    std::string sliderName;
    float min;
    float max;

    void drawContents();

    void use_text();
    void use_slider();
    void use_colorPicker();
};