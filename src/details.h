#include "main.h"
#include "numbers.h"

#ifndef DETAILS_H
#define DETAILS_H

class Details {
 public:
  Details() {}
  Details(bool t);

  int score;
  float life;
  float boost;
  glm::mat4 VP;

  Numbers numbers;

  void draw_score();
  void draw_rest();
  void draw(glm::mat4 VP, glm::vec3 target, glm::vec3 eye, float angle, int cam_type);
 private:
  VAO *red_cube;
  VAO *green_cube;
  VAO *black_cube;
};

#endif
