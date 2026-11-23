#pragma once
#include <algorithm>
#include <cstdlib>
#include "stage_scene.hpp"
#include "../assets/asset_store.hpp"
namespace shin {
// 430d70/430e00: non-colliding accelerating cloud objects, MAP3a.
class FlyingScenery {
 struct Cloud{int x,y,vx=0;bool fresh=true,removed=false,activated=false;};std::vector<Cloud> clouds_;std::vector<bool> spawned_;
public:
 void prepare(const std::vector<game::MapPlacement>&p,AssetStore&a){clouds_.clear();spawned_.assign(p.size(),false);const auto&im=a.image("cloud.app");for(std::size_t i=0;i<p.size();++i)if(p[i].type==0x3a){clouds_.push_back({p[i].pixel_x(im.width),p[i].pixel_y(im.height)});spawned_[i]=true;}}
 void tick(const std::vector<game::MapPlacement>&placements,int cx,int cy,AssetStore&a,IndexedSurface&f){
  if(spawned_.size()!=placements.size())prepare(placements,a);auto&image=a.image("cloud.app");
  for(auto&c:clouds_){if(c.removed)continue;if(!c.activated){if(c.x<cx||c.x>=cx+672||c.y<cy||c.y>=cy+512)continue;c.activated=true;}if(c.fresh){c.fresh=false;continue;}if(std::abs(c.x-cx-320)>960)continue;f.blit(image,c.x-cx,c.y-cy);c.x+=--c.vx;}

 }
 std::size_t active_count()const{return clouds_.size();}
};
}
