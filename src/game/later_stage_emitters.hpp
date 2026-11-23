#pragma once
#include "../assets/asset_store.hpp"
#include <functional>
namespace shin {
// MAP23 original4389c0/438a70 has no collision registration. It emits44e2c0.
class LaterStageEmitters {
public:
 struct Emitter {int x,y,frame=0,ticks=0;bool removed=false;};
 std::vector<Emitter> objects;
 std::function<void(unsigned,unsigned,unsigned,int,int,int)> spawn;
 std::function<void(int,int,int)> respawn;
 void add(int x,int y){objects.push_back({x,y});}
 void tick(int cx,int cy,AssetStore&assets,IndexedSurface&target){
  static constexpr const char*names[]={"hw1.app","hw2.app","hw3.app","hw4.app","hw5.app","hw6.app"};
  for(auto&o:objects){if(o.removed)continue;if(o.x<cx-target.origin_x-64||o.x>=cx-target.origin_x+target.width+64||o.y<cy-target.origin_y-96||o.y>=cy-target.origin_y+target.height+96)continue;
   target.blit(assets.image(names[o.frame]),o.x-cx,o.y-cy,true);
   if(o.frame==5&&o.ticks==9){if(!spawn)throw std::runtime_error("unbound original44e2c0 hazard constructor");spawn(0x44e2c0,0x44e3b0,0x44e4f0,0x62,o.x,o.y);}
   if(++o.ticks>10){o.ticks=0;o.frame=(o.frame+1)%6;}
  }
 }
};
}
