#include "background.h"
#include "main.h"


Background::Background(bool t) {
  this->sea_object = create3DObject(GL_TRIANGLES, 6, sea.body, COLOR_BLUE, GL_FILL);
  this->shader = create3DObject(GL_TRIANGLES, 6, sea.body, sea.color, GL_FILL);
  color_t red = {100, 0, 100};
  this->land = create3DObject(GL_TRIANGLES, 6, sea.body, red, GL_FILL);
}

void Background::draw_sea(glm::mat4 VP, glm::vec3 pos, glm::vec3 eye) {
  float theta = acos(glm::length(glm::dot(glm::normalize(glm::vec3(eye.x,0,eye.z)), glm::normalize(glm::vec3(0,0,1)))));
  float sign = ((glm::cross(glm::normalize(glm::vec3(eye.x, 0, eye.z)), glm::vec3(0,0,1))).y);
  theta = -sign * theta;
  
  glm::mat4 translate2 = glm::translate (glm::vec3(pos.x, -15, pos.z));
  glm::mat4 rotate = glm::rotate(theta, glm::vec3(0,1,0));
  glm::mat4 scale = glm::scale(glm::vec3(4,4,4));
  glm::mat4 MVP2 = VP * translate2 *rotate* scale;
  glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP2[0][0]);
  glUniform1f(Matrices.Transparency, 0.7);
  glEnable(GL_BLEND);
  glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

  draw3DObject(this->shader);

  glDisable(GL_BLEND);

  translate2 = glm::translate (glm::vec3(pos.x, -8, pos.z));

  MVP2 = VP * translate2 *rotate* scale;
  glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP2[0][0]);
  glUniform1f(Matrices.Transparency, 0.5);
  glEnable(GL_BLEND);
  glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

  draw3DObject(this->shader);

  glDisable(GL_BLEND);

  glm::mat4 translate = glm::translate(glm::vec3(0, -0.3, 400));
  glm::mat4 MVP = VP * translate;

  glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
  glUniform1f(Matrices.Transparency, 1.0);

  //  draw3DObject(this->land);

  translate = glm::translate (glm::vec3(pos.x, 0, pos.z));    // glTranslatef
  MVP = VP * translate * rotate * scale;
  float transparency = 0.5;
  glEnable(GL_BLEND);
  glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
  glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
  glUniform1f(Matrices.Transparency, transparency);

  draw3DObject(this->sea_object);

  glDisable(GL_BLEND);


}
