#pragma once
#include "map_placement_assets.hpp"
#include "level1_objects.hpp"
#include "spring_sweep.hpp"
namespace shin {
// MAP9: 434cf0/434dc0, MAP58: 41ab50/41ac20. Their shared
// player callback 4098f0 case17 launches the player with vertical speed -35.
class Level1Springs {
 AssetStore&assets_;std::vector<game::MapPlacement> placements_;std::vector<bool> spawned_;
 struct Spring{std::size_t placement;int type;game::EntityPayload payload;};std::vector<Spring> springs_;
public:
 std::size_t size()const{return springs_.size();}
 static bool supports(int type){return type==9||type==0x45||type==0x58||type==0x59;}
 Level1Springs(AssetStore&a,const std::vector<game::MapPlacement>&p):assets_(a),placements_(p),spawned_(p.size()){
  for(std::size_t i=0;i<placements_.size();++i){auto&m=placements_[i];if(!supports(m.type))continue;
   auto&im=assets_.image(map_placement_asset(m.type));Spring s{i,m.type,{}};auto&e=s.payload;e.kind=0x40;e.state=1;e.at(4)=m.pixel_x(im.width);e.at(8)=m.pixel_y(im.height);e.at(0x14)=56;e.at(0x18)=10;e.at(0x28)=1;e.at(0x2c)=10;springs_.push_back(s);spawned_[i]=true;
  }
 }
 void tick(int cx,int cy,IndexedSurface&target,PlayerPayload&p,game::ObjectResponseContext&ctx,int old_player_x=2147483647,int old_player_y=2147483647){
  if(old_player_x==2147483647){old_player_x=p.at(0x4c);old_player_y=p.at(0x50);}
  for(auto&s:springs_){auto&e=s.payload;if(e.state==2)continue;if(e.state==1)e.state=0;
   if(e.at(0x50)==1&&e.at(0x20)==(s.type==9?4:2)&&e.at(0x24)==9){e.at(0x50)=0;e.at(0x20)=e.at(0x24)=0;e.at(0x28)=1;}
   int index=e.at(0x50)==1?e.at(0x20)+1:0;
   // The green atlas contains only three frames; clamp the shared callback's
   // out-of-atlas index rather than reproducing a legacy invalid surface read.
   std::string image=s.type==9?"spring"+std::to_string(index+1)+".app":"gspring"+std::to_string((s.type==0x45?1:s.type==0x59?7:4)+std::min(index,2))+".app";
   target.blit(assets_.image(image),e.at(4)-cx,e.at(8)-cy);
   if(++e.at(0x24)>e.at(0x2c)){e.at(0x24)=0;if(++e.at(0x20)>=e.at(0x28))e.at(0x20)=0;}
   if(spring_swept_contact(p,e,ctx,old_player_x,old_player_y,e.at(4),e.at(8)))continue;
   if(game::intersects({p.at(0x2c),p.at(0x30),p.at(0x34),p.at(0x38)},p.at(8),p.at(12),{0,0,56,10},e.at(4),e.at(8)))game::respond_player_object(p,e,ctx);
  }
 }
};
}
