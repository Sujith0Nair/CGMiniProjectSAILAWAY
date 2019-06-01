#include "main.h"

#ifndef ROCK_H
#define ROCK_H

class Rock {
 public:
  Rock() {};
  Rock(glm::vec3 init_pos);

  glm::vec3 pos;
  float y;
  void draw(glm::mat4 VP);
 private:
  VAO *rock;
  VAO *under;
};

#endif
