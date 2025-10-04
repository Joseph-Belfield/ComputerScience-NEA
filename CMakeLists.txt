cmake_minimum_required(VERSION 3.10.0)
project(CS-NEA VERSION 0.1.0 LANGUAGES C CXX)
cmake_policy(SET CMP0072 NEW)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(CS-NEA main.cpp  
    src/main.cpp
    src/glad.c
)

target_include_directories(CS-NEA PRIVATE dependencies)

target_link_libraries(cmake_test glfw OpenGL::GL)
