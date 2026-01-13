#include "render.hpp"
#include "defines/everythingClass.hpp"
#include "defines/camera.hpp"

// *************************************************

#include "glad/glad.h"          // OpenGL extension loader

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

// *************************************************

namespace render
{
    void check_events(Context* globalContext)
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
                globalContext -> flag_mainLoop = false;
            }
            // if Esc key is pressed, end the run loop
            if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_ESCAPE) 
            {
                globalContext -> flag_mainLoop = false;
            }

            if (event.key.key == SDLK_W)
            {
                globalContext -> uDisplacement[1]+=0.1f;
            }
            if (event.key.key == SDLK_A)
            {
                globalContext -> uDisplacement[0]-=0.1f;
            }
            if (event.key.key == SDLK_S)
            {
                globalContext -> uDisplacement[1]-=0.1f;
            }
            if (event.key.key == SDLK_D)
            {
                globalContext -> uDisplacement[0]+=0.1f;
            }

            if (event.key.key == SDLK_UP)
            {
               globalContext ->  uOffset+=0.1f;
            }
            if (event.key.key == SDLK_DOWN)
            {
                globalContext -> uOffset-=0.1f;
            }

            if (event.key.key == SDLK_RIGHT)
            {
                globalContext -> uRotate+=5.0f;
            }
            if (event.key.key == SDLK_LEFT)
            {
                globalContext -> uRotate-=5.0f;
            }

            if (event.key.key == SDLK_P)
            {
                globalContext -> uScale+=0.1f;
            }
            if (event.key.key == SDLK_O)
            {
                globalContext -> uScale-=0.1f;
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
            enableTranspose,
            &matrix[0][0]
            );

            return uniformLocation;
        }
        else
        {
            SDL_Log("Could not find uniform location!");
            SDL_Log("Check spelling.");
            exit(-1);
        }
    }


    // Creates a model matrix.
    // - Objects begin in local space, where they are created on their own set of axis
    // - The model matrix moves objects from local space to world space, where objects are all held relative to one shared set of axis
    //
    // The model matrix is also edited accordingly to change an objects position/rotation in world space accordingly.
    void model_matrix(Context* globalContext)
    {
        // create and adapt the matrix to adjust the following transformations
        glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(globalContext -> uDisplacement[0], globalContext -> uDisplacement[1], globalContext -> uOffset)); // movement
        modelMatrix = glm::rotate(modelMatrix ,glm::radians(globalContext -> uRotate), glm::vec3(0.0f, 1.0f, 0.0f));  // rotations
        modelMatrix = glm::scale(modelMatrix, glm::vec3(globalContext -> uScale, globalContext -> uScale, globalContext -> uScale));

        GLuint uLocation_modelMatrix = create_uniform_mat4(globalContext -> shaderProgram, "uModelMatrix", 1, false, modelMatrix);
    }


    // Creates a view matrix.
    // - The scene is viewed as if through a camera for the viewer.
    // - The view matrix rotates objects around the viewer to form the illusion of a a camera.
    void view_matrix(Context* globalContext)
    {
    glm::mat4 viewMatrix = globalContext -> camera -> get_view_matrix();

    GLuint uLocation_viewMatrix = create_uniform_mat4(globalContext -> shaderProgram, "uViewMatrix", 1, false, viewMatrix);
    }

    // Creates a projection matrix.
    // - The projection matrix creates the illusion of perspective
    // - It does this by changing a point's coordinates according to distance from the camera (Z-value)
    void perspective_matrix(Context* globalContext)
    {
        // projection matrix (in perspective)
        glm::mat4 perspective = glm::perspective                                             // create perspective matrix
                                (
                                    45.0f,                                                     // FOV (radians)
                                    (float)(globalContext -> window_width / globalContext -> window_height),     // aspect ratio
                                    0.1f,                                                      // near clipping plane (min. distance)
                                    10.0f                                                      // far clipping plane (max. distance)
                                );         

        GLuint uLocation_perpective = create_uniform_mat4(globalContext -> shaderProgram, "uPerspective", 1, false, perspective);
    }


    // Handles tasks that must be completed before draw:
    // - OpenGL preferences
    // - Sets glViewport
    // - Sets clear color (background color)
    // - Applies transformation matrices
    void preDraw_OpenGL(Context* globalContext)
    {
        // disables
        glDisable(GL_DEPTH_TEST); // disables depth check - 2D scene
        glDisable(GL_CULL_FACE);  // disables checking for overlap - 2D scene

        // set size of window for OpenGL
        glViewport(0, 0, (int)(globalContext -> window_width), (int)(globalContext -> window_height));

        // background color
        glClearColor(globalContext -> clearColor.x, globalContext -> clearColor.y, globalContext -> clearColor.z, globalContext -> clearColor.w);                   // sets background color
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);     // clears the OpenGL color and depth buffers

        glUseProgram(globalContext -> shaderProgram);            // selects program in use

        // transformation matrices
        model_matrix(globalContext);         // controls position and rotation on world axis
        view_matrix(globalContext);
        perspective_matrix(globalContext);   // creats illusion of perspective (size changes relative to camera)
    }



    // for drawing OpenGL data
    void draw_OpenGL(Context* globalContext)
    {
        // choose VAO and VBO
        glBindVertexArray(globalContext -> vertexArrayObject);

        // draw
        glDrawElements
        (
            GL_TRIANGLES,         // shape
            36,                    // number of vertices drawn to (count repeats)
            GL_UNSIGNED_INT,      // data type
            0                     // offset into index array for first element (triangle vertex order)
        );
    }



    void draw_ImGui(Context* globalContext)
    {
        ImGui::Begin("Main Window", &(globalContext -> show_mainWindow), ImGuiWindowFlags_MenuBar);  

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Options"))
            {
            if (ImGui::MenuItem("Change Background Color")) {globalContext -> show_colorPicker = true;}
            if (ImGui::MenuItem("Sine Graph")) {globalContext -> show_sineGraph = true;}
            if (ImGui::MenuItem("Scrolling")) {globalContext -> show_scrolling = true;}
            ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        if (ImGui::Button("Hello World"))
        {
            globalContext -> show_helloWorld = true;
        }

        ImGui::End();



        if (globalContext -> show_helloWorld)
        {
            ImGui::Text("Hello World!");
        }

        if (globalContext -> show_sineGraph)
        {
            ImGui::Begin("Sine Graph", &(globalContext -> show_sineGraph));
            float samples[100];
            for (int i = 0; i < 100; i++)
            {
            samples[i] = sinf(i * 0.2f + ImGui::GetTime() * 1.5f);
            }
            ImGui::PlotLines("Samples", samples, 100);
            ImGui::End();
        }

        if (globalContext -> show_scrolling)
        {
            // display contents in scrolling region
            ImGui::Begin("Scrolling", &(globalContext -> show_scrolling));
            ImGui::TextColored(ImVec4(1,1,0,1), "Important Stuff");
            ImGui::BeginChild("Scrolling");
            for (int i = 0; i < 15; i++)
            {
            ImGui::Text("Some text");
            }
            ImGui::EndChild();
            ImGui::End();
        }

        if (globalContext -> show_colorPicker)
        {
            // create a window with menu bar called "Color Picker"
            ImGui::Begin("Background Color", &(globalContext -> show_colorPicker));
            // edit a color stored as 4 floats
            ImGui::ColorEdit3("Color", (float*)&(globalContext -> clearColor));
            ImGui::End();
        }
    }



    void run_loop(Context* globalContext)
    {

        while (globalContext -> flag_mainLoop) {

            check_events(globalContext);

            // starts a new frame for OpenGL, SDL and ImGui
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplSDL3_NewFrame();
            ImGui::NewFrame();

            // ********************** DO STUFF HERE **********************

            draw_ImGui(globalContext);

            preDraw_OpenGL(globalContext);
            draw_OpenGL(globalContext);


            // render
            ImGui::Render();                                               // renders ImGui instructions 
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());        // renders the ImGui data with OpenGL  
            SDL_GL_SwapWindow(globalContext -> window);                                     // swaps in the new frame

        }
    }
}