// *************************************************

// added error protetion around includes. library includes grouped.

#ifndef GLAD
  #include "glad/glad.h"          // OpenGL extension loader
  #define GLAD
#endif

#ifndef SDL
  #include "SDL3/SDL.h"           // SDL main library
  #include "SDL3/SDL_opengl.h"    // SDL OpenGL integration
  #define SDL
#endif

#ifndef IMGUI
  #include "imgui.h"                              // ImGui main library
  #include "backends/imgui_impl_sdl3.h"           // ImGui SDL integration
  #include "backends/imgui_impl_opengl3.h"        // ImGui OpenGL integration
  #define IMGUI
#endif

#ifndef GLM
  #include "glm/glm.hpp"                       // main GLM library
  #include "glm/vec3.hpp"                      // GLM vec3 class and methods - for position data
  #include "glm/mat4x4.hpp"                    // GLM 4x4 matrix class and methods - for transformations 
  #include "glm/gtc/matrix_transform.hpp"      // translate function
  #define GLM
#endif

// *************************************************

#ifndef MATH
  #include <math.h>               // contains basic math functions (such as trig)
  #define MATH
#endif

#ifndef STRING
  #include <string>              // has standard input/output functions
  #define STRING
#endif

#ifndef FSTREAM
  #include <fstream>             // has standard input/output functions
  #define FSTREAM
#endif

#ifndef VECTOR
  #include <vector>              // has standard input/output functions
  #define VECTOR
#endif

// *************************************************

#ifndef GLOBAL
  #include "defines/global.hpp"        
  #define GLOBAL
#endif

// *************************************************

// Load a shader from a file
// - Pass in the shader by file name. File name is passed only by reference to prevent duplication.
std::string load_shader_from_file(const std::string& fileName)
{
  // initialize result var
  std::string result = "";    // holds shader program as single string

  std::string line = "";  // holds one line of shader file at a time
  std::ifstream shaderFile(fileName.c_str());     // opens file

  // if the file is opened successfully
  if (shaderFile.is_open())
  {
    while(std::getline(shaderFile, line))   // go through each line of the file
    {
      result += line + "\n";                // concatinate new line into result string
    }
    shaderFile.close();                     // close file when done 
  }

  return result;
}



// Compiles veretx and fragment shaders.
// - Shader type is a (enum) OpenGL flag indicating the type of shader it is (vertex/fragment)
// - Source is the GLSL code for a shader passed as a C++ string
//
// If the shader compiles incorrectly, this function will cancel and log the appropriate error information.
GLuint compile_shader(GLuint type, const std::string source)
{
  // create shader object
  GLuint shaderObject;

  // check type of shader
  if (type == GL_VERTEX_SHADER)
  {
    shaderObject = glCreateShader(GL_VERTEX_SHADER);
  }
  else if (type == GL_FRAGMENT_SHADER)
  {
    shaderObject = glCreateShader(GL_FRAGMENT_SHADER);
  }

  // turn source to a C-string
  const char* src = source.c_str();

  // create shader source code
  glShaderSource
  (
    shaderObject,   // shader
    1,              // amount of elements compiled (1 shader)
    &src,           // shader source
    nullptr         // length of string 
  );

  // compile the shader
  glCompileShader(shaderObject);

  // error checking
  int result;
  glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &result);  // gets compile status, stores in result

  // if shaders failed to compile
  if (result == GL_FALSE)
  {
    int length;                                                     
    glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &length);           // find the length of the error message
    char* errorMessages = new char[length];                             // create a C-string of that length
    glGetShaderInfoLog(shaderObject, length, &length, errorMessages);   // log error info

    // display error messages
    if (type == GL_VERTEX_SHADER)
    {
    SDL_Log("GL_VERTEX_SHADER compilation failed! \n");
    SDL_Log("%s", errorMessages);
    }
    else if(type == GL_FRAGMENT_SHADER)
    {
      SDL_Log("GL_FRAGMENT_SHADER compilation failed! \n");
      SDL_Log("%s", errorMessages);
    }

    // reclaim memory
    delete[] errorMessages;

    // delete broken shader object
    glDeleteShader(shaderObject);

    return 0;
  }

  return shaderObject;
}


// Creates a shader program object.
// - The function passes in the vertex and fragment shaders' GLSL code as C++ strings
//
// The shaders are compiled within the function, and then attatcehed to the program object. The program object is then validated.
GLuint create_shader_program(const std::string source_vertexShader, const std::string source_fragmentShader)
{
  GLuint programObject = glCreateProgram(); // creates an empty program to be filled with shaders

  // compile shaders
  GLuint vertexShader = compile_shader(GL_VERTEX_SHADER, source_vertexShader);
  GLuint fragmentShader = compile_shader(GL_FRAGMENT_SHADER, source_fragmentShader);

  // attatch shaders to program object
  glAttachShader(programObject, vertexShader);    // attatches vertex shader to the object
  glAttachShader(programObject, fragmentShader);
  glLinkProgram(programObject);   // links shaders together within object

  // validate program - check for errors
  glValidateProgram(programObject); 

  return programObject;
}


// Creates a shader program using the shaders found in the shader folder. The shader program is referenced using a unique unsigned integer value assigned as its ID.
void create_graphics_pipeline()
{
  std::string source_vertexShader = load_shader_from_file("./shaders/vertexShader.glsl");
  std::string source_fragmentShader = load_shader_from_file("./shaders/fragmentShader.glsl");

  global::shaderProgram = create_shader_program(source_vertexShader, source_fragmentShader);
}