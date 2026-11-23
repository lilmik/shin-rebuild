#pragma once
#include "player_animation.hpp"
#include "player_object_response.hpp"
#include "collision_world.hpp"
#include <algorithm>
#include <cstdint>
namespace shin {
// Recovered from 436f10/4370f0/437200/437800/437830/437860.
class FlyingBoss {
 int fire_cooldown_=0;
 std::uint32_t random_=1;
 int random(){random_=random_*214013u+2531011u;return int((random_>>16)&32767);}
 void pose(int state){payload.at(0x50)=state;payload.at(0x10)=payload.at(0x14)=0;payload.at(0x18)=state==3?2:4;payload.at(0x1c)=state==4?20:10;}
 void fire(int px,int py){if(fire_cooldown_>0)return;fire_cooldown_=45;game::EntityPayload s;s.kind=0x25;s.at(0x14)=s.at(0x18)=2;s.at(0x1c)=s.at(0x20)=12;s.at(4)=payload.at(4);s.at(8)=payload.at(8)+30;s.at(0x3c)=(px-s.at(4))/40;s.at(0x40)=(py-s.at(8))/40;projectiles.push_back(s);}
public:
 game::EntityPayload payload;
 std::vector<game::EntityPayload> projectiles;
 std::vector<std::string> sounds;
 bool completed=false;
 FlyingBoss(int x,int y,int tag=0){payload.kind=0x6b;payload.at(0x20)=38;payload.at(0x24)=10;payload.at(0x28)=113;payload.at(0x2c)=189;payload.at(4)=x;payload.at(8)=y;payload.at(0x54)=tag;payload.at(0x6c)=20;pose(1);}
 static game::CollisionRect rectangle(){return {38,10,113,189};}
 bool hurt(){if(payload.state==2||payload.at(0x50)==4)return false;pose(4);sounds.push_back("RESOURCE/punch2.wav");return true;}
 void tick(int cx,int cy,int px,int py,int playerwidth,AssetStore&a,IndexedSurface&f){
  static const char* names[]={"chh12aa.raw","chh12ab.raw","chh12ac.raw","chh12ba.raw","chh123aa.raw","chh123ab.raw","chh124aa.raw","chh124ab.raw","chh124ac.raw","chh125aa.raw","chh125ab.raw","chh125ac.raw","chh125ba.raw","chh125bb.raw","chh125bc.raw"};
  if(fire_cooldown_>0)--fire_cooldown_;
  auto&p=payload;const auto&base=a.image(names[14]);p.at(0x64)=int(base.width);p.at(0x68)=int(base.height);
  if(p.state!=2){
   p.at(0x40)=p.at(4);p.at(0x44)=p.at(8);
   if(p.at(4)<cx)p.at(0x48)=10;if(p.at(4)>cx+500)p.at(0x48)=-10;
   if(p.at(8)<0){p.at(8)=0;p.at(0x4c)=10;}if(p.at(8)>480){p.at(8)=480;p.at(0x4c)=-10;}
   if(p.at(0x50)==1){
    if(p.at(0x58)==20){pose(3);p.at(0x58)=0;}
    p.at(0x48)=random()%7-3;p.at(0x4c)=random()%7-3;
    if(p.at(4)<cx)p.at(0x48)=10;if(p.at(4)>cx+500)p.at(0x48)=-10;
    if(p.at(8)<0)p.at(0x4c)=10;if(p.at(8)>500)p.at(0x4c)=-10;p.at(0x60)=0;
    if(p.at(0x58)>20){pose(3);p.at(0x58)=0;}
   }else if(p.at(0x50)==3){
    if(p.at(0x10)==0&&p.at(0x14)==1)p.at(0x60)=p.at(4)+p.at(0x64)/2<=px+playerwidth/2;
    if(p.at(0x10)==1&&p.at(0x14)==9){fire(px,py);pose(1);}
   }else if(p.at(0x50)==4){
    if(p.at(0x10)==0)fire(px,py);
    if(p.at(0x10)==1&&p.at(0x14)==0){if(--p.at(0x6c)<=0){completed=true;p.state=2;projectiles.clear();return;}pose(3);}
   }
   int index=p.at(0x10)+(p.at(0x50)==3?4:p.at(0x50)==4?6:0);
   const auto&image=a.image(names[index]);f.blit(image,p.at(4)-cx,p.at(8)-cy,true,0,0,-1,-1,p.at(0x60)!=0);
   f.blit(a.image("bsfc31.raw"),10,50);for(int i=0;i<p.at(0x6c);++i)f.blit(a.image("rlife.raw"),50+20*i,50);
   if(p.at(0x48)>0)--p.at(0x48);if(p.at(0x48)<0)++p.at(0x48);p.at(4)+=p.at(0x48);p.at(8)+=p.at(0x4c);
   if(++p.at(0x14)>p.at(0x1c)){p.at(0x14)=0;if(++p.at(0x10)>=p.at(0x18))p.at(0x10)=0;}++p.at(0x58);
  }
  for(auto&s:projectiles){if(s.state==2)continue;if(s.at(4)<cx-f.origin_x||s.at(4)>cx-f.origin_x+f.width||s.at(8)<cy-f.origin_y||s.at(8)>cy-f.origin_y+f.height){s.state=2;continue;}f.blit(a.image("shutfa.raw"),s.at(4)-cx,s.at(8)-cy);s.at(0x34)=s.at(4);s.at(0x38)=s.at(8);s.at(4)+=s.at(0x3c);s.at(8)+=s.at(0x40);}
  projectiles.erase(std::remove_if(projectiles.begin(),projectiles.end(),[](auto&s){return s.state==2;}),projectiles.end());
 }
};
}

