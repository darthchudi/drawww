#include "../vendor/glad/gl.h"
#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

namespace utils {
// readFile reads a file in the given path
std::string readFile(const char *filePath) {
  std::ifstream fileStream;
  std::stringstream fileStringStream;

  // Open the file
  fileStream.open(filePath);
  if (!fileStream.is_open()) {
    // todo: add file name to error
    throw std::runtime_error("failed to open file");
  }

  // Copy the file stream into a string stream
  fileStringStream << fileStream.rdbuf();

  // Close the file stream
  fileStream.close();

  // Convert the file stream into a string
  return fileStringStream.str();
}

// checkShaderErrors returns errors when compiling a vertex or fragment shader
std::string checkShaderErrors(int shaderID) {
  int success;
  char infoLog[1024];

  glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
  if (success) {
    return "";
  }

  glGetShaderInfoLog(shaderID, 1024, NULL, infoLog);

  std::string errMessage = std::string("Shader compilation error: ") + infoLog;
  return errMessage;
}

// checkShaderProgramErrors returns errors when compiling a shader program
std::string checkShaderProgramErrors(int programID) {
  int success;
  char infoLog[1024];

  glGetProgramiv(programID, GL_LINK_STATUS, &success);
  if (success) {
    return "";
  }

  glGetProgramInfoLog(programID, 1024, NULL, infoLog);

  std::string errMessage =
      std::string("Shader program linking error: ") + infoLog;
  return errMessage;
}

int time_now() { return 1; }

void print_bytes(const std::vector<uint8_t> &data) {
  for (const uint8_t byte : data) {
    std::cout << "" << static_cast<int>(byte) << "";
  }
}

void print(const char *input) { std::cout << input; }

float minF(float a, float minVal) {
    if (a <= minVal)
      return minVal;
  
    return a;
  }  
} // namespace utils