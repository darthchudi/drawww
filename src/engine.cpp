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
#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#endif

const char *RENDER_CONTEXT_WEB = "web";
const char *RENDER_CONTEXT_NATIVE = "native";

// TODO: these could be defined in a separate file
#ifdef __EMSCRIPTEN__
/**
 * applyEmscriptenCanvasResize resizes the following items:
 * 1. It stretches the HTML canvas item so that it stretches the full dimensions of its container.
 * 2. It updates the GLFW window size, which implicitly updates the GLFW framebuffer size, so that it uses the new
 * canvas size.
 * 3. Finally, we update the viewport with the new updated framebuffer size.
 * @param window The GLFW window
 */
void applyEmscriptenCanvasResize(GLFWwindow * window) {
  // 1. Get canvas container size in CSS pixels and set HTML canvas element size to span its
  // container element dimensions in CSS pixels
  double canvasContainerWidth, canvasContainerHeight;

  emscripten_get_element_css_size("#canvas-container", &canvasContainerWidth, &canvasContainerHeight);
  emscripten_set_canvas_element_size("canvas", int(canvasContainerWidth), int(canvasContainerHeight));

  // 2. Set the GLFW window size based on the updated canvas pixel size
  // This will cause the frame buffer size to be recalculated.
  glfwSetWindowSize(window, int(canvasContainerWidth), int(canvasContainerHeight));

  // 3. Finally update the viewport with the updated frame buffer size
  int frameBufferWidth, frameBufferHeight;
  glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);

  glViewport(0, 0, frameBufferWidth, frameBufferHeight);
}

/**
 * emscriptenResizeCallback handles the HTML window being resized
 */
bool emscriptenResizeCallback(int eventType, const EmscriptenUiEvent *uiEvent, void *userData) {
  GLFWwindow *window = reinterpret_cast<GLFWwindow *>(userData);
  if (!window) return false;

  applyEmscriptenCanvasResize(window);

  return true;
}
#endif

// Initialises the engine
Engine::Engine(int width, int height, const char *title) {
  this->setRenderContext();
  this->createWindow(width, height, title);
  this->initOpenGL();
  this->registerCallbacks();
}

// Destructor to clean up heap-allocated objects
Engine::~Engine() { this->nodes.clear(); }

void Engine::setRenderContext() {
#ifdef __EMSCRIPTEN__
  this->context = RENDER_CONTEXT_WEB;
#else
  this->context = RENDER_CONTEXT_NATIVE;
#endif
}

void Engine::setDrawing(bool isDrawing) {
  this->_isDrawing = isDrawing;

  if (!isDrawing) {
    this->hasLastPoint = false;
  } else {
    printf("SET DRAWING!!!!\n");
  }
}

bool Engine::isDrawing() {
  return this->_isDrawing;
}

void engineMouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
  auto engine = reinterpret_cast<Engine *>(glfwGetWindowUserPointer(window));

  switch (action){
    case GLFW_PRESS:
      engine->setDrawing(true);
      break;
    case GLFW_RELEASE:
      engine->setDrawing(false);
      break;
  }

  printf("Is drawing: %s \n", engine->isDrawing() ? "true":"false");
}

void engineCursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {
  auto engine = reinterpret_cast<Engine *>(glfwGetWindowUserPointer(window));

  if (!engine->isDrawing()) {
    return;
  };

  glm::vec2 mousePositionNDC = getMousePositionNDC(window);

  // printf("Drawing at Window position: %f %f. NDC: %f %f\n", xpos, ypos, mousePositionNDC.x, mousePositionNDC.y);

  if (!engine->hasLastPoint) {
    engine->lastPoint = glm::vec2{mousePositionNDC.x, mousePositionNDC.y};
    engine->hasLastPoint = true;

    // Return early as there's nothing to do
    return;
  }

  // Compute the distance between the current position and the last point
  auto dx = mousePositionNDC.x - engine->lastPoint.x;
  auto dy = mousePositionNDC.y - engine->lastPoint.y;

  auto distance = std::sqrt(dx * dx + dy * dy);

  printf("Distanceee: x=%f y=%f distance=%f\n", dx, dy, distance);

  if (distance <= 0) {
    // Nothing to interpolate
    return;
  }

  float seperator = 1;

  int steps = std::ceil(distance / seperator);

  for (int i = 1; i <= steps; i++) {
  double t = double(i) / double(steps);

    auto new_x = engine->lastPoint.x + (dx * t);
    auto new_y = engine->lastPoint.y + (dy * t);

    // Add a new point at this position
    std::unique_ptr<Point> point(new Point(new_x, new_y));

    engine->add(std::move(point));
  }

  // Set this position as the last
  engine->lastPoint = glm::vec2{mousePositionNDC.x, mousePositionNDC.y};
}

