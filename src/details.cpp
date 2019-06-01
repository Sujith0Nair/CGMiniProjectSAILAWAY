#include "main.h"
#include "details.h"
#include "numbers.h"

Details::Details(bool t) {
  this->score = 0;
  this->life = 10;
  this->boost = 12;

  this->numbers = Numbers(t);
  this->red_cube = create3DObject(GL_TRIANGLES, ball.num_sides,
				 ball.body, COLOR_RED, GL_FILL);
  this->green_cube = create3DObject(GL_TRIANGLES, ball.num_sides,
				    ball.body, (color_t) {200,200,100}, GL_FILL);
}

void Details::draw(glm::mat4 VP, glm::vec3 target, glm::vec3 eye, float angle, int cam_type) {
  glm::mat4 translate = glm::translate(glm::vec3(target.x, 3, target.z));
  glm::mat4 scale = glm::scale(glm::vec3(this->life/4, 0.2, 0.2));
  //  glm::mat4 MVP = VP * translate * scale;

  float d = -10, x = 1;
  switch(cam_type) {
  case 1: d = 0;x = -1.3;
    break;
  case 2: d = -10;
    break;
  case 3: d = -10;
    break;
  case 0: d = -25; x = -2;
    break;
  }
  glm::mat4 t2 = glm::translate(glm::vec3(x, 0, d));
  glm::mat4 view =  glm::lookAt( eye, target,glm::vec3(0,1,0) );
  glm::mat4 projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 500.0f);


  glm::vec3 dir = glm::normalize(target - eye);
  //  dir = glm::rotate(acos(dir.z)) * glm::vec4(0,0,1,0);
  glm::vec3 cross = glm::cross(dir, glm::vec3(0,0,1));
  glm::mat4 MVP = VP * translate * glm::rotate(angle, glm::vec3(0,1,0)) * scale * t2;

  if( !(cam_type == 2 || cam_type == 3 || cam_type == 4)) {
  glEnable(GL_BLEND);
  glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
  glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
  glUniform1f(Matrices.Transparency, 0.5);

  draw3DObject(this->red_cube);

  t2 = glm::translate(glm::vec3(x, -4, d));
  scale = glm::scale(glm::vec3(this->boost/4, 0.2, 0.2));
  MVP = VP * translate * glm::rotate(angle, glm::vec3(0,1,0)) * scale * t2;
  glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
  glUniform1f(Matrices.Transparency, 0.5);

  draw3DObject(this->green_cube);
  
  glDisable(GL_BLEND);
  }

  glm::mat4 rotate = glm::rotate((float) M_PI + angle, glm::vec3(0,1,0));
  scale = glm::scale(glm::vec3(0.4, 0.2, 0.5));
  glm::vec4 t_vec = rotate * glm::vec4(4,0,0,0);
  translate = glm::translate(target + glm::vec3(2.3 * t_vec.x, 4 - target.y, 2.3 * t_vec.z));

  if( cam_type == 0 ){
    //    scale = glm::scale(glm::vec3(0.3, 0.2, 0.5));
    VP *=  translate *rotate;
    this->numbers.draw(VP,glm::vec3(0,0,0), 1.0, this->score);
  }
  else if( cam_type == 1 ){
    translate = glm::translate(target + glm::vec3(1.2 * t_vec.x, 0,1.2 *  t_vec.z));
    VP *= translate * rotate* scale;
    this->numbers.draw(VP, glm::vec3(0,0,0), 1.0, this->score);
  }
}
  
