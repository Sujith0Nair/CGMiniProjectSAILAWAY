#include "main.h"

#ifndef NUM_H
#define NUM_H

class Numbers {
 public:
  Numbers() {};

  Numbers(bool a);
  void pixel(glm::mat4 VP, glm::vec3 pos, float scale);
  void draw(glm::mat4 VP, glm::vec3 pos, float scale, int num);
 private:
  VAO* object;
};

#endif
