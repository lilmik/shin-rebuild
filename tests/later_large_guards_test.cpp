#include "../src/game/later_large_guards.hpp"
#include <cassert>
int main(int argc,char**argv){assert(argc==2);shin::AssetStore a(argv[1]);shin::IndexedSurface f(640,480);for(int t:{20,21}){shin::game::LaterLargeGuard e(t,300,100,a);for(int i=0;i<8;++i)e.tick(0,0,a,f);assert(e.payload.at(4)==292);e.hurt();for(int i=0;i<12;++i)e.tick(0,0,a,f);assert(e.defeated&&e.payload.state==2);}}
