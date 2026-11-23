#include "../src/game/stage2_remaining_entities.hpp"
#include <cassert>
int main(int argc,char**argv){assert(argc==2);shin::AssetStore a(argv[1]);shin::IndexedSurface f(640,480);shin::game::Stage2RemainingEnemy e(0x25,200,100,a);assert(e.payload.kind==0x71&&e.payload.at(0x34)==193);e.tick(0,0,a,f);assert(e.payload.at(4)==199);e.platform(2);e.tick(0,0,a,f);assert(e.payload.at(4)==200);e.hurt();assert(e.sound_requests.size()==1);for(int i=0;i<12;++i)e.tick(0,0,a,f);assert(e.defeated);}
