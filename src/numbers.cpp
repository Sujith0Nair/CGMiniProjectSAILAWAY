#include "numbers.h"
#include "main.h"

Numbers::Numbers(bool a) {
  static const GLfloat t1[] = {
    0.5, 0.5, 0,
    -0.5, -0.5, 0,
    0.5, -0.5, 0,

    0.5, 0.5, 0,
    -0.5, -0.5, 0,
    -0.5, 0.5, 0,
  };
  this->object = create3DObject(GL_TRIANGLES, 6, t1, (color_t) {0, 255, 255}, GL_FILL);
}

void Numbers::pixel(glm::mat4 VP, glm::vec3 pos, float scale) {
  glm::mat4 s;
  for(int i=0; i<4; ++i)
    for(int j=0; j<4; ++j)
      s[i][j] = 0;
  for(int i=0; i<3; ++i)
    s[i][i] = 1;
  s[3][3] = 1.0f;

  Matrices.model = glm::mat4(1.0f);
  glm::mat4 rotate    = glm::rotate((float) (0 * M_PI / 180.0f), glm::vec3(0, 0, 1));
  glm::mat4 translate = glm::translate (pos);    // glTranslatef
  rotate = rotate * glm::translate(glm::vec3(0, 0, 0));
  Matrices.model *= (translate * rotate * s);
  glm::mat4 MVP = VP * Matrices.model;
  glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
  glUniform1f(Matrices.Transparency, 0.3);
  glEnable(GL_BLEND);
  glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

  draw3DObject(this->object);

  glDisable(GL_BLEND);
}

void Numbers::draw(glm::mat4 VP, glm::vec3 posi, float scale, int num) {
  glm::vec4 pos = glm::vec4(posi, 0);
  glm::vec4 p[15];
  glm::mat4 s;
  for(int i=0; i<4; ++i)
    for(int j=0; j<4; ++j)
      s[i][j] = 0;
  for(int i=0; i<3; ++i)
    s[i][i] = 1;
  s[3][3] = 1.0f;

  p[0] = pos + glm::vec4(-1, 1.5, 0,1);
  p[1] = pos + glm::vec4(0, 1.5, 0,1);
  p[2] = pos + glm::vec4(1, 1.5, 0, 1);

  p[3] = pos + glm::vec4(-1, 0.5, 0, 1);
  p[4] = pos + glm::vec4(0, 0.5, 0, 1);
  p[5] = pos + glm::vec4(1, 0.5, 0, 1);

  p[6] = pos + glm::vec4(-1, -0.5, 0,1);
  p[7] = pos + glm::vec4(0, -0.5, 0, 1);
  p[8] = pos + glm::vec4(1, -0.5, 0, 1);

  p[9] = pos + glm::vec4(-1, -1.5, 0, 1);
  p[10] = pos + glm::vec4(0, -1.5, 0, 1);
  p[11] = pos + glm::vec4(1, -1.5, 0, 1);

  p[12] = pos + glm::vec4(-1, -2.5, 0, 1);
  p[13] = pos + glm::vec4(0, -2.5, 0, 1);
  p[14] = pos + glm::vec4(1, -2.5, 0, 1);

  bool n[10][15] = {
    1,1,1, 1,0,1, 1,0,1, 1,0,1, 1,1,1,
    0,0,1, 0,0,1, 0,0,1, 0,0,1, 0,0,1,
    1,1,1, 0,0,1, 1,1,1, 1,0,0, 1,1,1,

    1,1,1, 0,0,1, 1,1,1, 0,0,1, 1,1,1,
    1,0,1, 1,0,1, 1,1,1, 0,0,1, 0,0,1,
    1,1,1, 1,0,0, 1,1,1, 0,0,1, 1,1,1,

    1,1,1, 1,0,0, 1,1,1, 1,0,1, 1,1,1,
    1,1,1, 0,0,1, 0,0,1, 0,0,1, 0,0,1,
    1,1,1, 1,0,1, 1,1,1, 1,0,1, 1,1,1,

    1,1,1, 1,0,1, 1,1,1, 0,0,1, 1,1,1,
  };
  
  int a = num % 10;
  num = num/10;
  int g = 2, d[2];

  d[0] = a;
  d[1] = num%10;
  
  for(int j=0; j<2; ++j)
    for(int i=0; i<15; ++i )
      if( n[d[j]][i] )
	this->pixel(VP, p[i] - glm::vec4(-2 + j*4, 0,0,0), scale);
}
    

    
