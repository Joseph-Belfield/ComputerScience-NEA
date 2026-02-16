#include "render/runLoop.hpp"

#include "appData.hpp"
#include "chemistry/molecule.hpp"
#include "render/userInterfaceClass.hpp"
#include "render/objectData.hpp"


#include "imgui.h"                              // ImGui main library
#include "backends/imgui_impl_sdl3.h"           // ImGui SDL integration
#include "backends/imgui_impl_opengl3.h"        // ImGui OpenGL integration

void update_molecule(Compound& compound, int molecule)
{
    switch(molecule)
    {
        case(0):
        {
            compound = WATER;
            break;
        }
        case(1):
        {
            compound = METHANE;
            break;
        }
        case(2):
        {
            compound = ETHANE;
            break;
        }
        case(3):
        {
            compound = PROPANE;
            break;
        }
        case(4):
        {
            compound = CYCLOHEXANE;
            break;
        }
        case(5):
        {
            compound = OPTICAL_ISOMER;
            break;
        }
        case(6):
        {
            compound = ETHENE_ISOMER;
            break;
        }
    }
}

void draw_ImGui(appData &appData)
{
    static int currentItem = 0;

    if (appData.ImGui.show_mainWindow)
    {
        ImGui::Begin("Main Window", &(appData.ImGui.show_mainWindow));  

        if (ImGui::Button("Background Color"))
        {
            appData.ImGui.show_colorPicker = !appData.ImGui.show_colorPicker;
        }

        if (ImGui::Button("Controls List"))
        {
            appData.ImGui.show_controls = !appData.ImGui.show_controls;
        }

        if (ImGui::Button("User Settings"))
        {
            appData.ImGui.show_settings = !appData.ImGui.show_settings;
        }

        const char* items[] = {"WATER", "METHANE", "ETHANE", "PROPANE", "CYCLOHEXANE", "OPTICAL ISOMER", "STEREOISOMER"};
        const char* combo_preview_value = items[currentItem];
        if (ImGui::BeginCombo("Molecules", combo_preview_value))
        {
            for (int n = 0; n < IM_COUNTOF(items); n++)
            {
                const bool is_selected = (currentItem == n);
                if (ImGui::Selectable(items[n], is_selected))
                    currentItem = n;

                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        ImGui::End();
    }

    update_molecule(appData.displayedMolecule, currentItem);

    if (appData.ImGui.show_colorPicker)
    {
        // create a window with menu bar called "Color Picker"
        ImGui::Begin("Background Color", &(appData.ImGui.show_colorPicker));
        // edit a color stored as 4 floats
        ImGui::ColorEdit3("Color", (float*)&(appData.window.clearColor));
        ImGui::End();
    }

    if (appData.ImGui.show_controls)
    {
        ImGui::Begin("Controls", &appData.ImGui.show_controls);


        ImGui::Text("Use your cursor to to control the camera.");
        ImGui::Text("Use WASD keys for directional movement.");
        ImGui::Text("'O' to ascend.");
        ImGui::Text("'P' to descend.");
        ImGui::Text("'tab' will open and close the menus. The camera will only move while menus are closed.");

        ImGui::End();
    }

    if (appData.ImGui.show_settings)
    {
        ImGui::Begin("Settings", &appData.ImGui.show_settings);

        ImGui::SliderFloat("Sensitivity", &(appData.camera.sense), 0.01f, 2.0f);
        ImGui::SliderFloat("Speed", &(appData.camera.speed), 0.01f, 5.0f);

        ImGui::End();
    }
}