#include "point.h"
#include <exception>
#include <stdexcept>

// Initialise the mouse click shader program
Point::Point(float x, float y)
    : shader("../src/shaders/point/point.vert",
             "../src/shaders/point/point.frag") {
  // Setup the vertex buffers
  glGenVertexArrays(1, &(this->VAO));
  glGenBuffers(1, &(this->VBO));

  // Bind VAO and VBO
  glBindVertexArray(this->VAO);
  glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

  // Create a single vertex at (x, y) for the point
  float vertexData[] = {
      x,
      y,
  };
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

  // Set vertex attribute pointers
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // Unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  this->isInitialised = true;
}

// Cleanup
Point::~Point() {
  if (!this->isInitialised) {
    return;
  }

  // Cleanup the allocated objects
  glDeleteVertexArrays(1, &(this->VAO));
  glDeleteBuffers(1, &(this->VBO));
  glDeleteProgram(this->shader.ID);
}

// draws to screen
void Point::draw() {
  if (!this->isInitialised)
    throw std::runtime_error("point shader not not initialised");

  // Active the shader program
  this->shader.use();

  // Enable point rendering
  glEnable(GL_PROGRAM_POINT_SIZE);

  // draw a single point
  glBindVertexArray(this->VAO);

  glDrawArrays(GL_POINTS, 0, 1);
}