#include "render.hpp"
#include "defines/contextData.hpp"
#include "defines/camera.hpp"

// *************************************************

#include "glad/gl.h"            // OpenGL extension loader

#include "SDL3/SDL.h"           // SDL main library
#include "SDL3/SDL_opengl.h"    // SDL OpenGL integration

#include "imgui.h"                              // ImGui main library
#include "backends/imgui_impl_sdl3.h"           // ImGui SDL integration
#include "backends/imgui_impl_opengl3.h"        // ImGui OpenGL integration

#include "glm/glm.hpp"                       // main GLM library
#include "glm/vec3.hpp"                      // GLM vec3 class and methods - for position data
#include "glm/mat4x4.hpp"                    // GLM 4x4 matrix class and methods - for transformations 
#include "glm/gtc/matrix_transform.hpp"      // translate function


// *************************************************

#include <string>              // has standard input/output functions
#include <iostream>

// *************************************************

namespace render
{
    void check_events(appData &appData)
    {
        // checks for events
        SDL_Event event;

        // if there were events, do:
        while (SDL_PollEvent(&event)) {

            // ImGui processes the event
            ImGui_ImplSDL3_ProcessEvent(&event);

            // if SDL is quit, end the run loop
            if (event.type == SDL_EVENT_QUIT) 
            {
                appData.window.flag_mainLoop = false;
            }
            // if Esc key is pressed, end the run loop
            if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_ESCAPE) 
            {
                appData.window.flag_mainLoop = false;
            }

            if (event.key.key == SDLK_W)
            {
                appData.camera.camera1.move_forward(1.0f);
            }
            if (event.key.key == SDLK_S)
            {
                appData.camera.camera1.move_backward(1.0f);
            }

            if (event.key.key == SDLK_UP)
            {
               appData.uniform.uOffset += 0.1f;
            }
            if (event.key.key == SDLK_DOWN)
            {
                appData.uniform.uOffset -= 0.1f;
            }

            if (event.key.key == SDLK_P)
            {
                appData.uniform.uScale += 0.1f;
            }
            if (event.key.key == SDLK_O)
            {
                appData.uniform.uScale -= 0.1f;
            }
        }
    }


    // Creates a uniform matrix, and returns its GLuint ID.
    GLuint create_uniform_mat4(GLuint shaderProgram, std::string uniformName, int amount, bool enableTranspose, glm::mat4 matrix)
    {
        GLuint uniformLocation = glGetUniformLocation(shaderProgram, uniformName.c_str());
        if (uniformLocation >= 0)
        {
            glUniformMatrix4fv
            (
                uniformLocation,
                amount,
                false,
                &matrix[0][0]
            );

            return uniformLocation;
        }
        else
        {
            std::cout << "Could not find uniform - check spelling!" << std::endl;
            exit(-1);
        }
    }


    // Creates a model matrix.
    // - Objects begin in local space, where they are created on their own set of axis
    // - The model matrix moves objects from local space to world space, where objects are all held relative to one shared set of axis
    //
    // The model matrix is also edited accordingly to change an objects position/rotation in world space accordingly.
    void model_matrix(appData &appData)
    {
        // create and adapt the matrix to adjust the following transformations
        glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(appData.uniform.uDisplacement[0], appData.uniform.uDisplacement[1], appData.uniform.uOffset)); // movement
        modelMatrix = glm::rotate(modelMatrix ,glm::radians(appData.uniform.uRotate), glm::vec3(0.0f, 1.0f, 0.0f));  // rotations
        modelMatrix = glm::scale(modelMatrix, glm::vec3(appData.uniform.uScale, appData.uniform.uScale, appData.uniform.uScale));

        GLuint location_modelMatrix = create_uniform_mat4(appData.OpenGL.shaderProgram, "uModelMatrix", 1, false, modelMatrix);
    }


    // Creates a view matrix.
    // - The scene is viewed as if through a camera for the viewer.
    // - The view matrix rotates objects around the viewer to form the illusion of a a camera.
    void view_matrix(appData &appData)
    {
        glm::mat4 viewMatrix = appData.camera.camera1.get_view_matrix();
        GLuint location_viewMatrix = create_uniform_mat4(appData.OpenGL.shaderProgram, "uViewMatrix", 1, false, viewMatrix);
    }

    // Creates a projection matrix.
    // - The projection matrix creates the illusion of perspective
    // - It does this by changing a point's coordinates according to distance from the camera (Z-value)
    void perspective_matrix(appData &appData)
    {
        // projection matrix (in perspective)
        glm::mat4 perspective = glm::perspective      // create perspective matrix
                                (
                                    glm::radians(45.0f),                                                                     // FOV (radians)
                                    (float)(appData.display.window_width / appData.display.window_height),     // aspect ratio
                                    0.1f,                                                                      // near clipping plane (min. distance)
                                    10.0f                                                                      // far clipping plane (max. distance)
                                );         

        GLuint location_perspective = create_uniform_mat4(appData.OpenGL.shaderProgram, "uPerspective", 1, false, perspective);
    }


    // Handles tasks that must be completed before draw:
    // - OpenGL preferences
    // - Sets glViewport
    // - Sets clear color (background color)
    // - Applies transformation matrices
    void preDraw_OpenGL(appData &appData)
    {

        // disables
        // glDisable(GL_DEPTH_TEST); // disables depth check - 2D scene
        // glDisable(GL_CULL_FACE);  // disables checking for overlap - 2D scene

        glEnable(GL_DEPTH_TEST);

        // set size of window for OpenGL
        glViewport(0, 0, (int)(appData.display.window_width), (int)(appData.display.window_height));

        // background color
        glClearColor(appData.display.clearColor.x, appData.display.clearColor.y, appData.display.clearColor.z, appData.display.clearColor.w);                   // sets background color
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);     // clears the OpenGL color and depth buffers

        // selects program in use
        glUseProgram(appData.OpenGL.shaderProgram);  

        // transformation matrices
        model_matrix(appData);         // controls position and rotation on world axis
        view_matrix(appData);          // Makes a camera work!
        perspective_matrix(appData);   // creats illusion of perspective (size changes relative to camera)

        
    }



    // for drawing OpenGL data
    void draw_OpenGL(appData &appData)
    {
        // choose VAO and VBO
        glBindVertexArray(appData.OpenGL.vertexArrayObject);

        // draw
        glDrawElements
        (
            GL_TRIANGLES,         // shape
            6,                    // number of vertices drawn to (count repeats)
            GL_UNSIGNED_INT,      // data type
            0                     // offset into index array for first element (triangle vertex order)
        );

        int error = glGetError();
        if (error)
        {
            std::cout << "ERROR: " << error << std::endl;
        }

        // unbind VAO after shape drawn
        glBindVertexArray(0);
    }



    void draw_ImGui(appData &appData)
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
            appData.ImGui.show_helloWorld = true;
        }

        ImGui::End();



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
            // display contents in scrolling region
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
            ImGui::ColorEdit3("Color", (float*)&(appData.display.clearColor));
            ImGui::End();
        }
    }



    void run_loop(appData &appData)
    {

        while (appData.window.flag_mainLoop) {

            check_events(appData);

            // starts a new frame for OpenGL, SDL and ImGui
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplSDL3_NewFrame();
            ImGui::NewFrame();

            // ********************** DO STUFF HERE **********************            

            preDraw_OpenGL(appData);
            draw_OpenGL(appData);

            draw_ImGui(appData);

            appData.uniform.uRotate += 2.0f;

            // render
            ImGui::Render();                                               // renders ImGui instructions 
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());        // renders the ImGui data with OpenGL  
            SDL_GL_SwapWindow(appData.window.window);                      // swaps in the new frame

        }
    }
}