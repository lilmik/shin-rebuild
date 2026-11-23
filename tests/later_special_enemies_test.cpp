#include "../src/game/later_special_enemies.hpp"
#include <cassert>
#include "../src/game/player_animation.hpp"
int main(int argc,char**argv){assert(argc==2);shin::AssetStore a(argv[1]);shin::IndexedSurface f(640,480);shin::PlayerPayload p;shin::initialize_player_payload(p,4,4);shin::game::LaterSpecialEnemies manager;manager.spawn(0x22,400,50,1);manager.spawn(0x22,400,50,1);assert(manager.targets().size()==1);for(int i=0;i<250;++i)manager.tick(0,0,p,a,f);assert(manager.projectile_events.size()==1&&manager.projectile_events[0].vy==-16);auto*e=manager.targets().front();assert(manager.hurt(*e));assert(!manager.hurt(*e));for(int i=0;i<25;++i)manager.tick(0,0,p,a,f);assert(manager.targets().empty());}

