#include "ray.h"

// getMousePositionNDC returns the mouse position within the window
// in normalised device coordinates (NDC) With values in the range [-1, 1].
glm::vec2 getMousePositionNDC(GLFWwindow *window) {
  // Get the mouse position in the window
  double mouseXWindow, mouseYWindow;
  glfwGetCursorPos(window, &mouseXWindow, &mouseYWindow);

  // Handle HiDPI: scale screen coords to framebuffer pixels
  int windowWidth, windowHeight, frameBufferWidth, frameBufferHeight;

  glfwGetWindowSize(window, &windowWidth, &windowHeight);
  glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);

  double scalingFactorX = windowWidth ? (frameBufferWidth / windowWidth) : 1;
  double scalingFactorY = windowHeight ? (frameBufferHeight / windowHeight) : 1;

  // Scale the mouse coordinates based on the frame buffer size
  double mouseX = mouseXWindow * scalingFactorX;
  double mouseY = mouseYWindow * scalingFactorY;

  // Convert the mouse coords from screen space to (NDC).
  // We use the viewport for this, as it uses the framebuffer size
  int viewport[4]; // x, y, w, h
  glGetIntegerv(GL_VIEWPORT, viewport);

  float x_ndc = (2.0f * ((mouseX - viewport[0]) / viewport[2])) - 1.0f;
  float y_ndc = 1.0f - (2.0f * ((mouseY - viewport[1]) / viewport[3]));

  return glm::vec2(x_ndc, y_ndc);
}