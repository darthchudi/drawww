#ifndef SHADER_H
#define SHADER_H
#include "../vendor/glad/gl.h"
#include "../vendor/glfw/include/GLFW/glfw3.h"

// Shader is shader program containing a vertex shader and fragment shader
class Shader {
public:
  // ID is the identifer of the shader program
  unsigned int ID;

  // Shader initialises a new shader
  Shader(const char *vertexShaderPath, const char *fragmentShaderPath);

  // use activates the shader program
  void use();

  // set_uniforms sets the values for render loop uniforms
  void set_uniforms();

  // print displays the shader program ID
  void print();
};

#endif // SHADER_H