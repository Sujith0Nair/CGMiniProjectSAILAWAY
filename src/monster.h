#include "main.h"

#ifndef MONSTER_H
#define MONSTER_H

class Monster {
 public:
  Monster() {};
  Monster(glm::vec3 init_pos, int type);

  glm::vec3 pos;
  glm::vec3 bullet_pos;
  glm::vec3 bullet_dir;
  
  float gun_angle;
  int tier;
  bool right;
  bool alive;

  void shoot_target(glm::vec3 pos);
  void tick(glm::vec3 boat_pos);
  void draw(glm::mat4 VP);
  void bullet_tick();
  
 private:
  VAO *face;
  VAO *body;
  VAO *gun;
  VAO *bullet;
};

#endif

  
