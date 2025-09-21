#include "../vendor/glad/gl.h"
#include "../vendor/glfw/include/GLFW/glfw3.h"
#include "drawable.h"
#include <memory>
#include <vector>

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

  // Terminates the engine and window
  void terminate();

private:
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

  // processKeyboardInput processes keyboard input from the window on each render loop
  int processKeyboardInput();

  // processMouseInput processes mouse input from the window on each render loop
  void processMouseInput();

  // clearScreen clears the screen
  void clearScreen();

  // render renders the nodes in the engine's scene graph
  void render();

  // recordMetrics records metrics on each iteration of the render loop.
  void recordMetrics();
};