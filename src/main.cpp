#include "main.h"
#include "timer.h"
#include "boat.h"
#include "background.h"
#include "monster.h"
#include "rock.h"
#include "barrel.h"
#include "map.h"
#include "engine.h"
#include "details.h"
#include <GL/glut.h>
#include <string.h>
#include <il.h>
#include "numbers.h"
using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

glm::vec3 eye (0,10,80);
glm::vec3 target (0, 8, 120);
glm::vec3 up (0, 1, 0);

Boat boat;
Background bg;
Monster monster[no_monsters], boss;
Rock rock[no_rocks];
Barrel barrel[no_barrels];

Details player;

float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;

int width = 1400;
int height = 900;
int cam_type = 0;
bool boost;
bool boss_killed = false;
bool rookies_killed = false;
int boss_hits = 200;
bool boss_score = false;

const int boat_view = 1, top_view = 2, tower_view = 3, follow_cam_view = 0, heli_cam_view = 4;

Timer t60(1.0 / 60);

bool on_land;

bool nextScreen;

void cursor_position(GLFWwindow *window, double xoffset, double yoffset) {
  glm::mat4 rotate_x = glm::rotate(((float)xoffset - 700)/10000, glm::vec3(0,1,0));
  glm::vec4 rot = rotate_x * glm::vec4(target - eye, 0);
  if( cam_type == heli_cam_view ) {
    target = eye + glm::vec3(rot.x, rot.y, rot.z);
    target.y = (yoffset - 450)/10;
  }
  std::cout<<xoffset<<"\n";
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
  glm::mat4 rotate_x = glm::rotate(((float)yoffset)/10.0f, glm::vec3(0,1,0));
  glm::vec4 rot = rotate_x * glm::vec4(target - eye, 0);
  
  if( cam_type == heli_cam_view ) {
    if( yoffset > 0 ) {
      target += glm::vec3(rot.x/100, 0, rot.z/100);
      eye+= glm::vec3(rot.x/100, 0, rot.z/100);
    }
    else {
     target -= glm::vec3(rot.x/100, 0, rot.z/100);
      eye -= glm::vec3(rot.x/100, 0, rot.z/100);
    } 
  }
}

void keyboard(GLFWwindow *window, int key, int scancode, int action, int mods) {
  // Function is called first on GLFW_PRESS.
  float top    = screen_center_y + 4 / screen_zoom;
  float bottom = screen_center_y - 4 / screen_zoom;
  float left   = screen_center_x - 4 / screen_zoom;
  float right  = screen_center_x + 4 / screen_zoom;

  if (action == GLFW_RELEASE) {
    switch (key) {
    case GLFW_KEY_B:
      boost = false;
      break;
    default:
      break;
    }
  } else if (action == GLFW_PRESS) {
    switch (key) {
    case GLFW_KEY_ESCAPE:
      quit(window);
      break;
    case GLFW_KEY_F:
      boat.canon_fire();
      break;
    case GLFW_KEY_C:
      cam_type = ++cam_type % 5;
      break;
    case GLFW_KEY_SPACE:
      boat.jump();
      break;
    case GLFW_KEY_P:
      screen_zoom = 1;
      Matrices.projection = glm::perspective(glm::radians(90.0f), 14.0f/9.0f, 1.0f, 300.0f);
      break;
    case GLFW_KEY_O:
      screen_zoom = 0.4;
      Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 100.0f);
      break;
    case GLFW_KEY_B:
      boost = true;
      break;
    default:
      break;
    }
  }
}

