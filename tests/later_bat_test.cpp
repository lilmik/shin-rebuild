#include "../src/game/later_bat.hpp"
#include <cassert>
int main(int argc,char**argv){assert(argc==2);shin::AssetStore a(argv[1]);shin::IndexedSurface f(640,480);shin::game::LaterBat e(300,100,a);for(int i=0;i<8;++i)e.tick(0,0,a,f);assert(e.payload.at(4)==292&&e.payload.at(8)==100);e.hurt();for(int i=0;i<10;++i)e.tick(0,0,a,f);assert(e.defeated);}
