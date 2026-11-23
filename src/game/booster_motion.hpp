#pragma once
#include "flying_stage.hpp"
namespace shin {
// 407380/40db10/40db80: fourth-selection inertial booster movement.
class BoosterMotion{
 bool initialized=false;
 static void pose(PlayerPayload&p){p.at(0x5c)=0x1a;p.at(0x18)=0x45c7c8;p.at(0x1c)=p.at(0x20)=0;p.at(0x24)=2;p.at(0x28)=10;}
public:
 bool hurt(PlayerPayload&p){if(p.at(0xb8)||p.at(0x5c)==0x1b)return false;p.at(0x5c)=0x1b;p.at(0x18)=0x45c7d8;p.at(0x1c)=p.at(0x20)=0;p.at(0x24)=1;p.at(0x28)=10;p.at(0x54)=-5;return true;}
 void tick(PlayerPayload&p,const InputState&in,int cx,int cy,int width,int height,AssetStore&a,IndexedSurface&f,FlyingStageMotion&shots){
 if(!initialized){pose(p);initialized=true;}
 if(p.at(0x5c)==0x1b&&p.at(0x1c)==0&&p.at(0x20)==9){pose(p);--p.at(0x8c);p.grant_damage_immunity();shots.game_over=p.at(0x8c)<=0;}
 p.at(0x4c)=p.at(8);p.at(0x50)=p.at(12);
 if(p.at(0x5c)==0x1c&&p.at(0x1c)==1){if(p.at(0x20)==1){game::EntityPayload s;s.kind=0x23;s.at(4)=p.at(8)+(p.at(0x88)?80:21);s.at(8)=p.at(12)+36;s.at(0x3c)=p.at(0x88)?12:-12;shots.shots.push_back(s);}if(p.at(0x20)==4)pose(p);}
 if(in.read(0x46d4b0)&&p.at(0x58)>-8)p.at(0x58)-=2;
 if(in.read(0x46d4b4)&&p.at(0x58)<10)p.at(0x58)+=2;
 if(in.read(0x46d4b8)){if(p.at(0x54)>-8)p.at(0x54)-=2;p.at(0x88)=0;}
 if(in.read(0x46d4bc)){if(p.at(0x54)<8)p.at(0x54)+=2;p.at(0x88)=1;}
 if(!in.read(0x46d4e8))p.at(0xe8)=0;else{if(!p.at(0xe8)&&p.at(0x5c)!=0x1b){p.at(0x5c)=0x1c;p.at(0x18)=0x45c7d0;p.at(0x1c)=p.at(0x20)=0;p.at(0x24)=2;p.at(0x28)=5;}p.at(0xe8)=1;}
 p.at(0x44)=p.at(8)-cx;p.at(0x48)=p.at(12)-cy;if(p.at(0xb8)%5==0)render_player(p,a,f);
 if(p.at(0x54)>0)--p.at(0x54);if(p.at(0x54)<0)++p.at(0x54);if(p.at(0x58)<11)++p.at(0x58);
 p.at(8)=std::clamp(p.at(8)+p.at(0x54),std::max(0,cx-f.origin_x),std::min(width*32,cx-f.origin_x+f.width-140));p.at(12)=std::clamp(p.at(12)+p.at(0x58),-30,height*32-40);
 if(++p.at(0x20)>p.at(0x28)){p.at(0x20)=0;p.at(0x1c)=(p.at(0x1c)+1)%p.at(0x24);}
 for(auto&s:shots.shots)if(s.state!=2){f.blit(a.image("shutga.app"),s.at(4)-cx,s.at(8)-cy);s.at(4)+=s.at(0x3c);if(std::abs(s.at(4)-cx-320)>960)s.state=2;}
 shots.shots.erase(std::remove_if(shots.shots.begin(),shots.shots.end(),[](auto&s){return s.state==2;}),shots.shots.end());
 }
};
}
