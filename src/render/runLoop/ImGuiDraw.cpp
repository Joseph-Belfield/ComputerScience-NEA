#include "runLoop.hpp"

#include "defines/appData.hpp"
#include "defines/render/objectData.hpp"


#include "imgui.h"                              // ImGui main library
#include "backends/imgui_impl_sdl3.h"           // ImGui SDL integration
#include "backends/imgui_impl_opengl3.h"        // ImGui OpenGL integration



void draw_ImGui(appData &appData)
{
    if (appData.ImGui.show_mainWindow)
    {
        ImGui::Begin("Main Window", &(appData.ImGui.show_mainWindow), ImGuiWindowFlags_MenuBar);  
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Options"))
            {
                if (ImGui::MenuItem("Change Background Color")) {appData.ImGui.show_colorPicker = true;}
                if (ImGui::MenuItem("Sine Graph")) {appData.ImGui.show_sineGraph = true;}
                if (ImGui::MenuItem("Scrolling")) {appData.ImGui.show_scrolling = true;}

                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        if (ImGui::Button("Hello World"))
        {
            appData.ImGui.show_helloWorld = !appData.ImGui.show_helloWorld;
        }

        ImGui::SliderFloat("Sensitivity", &(appData.camera.sense), 0.01f, 2.0f);
        ImGui::SliderFloat("Speed", &(appData.camera.speed), 0.01f, 5.0f);

        ImGui::End();
    }


    if (appData.ImGui.show_helloWorld)
    {
        ImGui::Text("Hello World!");
    }

    if (appData.ImGui.show_sineGraph)
    {
        ImGui::Begin("Sine Graph", &(appData.ImGui.show_sineGraph));
        float samples[100];
        for (int i = 0; i < 100; i++)
        {
        samples[i] = sinf(i * 0.2f + ImGui::GetTime() * 1.5f);
        }
        ImGui::PlotLines("Samples", samples, 100);
        ImGui::End();
    }

    if (appData.ImGui.show_scrolling)
    {
        // window contents in scrolling region
        ImGui::Begin("Scrolling", &(appData.ImGui.show_scrolling));
        ImGui::TextColored(ImVec4(1,1,0,1), "Important Stuff");
        ImGui::BeginChild("Scrolling");
        for (int i = 0; i < 15; i++)
        {
        ImGui::Text("Some text");
        }
        ImGui::EndChild();
        ImGui::End();
    }

    if (appData.ImGui.show_colorPicker)
    {
        // create a window with menu bar called "Color Picker"
        ImGui::Begin("Background Color", &(appData.ImGui.show_colorPicker));
        // edit a color stored as 4 floats
        ImGui::ColorEdit3("Color", (float*)&(appData.window.clearColor));
        ImGui::End();
    }
}