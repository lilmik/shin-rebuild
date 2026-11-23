#pragma once
#include "player_object_response.hpp"
#include "collision_world.hpp"
#include "../assets/asset_store.hpp"
namespace shin {
class StageFallSequence {bool active_=false;
inline static constexpr const char*names[]={"fall1.app","fall2.app","fall3.app","fall4.app","fall5.app","fallba.app","fallbb.app","fallbc.app","fallbd.app","fallbe.app","fallca.app","fallcb.app","fallcc.app","fallcd.app","fallce.app"};
inline static constexpr int sequence[3][5]={{5,6,7,8,9},{0,1,2,3,4},{10,11,12,13,14}};
static void pose(PlayerPayload&p,int state){p.at(0x5c)=state;p.at(0x18)=state==95?0x45c950:state==96?0x45c968:0x45c980;p.at(0x1c)=p.at(0x20)=0;p.at(0x24)=5;p.at(0x28)=10;}
public:
 bool active()const{return active_;}
 void start(PlayerPayload&p){active_=true;pose(p,95);}
 bool tick(PlayerPayload&p,const game::CollisionWorld&w,int cx,int cy,AssetStore&assets,IndexedSurface&target){
  if(!active_)return false;int state=p.at(0x5c);p.at(0x54)=0;
  if(state==95&&p.at(0x1c)==4&&p.at(0x20)==9)pose(p,96);
  else if(state==96&&p.at(0x58)>0&&(w.ground({p.at(0x2c),p.at(0x30),p.at(0x34),p.at(0x38)},p.at(8),p.at(12)+4)||p.at(0x64)==1)){p.at(0x58)=0;pose(p,97);}
  else if(state==97&&p.at(0x1c)==4&&p.at(0x20)==9){active_=false;p.at(0x5c)=0;p.at(0x18)=0x45c714;p.at(0x1c)=p.at(0x20)=0;p.at(0x24)=1;p.at(0x28)=100;return true;}
  p.at(0x44)=p.at(8)-cx;p.at(0x48)=p.at(12)-cy;
  target.blit_player(assets.image(names[sequence[p.at(0x5c)-95][p.at(0x1c)]]),p.at(0x44)-17,p.at(0x48)-67,true);
  if(p.at(0x58)<11)++p.at(0x58);p.at(12)+=p.at(0x58);
  if(++p.at(0x20)>p.at(0x28)){p.at(0x20)=0;if(++p.at(0x1c)>=p.at(0x24))p.at(0x1c)=0;}return false;
 }
};
}
