#include "../vendor/glad/gl.h"
#include "../vendor/glfw/include/GLFW/glfw3.h"
#include "../vendor/glm/glm/glm.hpp"
#include "../vendor/glm/glm/gtc/matrix_transform.hpp"
#include "../vendor/glm/glm/gtc/type_ptr.hpp"

// getMousePositionNDC returns the mouse position within the window
// in normalised device coordinates (NDC) With values in the range [-1, 1].
glm::vec2 getMousePositionNDC(GLFWwindow *window);

// getMousePositionFrameBuffer returns the mouse position in frame buffer dimensions.
glm::vec2 getMousePositionFrameBuffer(GLFWwindow *window);

// frameBufferPosToNDC converts a 2d position vector from frame buffer co-ordinates to NDC
glm::vec2 frameBufferPosToNDC(glm::vec2 input);