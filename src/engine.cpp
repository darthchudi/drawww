#include "engine.h"
#include "drawable.h"
#include "point.h"
#include "ray.h"
#include "utils.h"
#include <__config>
#include <cmath>
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <memory>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

// framebuffer_size_callback is callback handler that is called each time the
// GLFW window is resized
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

// Initialises the engine
Engine::Engine(int width, int height, const char *title) {
  this->createWindow(width, height, title);
  this->initOpenGL();
}

// Destructor to clean up heap-allocated objects
Engine::~Engine() { this->nodes.clear(); }

// run runs the engine render loop
void Engine::run() {
  while (this->isRunning()) {
    // Record metrics at the start of each frame
    this->recordMetrics();

    // Process input within the engine
    this->processInput();

    // Clear the screen
    this->clearScreen();

    // Draw the objects
    this->render();

    // Swap buffers to render the draw calls
    glfwSwapBuffers(this->window);

    // Poll for events i.e process all pending OpenGL events
    glfwPollEvents();
  }
}

// createWindow creates a window for the engine
void Engine::createWindow(int width, int height, const char *title) {
  // Setup the GLFW library
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  // Create the GLFW window
  GLFWwindow *_window = glfwCreateWindow(width, height, title, NULL, NULL);
  if (_window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();

    throw std::runtime_error("failed to create window");
  }

  this->window = _window;
  this->title = title;

  glfwMakeContextCurrent(this->window);
}

// initOpenGL intiialises OpenGL
void Engine::initOpenGL() {
  // Load the OpenGl function points with GLAD
  if (!gladLoadGL(glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;

    throw std::runtime_error("failed to load OpenGL functions");
  }

  // Set the OpenGL viewport
  int frameBufferWidth, frameBufferHeight;
  glfwGetFramebufferSize(this->window, &frameBufferWidth, &frameBufferHeight);
  glViewport(0, 0, frameBufferWidth, frameBufferHeight);

  // Register callback
  glfwSetFramebufferSizeCallback(this->window, framebuffer_size_callback);
}

// isRunning indicates if the engine is running
bool Engine::isRunning() { return !glfwWindowShouldClose(window); }

// processInput processes input from the window on each render loop
void Engine::processInput() {
  int keyboardExitCode = processKeyboardInput();
  if (keyboardExitCode == -1)
    return;

  processMouseInput();
}

// processKeyboardInput processes keyboard input from the window on each render
// loop
int Engine::processKeyboardInput() {
  if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(this->window, true);
    utils::print("👋🏽 Closing");

    return -1;
  }

  return 0;
}

// processMouseInput processes mouse input from the window on each render loop
void Engine::processMouseInput() {
  // Add a new point if the mouse is currently pressed
  int leftMouseButtonState =
      glfwGetMouseButton(this->window, GLFW_MOUSE_BUTTON_LEFT);
  if (leftMouseButtonState != GLFW_PRESS)
    return;

  glm::vec2 mousePositionNDC = getMousePositionNDC(this->window);

  std::unique_ptr<Point> point(
      new Point(mousePositionNDC.x, mousePositionNDC.y));

  this->add(std::move(point));
}

// clearScreen clears the screen
void Engine::clearScreen() {
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
}

// add adds a drawable item to the engine's scene graph
void Engine::add(std::unique_ptr<Drawable> node) {
  this->nodes.push_back(std::move(node));
}

// render renders the nodes in the engine's scene graph
// Todo: Optimise how points are drawn, so we use a single draw call per frame.
void Engine::render() {
  for (std::unique_ptr<Drawable> &node : this->nodes) {
    node->draw();
  }
}

// Terminates the window and engine
void Engine::terminate() { glfwTerminate(); }

// recordMetrics records metrics (e.g FPS) on each iteration of the render loop.
void Engine::recordMetrics() {
  double now = glfwGetTime();

  if (this->lastCheckpointTime == 0.0) {
    this->lastCheckpointTime = now;
    return;
  }

  this->numFrames += 1;

  // Update the FPS metric if it's been more than 1 second since
  // the last time the metric was recorded
  double elapsedTime = now - this->lastCheckpointTime;
  if (elapsedTime < 1.0) {
    return;
  }

  // This assumes that elapsed time is either 1 second or close to that
  double milliSecondsPerFrame = (elapsedTime * 1000) / double(numFrames);
  double framesPerSecond = double(this->numFrames) / elapsedTime;
  int fps = int(std::floor(framesPerSecond));

  // Build the window title containing the metrics
  std::stringstream windowTitle;
  windowTitle.setf(std::ios::fixed);
  windowTitle << this->title << " @ " << fps << " FPS — "
              << std::setprecision(3) << milliSecondsPerFrame << "ms/frame";

  glfwSetWindowTitle(window, windowTitle.str().c_str());

  // Print metrics to stdout
  // printf("%i FPS - %.3f ms/frame\n", fps, milliSecondsPerFrame);

  // Reset the metrics for the next second
  this->numFrames = 0;
  this->lastCheckpointTime = now;
}