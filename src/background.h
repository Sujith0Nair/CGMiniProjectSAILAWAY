#include "main.h"

#ifndef BG_H
#define BG_H


class Background {
 public:
  Background() {};
  Background(bool t);

  void draw_sea(glm::mat4 VP, glm::vec3 pos, glm::vec3 eye);

 private:
  VAO *sea_object;
  VAO *shader;
  VAO *land;
};

#endif
