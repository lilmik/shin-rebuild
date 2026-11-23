#include "../src/game/later_exit_sequence.hpp"
#include <cassert>
int main(){shin::LaterExitSequence e;assert(e.begin(50,100,200));assert(!e.begin(60,0,0));assert(e.spawns.size()==4&&e.spawns[1].x==460&&e.spawns[3].x==-240);e.helpers.clear();e.tick(1050);assert(e.helpers.empty());e.tick(1051);assert(e.helpers.size()==2&&!e.stage_advance_requested);e.helpers.clear();e.tick(2050);assert(e.active());e.tick(2051);assert(!e.active()&&e.stage_advance_requested&&e.helpers.size()==5);e.tick(4000);assert(e.helpers.size()==5);}
