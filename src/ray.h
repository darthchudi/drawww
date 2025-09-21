#include "../vendor/glad/gl.h"
#include "../vendor/glfw/include/GLFW/glfw3.h"
#include "../vendor/glm/glm/glm.hpp"
#include "../vendor/glm/glm/gtc/matrix_transform.hpp"
#include "../vendor/glm/glm/gtc/type_ptr.hpp"

// getMousePositionNDC returns the mouse position within the window
// in normalised device coordinates (NDC) With values in the range [-1, 1].
glm::vec2 getMousePositionNDC(GLFWwindow *window);