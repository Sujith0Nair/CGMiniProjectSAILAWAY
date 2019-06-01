#include "engine.h"

void engine(Boat * boat, Monster monster[], Rock rock[], Details * player, Barrel * barrel, Monster * boss, bool b, int* boss_life) {
  glm::vec3 c1 = boat->position;
  c1.y = 0;
  glm::vec4 c2_4 = glm::rotate(boat->face_angle, glm::vec3(0,1,0)) * glm::vec4(0,0,5.5,0);
  glm::vec3 c2 = c1 + glm::vec3(c2_4.x, 0, c2_4.z);

  float r1 = 3.1, r2 = 2.6;
  glm::vec3 bullet_pos;
  
  for(int i=0; i<no_monsters; ++i) {
    bullet_pos = monster[i].bullet_pos;
    if( monster[i].alive) {
    if( glm::length( bullet_pos - c1) <= r1) {// && monster[i].bullet_pos.y <= 1.5 && monster[i].bullet_pos.y >= -10) {
      monster[i].bullet_pos = glm::vec3(-1000,-1000,-1000);
      //      std::cout<<"true"<<glm::length( bullet_pos - c1)<<"\n";
      boat->position.x += 0.1;
      player->life -= 1;
    }
    if( glm::length( bullet_pos - c2) <= r2) {// && monster[i].bullet_pos.y <= 1.5 && monster[i].bullet_pos.y >= -10) {
      monster[i].bullet_pos = glm::vec3(-1000,-1000,-1000);
	   //      std::cout<<"true"<<glm::length( bullet_pos - c1)<<"\n";
      boat->position.x += 0.1;
      player->life -= 1;
    }
    }
  }

  r1 = 7; r2 = 6;
  glm::vec3 rock_pos, dir;
  for(int i=0; i<no_rocks; ++i) {
    rock_pos = rock[i].pos;
    rock_pos.y = 0;
    if( glm::length( rock_pos - c1) <= r1) {// && monster[i].rock_pos.y <= 1.5 && monster[i].rock_pos.y >= -10) {
      //      std::cout<<"r1"<<glm::length( rock_pos - c1)<<"\n";
      dir = glm::normalize(rock_pos - c1);
      boat->position.x -= dir.x/10;
      boat->position.z -= dir.z/10;
      player->life -= 0.02;
      std::cout<<"collided\n";
    }
    if( glm::length( rock_pos - c2) <= r2) {// && monster[i].rock_pos.y <= 1.5 && monster[i].bullet_pos.y >= -10) {
      //      std::cout<<"r2"<<glm::length( bullet_pos - c1)<<"\n";
      dir = glm::normalize(rock_pos - c2);
      boat->position.x -= dir.x/10;
      boat->position.z -= dir.z/10;

      player->life -= 0.02;
      std::cout<<"collided\n";
    }
  }


  for(int i=0; i<no_barrels; ++i) {
    if( glm::length(c1 - barrel[i].pos) <=r1 || glm::length(c2 - barrel[i].pos) <=r2) {
      if( boat->jumping) {
	if( !barrel[i].collected ) {
	  barrel[i].collected = true;
	  player->score += 1;
	  if( barrel[i].life )
	    player->life = 10;
	  else
	    player->boost = 10;
	}
      }
      else {
	dir = glm::normalize(barrel[i].pos - c1);
	boat->position.x -= dir.x/10;
	boat->position.z -= dir.z/10;
      }
    }
  }

  for(int i=0; i<20; ++i) {
    if( boat->active[i])
      for( int j=0; j<no_monsters; ++j)
	if( monster[j].alive)
	  if( glm::length(boat->bullet_coors[i] - monster[j].pos) < 15){
	    monster[j].alive = false;
	    monster[j].bullet_pos = glm::vec3(-1000, -1000, -1000);
	    std::cout<<"monster got";
	    player->score += 10;
	  }
    
  }

  for(int i=0; i<20; ++i) {
    if( boat->active[i])
      if( b)
	  if( glm::length(boat->bullet_coors[i] - boss->pos) < 10){
	    //boss->alive = false;
	    (*boss_life)--;
	    boss->bullet_pos = glm::vec3(-1000, -1000, -1000);
	    std::cout<<"monster got";
	  }
    
  }

  
    bullet_pos = boss->bullet_pos;
    if( b) {
    if( bullet_pos.y > -2 && glm::length( bullet_pos - c1) <= r1 + 1) {// && monster[i].bullet_pos.y <= 1.5 && monster[i].bullet_pos.y >= -10) {
      boss->bullet_pos = glm::vec3(-1000,-1000,-1000);
      //      std::cout<<"true"<<glm::length( bullet_pos - c1)<<"\n";
      boat->position.x += 0.1;
      std::cout<<"hit\n";
      player->life -= 1;
    }
    if( bullet_pos.y > -2 && glm::length( bullet_pos - c2) <= r2 + 1) {// && monster[i].bullet_pos.y <= 1.5 && monster[i].bullet_pos.y >= -10) {
      boss->bullet_pos = glm::vec3(-1000,-1000,-1000);
	   //      std::cout<<"true"<<glm::length( bullet_pos - c1)<<"\n";
      std::cout<<"hit\n";
      boat->position.x += 0.1;
      player->life -= 1;
    }
    }
  

  
}
    
