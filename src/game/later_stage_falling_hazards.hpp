#pragma once
#include "player_object_response.hpp"
#include "collision_world.hpp"
#include "../assets/asset_store.hpp"
namespace shin {
// Original44e2c0/44e3b0 falling MAP23 emissions; only object collision, no terrain registry.
class LaterStageFallingHazards {
public:
 struct Hazard {game::EntityPayload e;int frame=0,ticks=0;};std::vector<Hazard> objects;
 void spawn(int x,int y){Hazard h;h.e.kind=0x62;h.e.state=1;h.e.at(4)=x;h.e.at(8)=y;h.e.at(0xc)=101;h.e.at(0x10)=153; //44e170 retains final ddw2 dimensions.
h.e.at(0x28)=9;h.e.at(0x2c)=58;h.e.at(0x30)=88;h.e.at(0x34)=145;h.e.at(0x54)=6;objects.push_back(h);}
 void tick(int cx,int cy,AssetStore&a,IndexedSurface&target,PlayerPayload&p,game::ObjectResponseContext&ctx){
  for(auto&h:objects){auto&e=h.e;if(e.state==2)continue;if(e.state==1){e.state=0;continue;}
   if(std::abs(e.at(4)-cx-320)>960||cy+380<e.at(8)){e.state=2;continue;}
   e.at(0x48)=e.at(4);e.at(0x4c)=e.at(8);e.at(0x40)=e.at(4)-cx;e.at(0x44)=e.at(8)-cy;
   target.blit(a.image(h.frame?"ddw2.app":"ddw1.app"),e.at(0x40),e.at(0x44),true);
   if(e.at(0x50)>0)--e.at(0x50);if(e.at(0x50)<0)++e.at(0x50);if(e.at(0x54)<11)++e.at(0x54);e.at(4)+=e.at(0x50);e.at(8)+=e.at(0x54);
   if(++h.ticks>4){h.ticks=0;h.frame=(h.frame+1)%2;}
   if(game::intersects({p.at(0x2c),p.at(0x30),p.at(0x34),p.at(0x38)},p.at(8),p.at(12),{9,58,88,145},e.at(4),e.at(8)))game::respond_player_object(p,e,ctx);
  }
 }
};
}
