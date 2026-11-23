#pragma once
#include "jungle_boss.hpp"
#include "player_world_response.hpp"
#include <functional>
namespace shin {
class JungleBossIntro {
#include "jungle_intro_data.inc"
 unsigned began_=0,phase_=0;bool active_=false;int fframe_=0,ftick_=0,sframe_=0,stick_=0,pframe_=0,ptick_=0;
public:
 bool active()const{return active_;}
 void begin(unsigned now,PlayerPayload&p,const std::function<void(unsigned)>&sound){began_=now;phase_=0;active_=true;fframe_=ftick_=sframe_=stick_=pframe_=ptick_=0;p.at(0xfc)=0;p.at(0x54)=0;p.at(0x5c)=0x4b;if(sound)sound(0x78e454);}
 bool tick(unsigned now,PlayerPayload&p,JungleBoss&first,JungleBoss&second,int cx,int cy,AssetStore&a,IndexedSurface&f,const game::CollisionWorld&w,const std::function<void(unsigned)>&sound){if(!active_)return false;unsigned t=now-began_;if(phase_==0&&t>1240){phase_=1;fframe_=ftick_=sframe_=stick_=0;if(sound)sound(0x78e45c);}if(phase_==1&&t>3450){phase_=2;fframe_=ftick_=0;if(sound)sound(0x78e44c);}if(phase_==2&&t>8090){phase_=3;pframe_=ptick_=0;p.at(0x5c)=0x49;if(sound)sound(0x78e450);}if(phase_==3&&t>12630){active_=false;p.at(0x5c)=0;p.at(0x18)=0x45c714;p.at(0x1c)=p.at(0x20)=0;p.at(0x24)=1;p.at(0x28)=100;first.begin_combat();second.begin_combat();return true;}
 auto npc=[&](JungleBoss&b,bool issecond){auto&e=b.payload;e.at(0x40)=e.at(4);e.at(0x44)=e.at(8);int*frame=issecond?&sframe_:&fframe_;int*tick=issecond?&stick_:&ftick_;int count=issecond?(phase_==0?2:1):(phase_==0?2:phase_==1?1:4);int index=issecond?(phase_==0?second0[*frame%count]:second1[0]):phase_==0?first0[*frame%count]:phase_==1?first1[0]:first2[*frame%count];f.blit(a.image(issecond?second_names[index]:first_names[index]),e.at(4)-cx,e.at(8)-cy);e.at(0x48)=0;if(e.at(0x4c)<11)++e.at(0x4c);e.at(8)+=e.at(0x4c);int material=w.body(b.rectangle(),e.at(4),e.at(8));if(material)b.terrain(w,material);if(++*tick>10){*tick=0;*frame=(*frame+1)%count;}};npc(first,false);npc(second,true);
 int count=phase_>=3?2:1;int index=phase_>=3?player1[pframe_%count]:player0[0];f.blit_player(a.image(player_names[index]),p.at(8)-cx,p.at(12)-cy);p.at(0x4c)=p.at(8);p.at(0x50)=p.at(12);p.at(0x54)=0;if(p.at(0x58)<11)++p.at(0x58);p.at(12)+=p.at(0x58);int material=w.body({p.at(0x2c),p.at(0x30),p.at(0x34),p.at(0x38)},p.at(8),p.at(12));if(material)game::respond_player_world(p,w,material);if(++ptick_>10){ptick_=0;pframe_=(pframe_+1)%count;}return false;
 }
};
class JungleMissiles{
#include "jungle_intro_data.inc"
public:
 struct Shot{game::EntityPayload e;int frame=0,ticks=0;bool right;};std::vector<Shot>shots;
 void spawn(int x,int y,int vx){Shot s{};s.e.kind=0x44;s.e.at(4)=x;s.e.at(8)=y;s.e.at(0x50)=vx;s.right=vx>=0;shots.push_back(s);}
 void tick(int cx,int cy,AssetStore&a,IndexedSurface&f,const game::CollisionWorld&w){for(auto&s:shots){auto&e=s.e;if(e.state==2)continue;if(std::abs(e.at(4)-cx-320)>960||e.at(8)>cy+400){e.state=2;continue;}f.blit(a.image(missile_names[s.frame]),e.at(4)-cx,e.at(8)-cy,true,0,0,-1,-1,s.right);e.at(0x48)=e.at(4);e.at(0x4c)=e.at(8);e.at(4)+=e.at(0x50);if(++s.ticks>4){s.ticks=0;s.frame=(s.frame+1)%3;}if(w.body({0,0,10,10},e.at(4),e.at(8)))e.state=2;}shots.erase(std::remove_if(shots.begin(),shots.end(),[](auto&s){return s.e.state==2;}),shots.end());}
};
}
