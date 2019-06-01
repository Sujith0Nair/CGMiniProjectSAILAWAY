#include "rock.h"
#include "main.h"

Rock::Rock(glm::vec3 init_pos) {
  this->rock = create3DObject(GL_TRIANGLES, ball.num_sides,
				 ball.body, COLOR_BLACK, GL_FILL);
  this->under = create3DObject(GL_TRIANGLES, model_rock.num_sides,
				 model_rock.body, COLOR_BLACK, GL_FILL);

  this->pos = init_pos;
  this->y = (rand() %6) * 2  + 9;
}

void Rock::draw(glm::mat4 VP) {
  glm::mat4 translate = glm::translate(glm::vec3(this->pos.x, -2, this->pos.z));
  glm::mat4 scale = glm::scale(glm::vec3(3,this->y,3));
  
  glm::mat4 MVP = VP * translate * scale;
  glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
  glUniform1f(Matrices.Transparency, 1.0);

  draw3DObject(this->under);
  draw3DObject(this->rock);

}

