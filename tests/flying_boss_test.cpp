#include "../src/game/flying_boss.hpp"
#include <cassert>
int main(int argc,char**argv){
 assert(argc==2);shin::AssetStore a(argv[1]);shin::IndexedSurface f(640,480);shin::FlyingBoss b(400,100,7);
 assert(b.payload.kind==0x6b&&b.payload.at(0x6c)==20);
 for(int i=0;i<42;++i)b.tick(0,0,100,100,100,a,f);
 assert(!b.projectiles.empty());
 for(int hp=20;hp>0;--hp){assert(b.hurt());assert(!b.hurt());
  for(int i=0;i<22;++i)b.tick(0,0,100,100,100,a,f);
  assert(b.payload.at(0x6c)==hp-1);
 }
 assert(b.completed&&b.payload.state==2&&b.projectiles.empty());assert(!b.hurt());
}
