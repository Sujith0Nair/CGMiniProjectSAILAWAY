#define no_rocks 20
#define no_monsters 4
#define no_boss 1
#define no_barrels 10

#include "main.h"
#include "boat.h"
#include "rock.h"
#include "monster.h"
#include "details.h"
#include "barrel.h"

void engine(Boat * boat, Monster  monster[], Rock rock[], Details * player, Barrel * barrel, Monster * boss, bool b, int* boss_life);
