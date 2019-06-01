#include "boat.h"
#include "main.h"

Boat::Boat(float x, float y, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->bounce_angle = 0;
    this->speed = 0;
    this->face_angle = 0;
    this->sail_angle = M_PI/5;
    this->canon_angle = 0;
    this->jump_y = 0;
    this->jump_angle = M_PI /4;
    this->jumping = false;
    this->top_speed = 0.1;

    this->back = create3DObject(GL_TRIANGLES, model_boat_back.num_sides,
				model_boat_back.body, COLOR_BLACK, GL_FILL);
    this->left = create3DObject(GL_TRIANGLES, model_boat_left_side.num_sides,
				model_boat_left_side.body, COLOR_BLACK, GL_FILL);
    this->right = create3DObject(GL_TRIANGLES, model_boat_right_side.num_sides,
				model_boat_right_side.body, COLOR_BLACK, GL_FILL);
    this->front = create3DObject(GL_TRIANGLES, model_boat_front.num_sides,
				 model_boat_front.body, COLOR_RED, GL_FILL);
    this->floor = create3DObject(GL_TRIANGLES, model_boat_floor.num_sides,
				 model_boat_floor.body, COLOR_BACKGROUND, GL_FILL);
    this->sail = create3DObject(GL_TRIANGLES, model_boat_sail.num_sides,
				model_boat_sail.body, COLOR_BLACK, GL_FILL);
    this->stand = create3DObject(GL_TRIANGLES, ball.num_sides,
				 ball.body, COLOR_BLACK, GL_FILL);

    this->canon = create3DObject(GL_TRIANGLES, ball.num_sides,
				 ball.body, COLOR_GREEN, GL_FILL);

    this->next = 0;
    for(int i=0; i<20; ++i) {
      this->bullet[i] = create3DObject(GL_TRIANGLES, ball.num_sides,
				       ball.body, COLOR_YELLOW, GL_FILL);
      this->bullet_dir[i] = glm::vec3(0,0,1);
      this->bullet_coors[i] = glm::vec3(100, 0, 0) + this->position;
      this->active[i] = false;
    }
}

void Boat::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    float rotation_update = 0.1/M_PI;

    if( this->jumping ) {
      jump_y = (sin(this->jump_angle) - sin(M_PI/4)) * 10;
      jump_angle += 0.01;
    }
    if( jump_angle > 3 * M_PI/4 ) {
      jump_angle = M_PI/4;
      jumping = false;
    }
    
    this->position.y = ((this->speed*2 + 0.07) * sin(this->bounce_angle) * 3)/2 - 0.1 + jump_y;
    this->bounce_angle += rotation_update;
    
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) ((this->speed + 0.05) * cos(this->bounce_angle))/2, glm::vec3(0, 0, 1));
    rotate *= glm::rotate((float) ((this->speed + 0.01) * sin(this->bounce_angle))/2, glm::vec3(1, 0, 0));
    rotate *= glm::rotate(this->face_angle, glm::vec3(0,1,0));

    glm::mat4 scale = glm::scale(glm::vec3(1.0, 1.0, 2.0));
    
    //    Matrices.model *= (translate * rotate * scale);
    glm::mat4 MVP = VP * Matrices.model * translate * rotate * scale;
    float transparency = 1.0;

    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    glUniform1f(Matrices.Transparency, transparency);

    draw3DObject(this->back);
    draw3DObject(this->left);
    draw3DObject(this->right);
    draw3DObject(this->front);
    draw3DObject(this->floor);


    scale = glm::scale(glm::vec3(0.2, 6, 0.2));
    glm::mat4 translate2 = glm::translate(glm::vec3(0,6,0));
    MVP = VP * translate * rotate * translate2 * scale;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    glUniform1f(Matrices.Transparency, transparency);

    draw3DObject(this->stand);

    glm::mat4 canon_scale = glm::scale(glm::vec3(0.5, 0.5, 3));
    glm::mat4 canon_translate = glm::translate(glm::vec3(0,2,4));
    glm::mat4 canon_rotate = glm::rotate(-this->canon_angle, glm::vec3(1,0,0));

    MVP = VP * translate * rotate *  canon_translate * canon_rotate * canon_scale;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    glUniform1f(Matrices.Transparency, transparency);

    draw3DObject(this->canon);

    scale = glm::scale(glm::vec3(0.5, 0.5, 0.5));
    for(int i=0; i<20; ++i) {
      if( true ) {
	rotate = glm::rotate(this->face_angle, glm::vec3(0,1,0));
	bullet_coors[i] += glm::vec3(bullet_dir[i].x/5, bullet_dir[i].y/5, bullet_dir[i].z/5);
	translate = glm::translate(bullet_coors[i]);
	MVP = VP * translate * scale * rotate;

	glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
	glUniform1f(Matrices.Transparency, transparency);

	draw3DObject(this->bullet[i]);
      }
    }
    

}

