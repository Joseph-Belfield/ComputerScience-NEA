cmake_minimum_required(VERSION 3.15)   # sets the CMake version
project(ChemS_Paint LANGUAGES C CXX)   # names the project and sets which languages are used

set(CMAKE_CXX_STANDARD 17)      # sets the C++ standard (effectively version, defining how many features it has) to 17
set(CMAKE_CXX_STANDARD_REQUIRED ON)     # makes use of C++17 or higher mandatory - or code will run into errors later

# Add GLFW
add_subdirectory(dependencies/glfw)     # makes the compiler aware of the GLFW library route

# Add GLAD
add_library(glad dependencies/glad/src/glad.c)      # adds GLAD as a library to the compiler
target_include_directories(glad PUBLIC dependencies/glad/include)   # tells compiler to include glad directory

# Include directories for your source
include_directories(include)    # tells compiler to include header and include files found in include directory

# Main executable
file(GLOB_RECURSE SRC_FILES src/*.cpp)      # creates a list of all files in the source folder that are .cpp -> searches recursively in source folder
add_executable(ChemS_Paint ${SRC_FILES})    # creates the executable file from the source file list (named ChemS_Paint)

# Link everything
target_link_libraries(ChemS_Paint PRIVATE glfw glad)    # links all the libraries from earlier to the source

# On macOS, link Cocoa and OpenGL frameworks
if(APPLE)   # if the platform is iOS or MacOS, link the following libraries to ChemS_Paint:
    #       Cocoa - handles GUI on MacOS (such as window creation)
    #       OpenGL - the OpenGL system on MacOS (the important bit!)
    #       IOKit - used to take keyboard/mouse input, etc
    #       CoreVideo - used to synchronize frames (important for rendering, which works frame by frame)
    target_link_libraries(ChemS_Paint PRIVATE "-framework Cocoa" "-framework OpenGL" "-framework IOKit" "-framework CoreVideo")
endif()

# On Linux, link X11 and GL
if(UNIX AND NOT APPLE)
    # if using UNIX and not MacOS or iOS (effectively just saying Linux, as thats the only other user):
    # find the OpenGL package for the system
    # X11 - a library responsible for window creation and input data on linux, creates render area for OpenGL
    # pthread manages multithreading (running multiple lines of commands simulataniously) on linux 
    find_package(OpenGL REQUIRED)
    target_link_libraries(ChemS_Paint PRIVATE ${OPENGL_LIBRARIES} X11 pthread)
endif()

# On Windows, GLFW already links OpenGL

# CMake file created with the assistance of ChatGPT, annotated and explained by yours truly