void cam(int cam_type) {
  glm::mat4 rotate;
  glm::vec4 dir;
  glm::vec4 cam_dir = glm::rotate(boat.face_angle, glm::vec3(0,1,0)) * glm::vec4(0,0,1,0);

  switch(cam_type) {
  case boat_view:
    rotate = glm::rotate(boat.face_angle, glm::vec3(0,1,0));
    dir = rotate * glm::vec4(0,0,4,0);

    eye = boat.position + glm::vec3(dir.x, dir.y, dir.z);
    target = eye + glm::vec3(dir.x, dir.y, dir.z);
    eye.y = target.y = 4;
    up = glm::vec3(0,1,0);
    break;
  case top_view:
    eye = boat.position + glm::vec3(0,40,0);
    target = boat.position + glm::vec3(0,2,0);
    up = glm::vec3(0,0,1);
    break;
  case tower_view:
    if( boat.position.z < 120)
      eye = glm::vec3(10, 30, 40);
    else
      eye = glm::vec3(10, 30, 160);
    target = boat.position;
    up = glm::vec3(0,1,0);
    break;
  case follow_cam_view:
    eye.x = boat.position.x - cam_dir.x * 12;
    eye.z = boat.position.z - cam_dir.z * 12;
    eye.y = 8;
    target = boat.position;
    break;
  case heli_cam_view:
    break;
  };
  
}

void draw() {
  reset_screen();
  cam(cam_type);
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glUseProgram (programID);
  Matrices.view = glm::lookAt( eye, target, up );
  glm::mat4 VP = Matrices.projection * Matrices.view;
  boat.draw(VP);
  rookies_killed = true;
  for(int i=0; i<no_rocks; ++i)
    rock[i].draw(VP);
  for(int i=0; i<no_monsters; ++i) {
    monster[i].draw(VP);
    if( monster[i].alive)
      rookies_killed = false;
  }
  for(int i=0; i<no_barrels; ++i)
    barrel[i].draw(VP);
  if( rookies_killed && !boss_killed )
    boss.draw(VP);
  if( boss_hits <= 0 ) {
    boss.alive = false;
    boss_killed = true;
  }

  if( !boss_score && boss_hits <= 0 ) {
    player.score += 20;
    boss_score = true;
  }

  if( player.life < 0 ){
      player.life = 0.1;
  }


  bg.draw_sea(VP, boat.position, target - eye);
  for(int i=0; i<no_barrels; ++i)
    barrel[i].draw_collectible(VP);

  boat.draw_sail(VP);
  player.draw(VP, target, eye, boat.face_angle, cam_type);
}

void tick_input(GLFWwindow *window) {
  int left  = glfwGetKey(window, GLFW_KEY_LEFT);
  int right = glfwGetKey(window, GLFW_KEY_RIGHT);
  int up = glfwGetKey(window, GLFW_KEY_UP);
  int down = glfwGetKey(window, GLFW_KEY_DOWN);
  int b = glfwGetKey(window, GLFW_KEY_B);
  int canon_up = glfwGetKey(window, GLFW_KEY_W);
  int canon_down = glfwGetKey(window, GLFW_KEY_S);
  int space = glfwGetKey(window, GLFW_KEY_SPACE);

  int k = glfwGetKey(window, GLFW_KEY_K);
  if( boost && player.boost > 0 ) {
    player.boost -= 0.015;
    boat.top_speed = 0.2;
  }
  else
    boat.top_speed = 0.1;
  
  double num = 0.1;
  if( left ) {
    boat.turn_left();
  }
  if( right ) {
    boat.turn_right();
  }
  if( up ) {
    boat.accelerate();
  }
  else {
    boat.decelerate();
  }

  if( canon_up )
    boat.canon_rise(true);
  else if(canon_down)
    boat.canon_rise(false);
  
  if( down ) {
    glm::vec4 rev = glm::rotate(boat.face_angle, glm::vec3(0, 1, 0)) * glm::vec4(0, 0, 1, 0);

  }
  if( b )
    eye.y-=num;

  if( k) {
    boat.position.z -= 5;
    on_land = false;
  }
    
   
  
}

void tick_elements() {
  if( glm::length(glm::vec3(0, 0, 400) - boat.position) > 120 ) {
    boat.tick();
  }
  else
    on_land = true;
  
  for(int i=0; i<no_monsters; ++i)
    monster[i].tick(boat.position);
  if( rookies_killed && !boss_killed )
    boss.tick(boat.position);
  glm::vec4 cam_dir = glm::rotate(boat.face_angle, glm::vec3(0,1,0)) * glm::vec4(0,0,1,0);
}

