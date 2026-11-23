#include "../src/game/later_patrol_enemies.hpp"
#include <cassert>
int main(int argc,char**argv){assert(argc==2);shin::AssetStore a(argv[1]);shin::IndexedSurface f(640,480);for(int t:{22,23,51}){shin::game::LaterPatrolEnemy enemy(t,400,100,a);for(int i=0;i<140;++i)enemy.tick(0,0,a,f);if(t==22)assert(enemy.projectile_events.size()==1);enemy.hurt();for(int i=0;i<12;++i)enemy.tick(0,0,a,f);assert(enemy.defeated);}}
