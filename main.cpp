#include "src/drawable.h"
#include "src/engine.h"
#include "src/utils.h"
#include <cstdint>
#include <iostream>
#include <memory>
#include <vector>

int main() {
  // Setup the engine
  Engine engine(800, 600, "Drawww");

  // Run the engine until the user closes the window
  engine.run();

  // Exit the engine and cleanup resources
  engine.terminate();

  return 0;
}
