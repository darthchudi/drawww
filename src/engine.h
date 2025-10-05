#include "../vendor/glad/gl.h"
#include "../vendor/glfw/include/GLFW/glfw3.h"
#include "drawable.h"
#include <memory>
#include <vector>
#include "../vendor/glm/glm/glm.hpp"

// Engine is a rendering engine which uses a given graphics library (OpenGL by
// default) to render graphics to the screen. The Engine primarily manages the
// state of the main loop and the app window
class Engine {
public:
  // Initialises the engine
  Engine(int width, int height, const char *title);

  // Destructor to clean up heap-allocated objects
  ~Engine();

  // add adds a drawable item to the engine's scene graph
  void add(std::unique_ptr<Drawable> node);

  // run runs the engine render loop
  void run();

  // tick is a single render pass used to draw on the screen.
  void tick();

  // Terminates the engine and window
  void terminate();

  // setRenderContext sets the context in which the engine is running in.
  // This could either be web or native
  void setRenderContext();

private:
  const char * context;

  /**
    Observability-related fields
  */
  double lastCheckpointTime;
  int numFrames;

  /**
    Engine metadata
   */
  const char *title;

  /**
    Core engine fields and methods
  */
  // The window used by the engine
  GLFWwindow *window;

  // The nodes which will be rendered in the sceene
  std::vector<std::unique_ptr<Drawable>> nodes;

  // createWindow creates a window for the engine
  void createWindow(int width, int height, const char *title);

  // initOpenGL intiialises OpenGL
  void initOpenGL();

  // isRunning indicates if the engine is running
  bool isRunning();

  // processInput processes input from the window on each render loop
  void processInput();

  // processKeyboardInput processes keyboard input from the window on each
  // render loop
  int processKeyboardInput();

  // processMouseInput processes mouse input from the window on each render loop
  void processMouseInput();

  // clearScreen clears the screen
  void clearScreen();

  // render renders the nodes in the engine's scene graph
  void render();

  // recordMetrics records metrics on each iteration of the render loop.
  void recordMetrics();

  // runNative runs the render loop on a native platform.
  void runNative();
};

/**
 Engine util functions
*/

// glfwFramebufferSizeCallback is callback handler that is called each time the GLFW
// window is resized
void glfwFramebufferSizeCallback(GLFWwindow *window, int width, int height);

// runWeb is a single-frame tick used for rendering to web platforms.
// The function is called per frame by the browser and yields back control after
// a single render pass.
static void runWeb(void *userData);

struct FrameAnalytics {
private:
  static glm::vec2 windowSize;
  static double dpi;
  static glm::vec2 frameBufferSize;
  static glm::vec2 viewportSize;
};