void initGL(GLFWwindow *window, int width, int height) {
  // Create the models
  Matrices.projection = glm::perspective(glm::radians(90.0f), 14.0f/9.0f, 1.0f, 300.0f);
  boat = Boat(0, 3, COLOR_RED);
  cam(follow_cam_view);

  boost = false;
  
  player = Details(true);
  for(int i=0; i<no_rocks; ++i)
    rock[i] = Rock(rock_positions[i]);
  for(int i=0; i<no_monsters; ++i)
    monster[i] = Monster(monster_positions[i], i);
  boss = Monster(boss_position, 100);
  for(int i=0; i<no_barrels; ++i)
    barrel[i] = Barrel(barrel_positions[i], i%2);
  
  bg = Background(true);

  programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");

  Matrices.MatrixID = glGetUniformLocation(programID, "MVP");
  Matrices.Transparency = glGetUniformLocation(programID, "trns");


  reshapeWindow (window, width, height);

  glClearColor (COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
  glClearDepth (100.0f);

  GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat mat_shininess[] = { 50.0 };
  GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
  glShadeModel (GL_SMOOTH);
  
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);

  glColorMaterial ( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE ) ;

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable (GL_DEPTH_TEST);

  glfwSetKeyCallback(window, keyboard);
  glfwSetScrollCallback(window, scroll_callback);
  glfwSetCursorPosCallback(window, cursor_position);
  
  cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
  cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
  cout << "VERSION: " << glGetString(GL_VERSION) << endl;
  cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}

void secondScreen(){
    srand(time(0));
    window = initGLFW(width, height);
    initGL(window, width, height);
    while(!glfwWindowShouldClose(window)) {
        if(player.life<0.5)  {
            quit(window);
        }
        if (t60.processTick()) {
            engine(&boat, monster, rock, &player, barrel, &boss, rookies_killed && !boss_killed, &boss_hits);
            draw();
            glfwSwapBuffers(window);
            tick_elements();
            tick_input(window);
            glfwPollEvents();
        }
    }
}

void key(unsigned char key, int x, int y){
    switch(key){
        case 13:
            nextScreen=true;
            secondScreen();
    }
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glBegin(GL_QUADS);
    glTexCoord2i(0, 0); glVertex2i(0,   0);
    glTexCoord2i(0, 1); glVertex2i(0,   height);
    glTexCoord2i(1, 1); glVertex2i(width, height);
    glTexCoord2i(1, 0); glVertex2i(width, 0);
    glEnd();
    glutSwapBuffers();
}

void reshape(int w, int h){
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, width, height, 0.0, 0.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glutPostRedisplay();
}

void initGL(int w, int h)
{
    glViewport(0, 0, w, h);
    glEnable(GL_TEXTURE_2D);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, w, h, 0.0, 0.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

int LoadImage(char *filename)
{
    ILboolean success;
    ILuint image;

    ilGenImages(1, &image);
    ilBindImage(image);
    success = ilLoadImage(filename);
    if (success)
    {
        success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
        if (!success)
        {
            return -1;
        }
    }
    else
        return -1;
    return image;
}

int main(int argc, char **argv) {
    GLuint texid;
    int    image;
    glutInit(&argc, argv);
    argv[0]="FrontPage.jpg";
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
    glutInitWindowSize(width,height);
    glutInitWindowPosition(0,0);
    glutCreateWindow("Welcome");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    initGL(width, height);
    if (ilGetInteger(IL_VERSION_NUM) < IL_VERSION)
    {
        printf("wrong DevIL version");
        return -1;
    }
    ilInit();
    image = LoadImage(argv[0]);
    if ( image == -1 )
    {
        printf("Can't load picture file by DevIL");
        return -1;
    }
    glGenTextures(1, &texid);
    glBindTexture(GL_TEXTURE_2D, texid);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT),
                 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());
    glutKeyboardFunc(key);
    glutMainLoop();
    ilDeleteImages(1, reinterpret_cast<const ILuint *>(&image));
    glDeleteTextures(1, &texid);
    return 0;
}

void reset_screen() {
  float top    = screen_center_y + 4 / screen_zoom;
  float bottom = screen_center_y - 4 / screen_zoom;
  float left   = screen_center_x - 4 / screen_zoom;
  float right  = screen_center_x + 4 / screen_zoom;
}