void Engine::registerCallbacks() {
  glfwSetMouseButtonCallback(window, engineMouseButtonCallback);
  glfwSetCursorPosCallback(window, engineCursorPositionCallback);
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
  GLFWwindow *_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
  if (_window == nullptr) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();

    throw std::runtime_error("failed to create window");
  }

  // Store a reference to the window in the engine instance
  this->window = _window;
  this->title = title;

  // Set a reference to the drawww engine instance in GLFW.
  // This allows us to reference the engine instance later on in callbacks
  glfwSetWindowUserPointer(_window, this);

  // Make the created window the GLFW current context
  glfwMakeContextCurrent(this->window);
}

// initOpenGL intialises OpenGL
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

  // Register GLFW callback on native platforms
  glfwSetFramebufferSizeCallback(this->window, glfwFramebufferSizeCallback);

  // Register emscripten callbacks
#ifdef __EMSCRIPTEN__
  applyEmscriptenCanvasResize(this->window);
  emscripten_set_resize_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, this->window, false, emscriptenResizeCallback);
#endif
}

// glfwFramebufferSizeCallback is callback handler that is called each time the GLFW
// window is resized
void glfwFramebufferSizeCallback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

// run runs the engine render loop.
// If we are in a web context, we'll want to use the emscripten render loop,
// otherwise we use the engine's native render loop.
void Engine::run() {
#ifdef __EMSCRIPTEN__
  emscripten_set_main_loop_arg(runWeb, this, 0, true);
#else
  // We are in a native context, so run the render loop
  this->runNative();
#endif
}

// runWeb is a single-frame tick used for rendering to web platforms.
// The function is called per frame by the browser and yields back control after
// a single render pass.
static void runWeb(void *userData) {
  Engine *engine = reinterpret_cast<Engine *>(userData);
  if (engine == nullptr)
    return;

  engine->tick();
}

// runNative runs the render loop on a native platform.
// The loop runs until the end of the program.
void Engine::runNative() {
  while (this->isRunning()) {
    this->tick();
  }
}

// tick is a single render pass used to draw on the screen.
void Engine::tick() {
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
  // int leftMouseButtonState =
  //     glfwGetMouseButton(this->window, GLFW_MOUSE_BUTTON_LEFT);
  // if (leftMouseButtonState != GLFW_PRESS)
  //   return;
  //
  // glm::vec2 mousePositionNDC = getMousePositionNDC(this->window);
  //
  // std::unique_ptr<Point> point(
  //     new Point(mousePositionNDC.x, mousePositionNDC.y));
  //
  // this->add(std::move(point));
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

  // Build the window title containing the metrics.
  // We only update the window title with this information on non-web platforms
  std::stringstream windowTitle;
  windowTitle.setf(std::ios::fixed);
  windowTitle << this->title << " @ " << fps << " FPS — "
              << std::setprecision(3) << milliSecondsPerFrame << "ms/frame";

#ifndef  __EMSCRIPTEN__
  glfwSetWindowTitle(window, windowTitle.str().c_str());
#endif

  // Print metrics to stdout
  // printf("%i FPS - %.3f ms/frame\n", fps, milliSecondsPerFrame);

  // Reset the metrics for the next second
  this->numFrames = 0;
  this->lastCheckpointTime = now;
}
