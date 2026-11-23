#include "../src/game/final_boss_actor.hpp"
#include <cassert>
#include <iostream>
int main(int argc,char**argv){try{assert(argc==2);shin::AssetStore a(argv[1]);shin::IndexedSurface f(640,480);for(int type:{0x29,0x3b}){shin::FinalBossActor b(type,400,0,a);assert(b.hp()==10);for(int hp=10;hp>0;--hp){assert(b.hurt());assert(!b.hurt());for(int n=0;n<(type==0x3b?12:22);++n)b.tick(0,0,0,0,100,a,f);assert(b.hp()==hp-1);}assert(b.defeated&&b.payload.state==2);}}catch(const std::exception&e){std::cerr<<e.what()<<"\n";return 2;}}
