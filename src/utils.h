#include <cstdint>
#include <iostream>
#include <vector>

namespace utils {
// readFile reads a file in the given path
std::string readFile(const char *filePath);

// checkShaderErrors returns errors when compiling a vertex or fragment shader
std::string checkShaderErrors(int shaderID);

// checkShaderProgramErrors returns errors when compiling a shader program
std::string checkShaderProgramErrors(int programID);

int time_now();

void print_bytes(const std::vector<uint8_t> &data);

void print(const char *input);

float minF(float a, float minVal);
} // namespace utils