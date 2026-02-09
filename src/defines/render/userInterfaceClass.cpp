#include "render/userInterfaceClass.hpp"

#include "imgui.h"                              // ImGui main library
#include "backends/imgui_impl_sdl3.h"           // ImGui SDL integration
#include "backends/imgui_impl_opengl3.h"        // ImGui OpenGL integration

#include <string>

// *************************************************

Window::Window(std::string name, ImGuiWindowFlags flags, bool show)
{
    name = name;
    flags = flags;
    visible = show;
}

void Window::draw()
{
    visible = true;

    ImGui::Begin(name.c_str(), &visible, flags);
    drawContents();
    ImGui::End();
}

void Window::add_text(const std::string text)
{

}