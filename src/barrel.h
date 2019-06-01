#include "main.h"

#ifndef BARREL_H
#define BARREL_H

class Barrel {
 public:
  Barrel() {};
  Barrel(glm::vec3 init_pos, bool t);

  glm::vec3 pos;
  float angle;

  void draw(glm::mat4 VP);
  void draw_collectible(glm::mat4 VP);
  bool collected;
  bool life;
 private:
  VAO *barrel;
  VAO *collectible;
};

#endif
