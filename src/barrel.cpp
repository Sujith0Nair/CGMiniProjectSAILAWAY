#include "main.h"
#include "barrel.h"

Barrel::Barrel(glm::vec3 init_pos, bool t) {
  this->pos = init_pos;
  this->life = t;
  
  this->barrel =  create3DObject(GL_TRIANGLES, ball.num_sides,
				 ball.body, COLOR_BROWN, GL_FILL);
  if( !this->life )
    this->collectible = create3DObject(GL_TRIANGLES, ball.num_sides,
			       ball.body, COLOR_GREEN, GL_FILL);
  else
    this->collectible = create3DObject(GL_TRIANGLES, ball.num_sides,
			       ball.body, COLOR_RED, GL_FILL);
    
  this->collected = false;
  this->angle = 0;
}

void Barrel::draw(glm::mat4 VP) {
  glm::mat4 translate = glm::translate(glm::vec3(this->pos.x, -5 + 0.5 * sin(this->angle), this->pos.z));
  glm::mat4 barrel_scale = glm::scale(glm::vec3(5, 3 ,3));
  glm::mat4 collectible_scale = glm::scale(glm::vec3(0.5, 0.5, 0.5));

  this->angle += 0.02;
  if( this->angle > M_PI)
    this->angle -= M_PI;

  glm::mat4 MVP = VP * translate * barrel_scale;

  glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
  glUniform1f(Matrices.Transparency, 1.0);

  draw3DObject(this->barrel);
  
}

void Barrel::draw_collectible(glm::mat4 VP) {
  glm::mat4 barrel_scale = glm::scale(glm::vec3(4, 1,1));
  glm::mat4 collectible_scale = glm::scale(glm::vec3(0.5, 0.5, 0.5));
  glm::mat4 translate = glm::translate(glm::vec3(this->pos.x, 1.5 + 0.6 * sin(this->angle), this->pos.z));
  glm::mat4 rotate = glm::rotate(this->angle, glm::vec3(1,1,1));
  glm::mat4 MVP = VP * translate * rotate * collectible_scale;
  glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
  glUniform1f(Matrices.Transparency, 0.7);

  glEnable(GL_BLEND);
  glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

  if( !this->collected )
    draw3DObject(this->collectible);

  glDisable(GL_BLEND);
}
