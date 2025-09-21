#include "drawable.h"
#include "ray.h"
#include "shader.h"
#include <vector>

class Point : public Drawable {
public:
  Point(float x, float y);
  ~Point();

  // draws to screen
  virtual void draw();

private:
  // Shader internals
  Shader shader;
  unsigned int VBO, VAO;

  // Drawable state
  bool isInitialised;
};