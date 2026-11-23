#pragma once
#include "later_mechanisms.hpp"
namespace shin {
// MAP5f: 419500 / 4195c0 / 419700. The source is a stationary contact flame,
// not a periodic projectile emitter. Animation table45efd8 has eight entries.
class LaterFireTraps {
public:
 struct Object{int index;game::EntityPayload e;};
 struct Respawn{int index,x,y;};std::vector<Object>objects;std::vector<Respawn>respawns;
 static game::CollisionRect rectangle(){return {10,10,40,40};}
 void spawn(int index,int x,int y){Object o{index,{}};auto&e=o.e;e.kind=0x45;e.state=1;e.at(4)=e.at(0x48)=x;e.at(8)=e.at(0x4c)=y;e.at(0x18)=e.at(0x1c)=0;e.at(0x28)=e.at(0x2c)=10;e.at(0x30)=e.at(0x34)=40;e.at(0x20)=8;e.at(0x24)=20;e.at(0x50)=e.at(0x54)=e.at(0x58)=e.at(0x68)=0;objects.push_back(o);}
 void tick(int cx,int cy,AssetStore&a,IndexedSurface&f){static constexpr int frames[]={0,1,2,1,0,1,2,1};for(auto&o:objects){auto&e=o.e;if(e.state==2)continue;if(e.state==1){e.state=0;continue;}

  e.at(0x48)=e.at(4);e.at(0x4c)=e.at(8);e.at(0x40)=e.at(4)-cx;e.at(0x44)=e.at(8)-cy;
  f.blit(a.image(std::string("fire")+std::to_string(frames[e.at(0x18)%8]+1)+".raw"),e.at(0x40),e.at(0x44));
  if(e.at(0x50)>0)--e.at(0x50);else if(e.at(0x50)<0)++e.at(0x50);
  e.at(4)+=e.at(0x50);e.at(8)+=e.at(0x54);
  if(++e.at(0x1c)>e.at(0x24)){e.at(0x1c)=0;e.at(0x18)=(e.at(0x18)+1)%8;}
 }}
};
}