void Boat::draw_sail(glm::mat4 VP) {
  glm::mat4 translate = glm::translate (this->position);    // glTranslatef
  glm::mat4 rotate_turn = glm::rotate(this->sail_angle - this->face_angle, glm::vec3(0,-1,0));
  glm::mat4 scale = glm::scale(glm::vec3(this->sail_angle * 10, 1.0, 2.0));
  glm::mat4 rotate    = glm::rotate((float) ((this->speed + 0.05) * cos(this->bounce_angle))/2, glm::vec3(0, 0, 1));
  rotate *= glm::rotate((float) ((this->speed + 0.01) * sin(this->bounce_angle))/2, glm::vec3(1, 0, 0));

  glm::mat4 MVP = VP * Matrices.model * translate * rotate * rotate_turn * scale;
  glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
  glUniform1f(Matrices.Transparency, 0.5);
  
  glEnable(GL_BLEND);
  glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
  draw3DObject(this->sail);
  
  glDisable(GL_BLEND);
}

void Boat::tick() {
  this->position.z += this->speed * cos(this->face_angle);
  this->position.x += this->speed * sin(this->face_angle);
}

void Boat::accelerate() {
  if( this->speed < this->top_speed )
    this->speed += 0.001;
  if( this->speed > this->top_speed )
    this->speed = this->top_speed;
}

void Boat::decelerate() {
  if( this->speed > 0.001 )
    this->speed -= 0.001;
  else
    this->speed = 0;
}

void Boat::turn_left() {
  if( this->sail_angle > -M_PI/5)
    this->sail_angle -= 0.04;
  this->face_angle += 0.01;
  if( this->face_angle > 2 * M_PI )
    this->face_angle = 0;
}

void Boat::turn_right() {
  if( this->sail_angle < M_PI/5)
    this->sail_angle += 0.04;
  this->face_angle -= 0.01;
  if( this->face_angle < -2 * M_PI )
    this->face_angle = 0;    
}

void Boat::canon_rise(bool up) {
  if( up && this->canon_angle < M_PI/7 )
    this->canon_angle += 0.01;
  else if( !up && this->canon_angle > 0 )
    this->canon_angle -= 0.01;
}

void Boat::canon_fire() {
  int i = this->next;
  glm::vec4 dir = glm::rotate(this->face_angle, glm::vec3(0,1,0)) * glm::vec4(0,0,1,0);
  
  this->bullet_coors[i] = this->position + glm::vec3(dir.x, dir.y, dir.z);

  dir = glm::rotate(this->face_angle, glm::vec3(0,1,0)) * glm::rotate(this->canon_angle, glm::vec3(-1,0,0)) * glm::vec4(0,0,6,0);

  this->bullet_coors[i] += glm::vec3(dir.x, dir.y + 0.7, dir.z);
  this->bullet_dir[i] = glm::normalize(glm::vec3(dir.x, dir.y, dir.z));

  this->active[i] = true;
  this->next = (this->next + 1)%20;
  std::cout<<this->next<<"\n";
}
  
void Boat::jump() {
  this->jumping = true;
}
      
