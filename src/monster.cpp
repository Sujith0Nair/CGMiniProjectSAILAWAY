#include "monster.h"
#include "main.h"

Monster::Monster(glm::vec3 init_pos, int type) {
  this->pos = init_pos;
  this->tier = type;
  this->right = true;
  this->bullet_pos = glm::vec3(-10, -10, -10);
  
  this->body = create3DObject(GL_TRIANGLES, ball.num_sides,
			      ball.body, (color_t) {200, 100, 100}, GL_FILL);
  this->face = create3DObject(GL_TRIANGLES, ball.num_sides,
			       ball.body, COLOR_GREEN, GL_FILL);
  this->gun = create3DObject(GL_TRIANGLES, ball.num_sides,
			       ball.body, COLOR_BLACK, GL_FILL);
  this->bullet = create3DObject(GL_TRIANGLES, ball.num_sides,
				ball.body, (color_t) {200,200,200}, GL_FILL);
  this->alive = true;
}

void Monster::shoot_target(glm::vec3 pos) {
}

void Monster::tick(glm::vec3 boat_pos) {

  if( this->tier %2 == 0 )
    this->pos.y = 4 + 4 * sin(this->gun_angle);
  if( this->pos.x > 30 ) {
    this->right = false;
    this->bullet_pos = this->pos;
    this->bullet_dir = glm::vec3(boat_pos.x, 1, boat_pos.z) - this->pos;
  }
  if (this->pos.x < -30) {
    this->right = true;
    this->bullet_pos = this->pos;
    this->bullet_dir = glm::vec3(boat_pos.x, 1, boat_pos.z) - this->pos;
  }
  if( this->right)
    this->pos.x += 0.2;
  else
    this->pos.x -= 0.2;

  this->bullet_pos += glm::normalize(this->bullet_dir);
  this->bullet_pos -= 0.05;

  this->gun_angle += 0.1/M_PI;
}

void Monster::bullet_tick() {
  
}

void Monster::draw(glm::mat4 VP) {

  glm::mat4 translate = glm::translate(glm::vec3(this->pos));
  glm::mat4 face_translate = glm::translate(glm::vec3(0,3,0));
  glm::mat4 gun_translate = glm::translate(glm::vec3(3, 0, 2 - 1.4 * sin(this->gun_angle)));
  glm::mat4 rotate = glm::rotate((float)M_PI, glm::vec3(0,1,0));
  glm::mat4 gun_scale = glm::scale(glm::vec3(1,1,4));

  glm::mat4 MVP = VP * translate * rotate * glm::scale(glm::vec3(2,2,2));
  
  glm::mat4 t2 = glm::translate(glm::vec3(0,6,0));;
  if( this->tier == 100 ) {
    rotate = rotate * glm::scale(glm::vec3(3,3,3));
    MVP *= glm::scale(glm::vec3(3,3,3));
  }
  
  glm::mat4 bullet_translate = glm::translate(this->bullet_pos);
  if( this->alive) {
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    glUniform1f(Matrices.Transparency, 1.0);
  
    draw3DObject(this->body);

    if( this->tier == 100 )
          MVP = VP * translate * face_translate * t2 * rotate;
    else 
        MVP = VP * translate * face_translate * rotate;

    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    glUniform1f(Matrices.Transparency, 1.0);

    draw3DObject(this->face);

    MVP = VP * translate * rotate * gun_translate * gun_scale;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    glUniform1f(Matrices.Transparency, 1.0);

    draw3DObject(this->gun);

    gun_scale = glm::scale(glm::vec3(1,2,1));
    gun_translate = glm::translate(glm::vec3(-3, -1, 2));

    MVP = VP * translate * rotate * gun_translate * gun_scale;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    glUniform1f(Matrices.Transparency, 1.0);

    draw3DObject(this->gun);
  
    MVP = VP * bullet_translate;

    if (this->tier == 100 )
      MVP *= glm::scale(glm::vec3(3,3,3));
    
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    glUniform1f(Matrices.Transparency, 1.0);

    if( this->bullet_pos.y >= -1 )
      draw3DObject(this->bullet);
  }
}

  

 
