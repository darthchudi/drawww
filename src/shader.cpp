#include "shader.h"
#include "utils.h"
#include <exception>
#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <stdio.h>

Shader::Shader(const char *vertexShaderPath, const char *fragmentShaderPath) {
  // Read the shader files
  std::string vertexShaderRaw = utils::readFile(vertexShaderPath);
  std::string fragmentShaderRaw = utils::readFile(fragmentShaderPath);

  // Compile the vertex shader
  const char *vertexShaderCode = vertexShaderRaw.c_str();

  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
  glCompileShader(vertexShader);

  std::string vertexShaderErr = utils::checkShaderErrors(vertexShader);
  if (vertexShaderErr != "") {
    throw std::runtime_error(std::string(
        "failed to compile vertex shader file: " + vertexShaderErr));
  }

  // Compile the fragment shader
  const char *fragmentShaderCode = fragmentShaderRaw.c_str();

  unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
  glCompileShader(fragmentShader);

  std::string fragmentShaderErr = utils::checkShaderErrors(fragmentShader);
  if (fragmentShaderErr != "") {
    throw std::runtime_error(std::string(
        "failed to compile fragment shader file: " + fragmentShaderErr));
  }

  // Create the shader program and link the shaders
  this->ID = glCreateProgram();
  glAttachShader(this->ID, vertexShader);
  glAttachShader(this->ID, fragmentShader);
  glLinkProgram(this->ID);

  std::string shaderProgramErr = utils::checkShaderProgramErrors(this->ID);
  if (shaderProgramErr != "") {
    throw std::runtime_error(
        std::string("failed to compile shader program: " + shaderProgramErr));
  }

  // Cleanup the shaders after linking
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}

// set_uniforms sets the values for render loop uniforms
void Shader::set_uniforms() {
  float timeValue = glfwGetTime();
  float greenValue = (sin(timeValue) / 2.0f) + 0.5f;

  int vertexColorLocation = glGetUniformLocation(this->ID, "ourUniformColor");

  float normalisedGreen = utils::minF(greenValue, 0.8f);

  glUniform4f(vertexColorLocation, 1.0f, normalisedGreen, 1.0f, 1.0f);
}

void Shader::use() { glUseProgram(this->ID); }

void Shader::print() { printf("Shader program ID %d\n", this->ID); };
