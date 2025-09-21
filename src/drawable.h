#ifndef DRAWABLE_H
#define DRAWABLE_H
#include "../vendor/glad/gl.h"
#include "../vendor/glfw/include/GLFW/glfw3.h"

// Drawable represents an object which can be rendered on a screen.
// A drawable object will typically be contained within a scene
class Drawable {
public:
  // Drawable constructor which setups the object and loads resources
  // such as shaders
  Drawable() = default;

  // Drawable destuctor which cleans up resources used by the drawable object
  virtual ~Drawable() = default;

  // draws renders data to the screen
  virtual void draw() = 0;
};

#endif // DRAWABLE_H