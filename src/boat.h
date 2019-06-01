#include "main.h"

#ifndef BOAT_H
#define BOAT_H


class Boat {
public:
    Boat() {}
    Boat(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    double speed;
    GLfloat color_buffer[108];
    float bounce_angle;
    float face_angle;
    float sail_angle;
    float canon_angle;
    float top_speed;

    glm::vec3 bullet_dir[20];
    glm::vec3 bullet_coors[20];
    bool active[20];
    int next;
    
    float jump_y;
    float jump_angle;
    bool jumping;

    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    void accelerate();
    void decelerate();
    void turn_left();
    void turn_right();
    void draw_sail(glm::mat4 VP);
    void canon_rise(bool up);
    void canon_fire();
    void jump();
   
private:
    VAO *back;
    VAO *left;
    VAO *right;
    VAO *front;
    VAO *floor;
    VAO *sail;
    VAO *stand;
    VAO *canon;
    VAO *bullet[20];
};

#endif // BALL_H
