#pragma once
#include "player_animation.hpp"
#include "collision_world.hpp"
#include "player_world_response.hpp"
#include <functional>
namespace shin {
// sixth selection MAP6f ->445810/4458d0, MAP70 ->4378e0/437a80.
class SixthStageScenes {
#include "sixth_stage_scene_data.inc"
 struct Actor{int type,x,y,vy=0,frame=0,tick=0;};std::vector<Actor>actors_;int type_=0,phase_=0,frame_=0,tick_=0;unsigned began_=0;bool active_=false,enterheld_=false;
 static void restore(PlayerPayload&p){p.at(0x5c)=0;p.at(0x18)=0x45c714;p.at(0x1c)=p.at(0x20)=0;p.at(0x24)=1;p.at(0x28)=100;p.at(0x54)=0;}
public:
 template<class F> void visit_actors(F&&f)const{for(const auto&a:actors_)f(a.type,a.x,a.y);}
 bool advance_stage_requested=false;
 bool active()const{return active_;}bool exiting()const{return active_&&type_==0x70;}
 bool begin(int type,unsigned now,PlayerPayload&p,const std::function<void(unsigned)>&sound){if(active_||(type!=0x6f&&type!=0x70))return false;type_=type;began_=now;phase_=frame_=tick_=0;enterheld_=false;active_=true;advance_stage_requested=false;actors_.clear();p.at(0xfc)=0;p.at(0x54)=0;if(type==0x70){actors_={{0x3b,p.at(8)+300,p.at(12)-200},{0x29,p.at(8)+360,p.at(12)-200},{0x27,p.at(8)-300,p.at(12)-200},{0x28,p.at(8)-340,p.at(12)-200}};p.at(0x5c)=0x77;}else{p.at(0x5c)=0x59;if(sound)sound(0x78f228);}return true;}
 bool tick(unsigned now,bool enter,PlayerPayload&p,int cx,int cy,AssetStore&a,IndexedSurface&f,const game::CollisionWorld&w,const std::function<void(unsigned)>&sound){if(!active_)return false;unsigned t=now-began_;
 if(type_==0x6f){if(enter)enterheld_=true;if((!enter&&enterheld_)||t>19570){restore(p);active_=false;return true;}if(phase_==0&&t>3830){phase_=1;if(sound)sound(0x78f220);}if(phase_==1&&t>13070){phase_=2;frame_=tick_=0;p.at(0x5c)=0x5a;}if(phase_==2&&t>15360){phase_=3;frame_=tick_=0;p.at(0x5c)=0x5b;}int count=phase_<2?2:phase_==2?2:1,index=phase_<2?mono0_frames[frame_%count]:phase_==2?mono1_frames[frame_%count]:mono2_frames[0];f.blit_player(a.image(monologue_names[index]),p.at(8)-cx,p.at(12)-cy-9);if(++tick_>10){tick_=0;frame_=(frame_+1)%count;}}
 else {if(phase_==0&&t>1000){phase_=1;for(auto&o:actors_)if(o.type==0x3b||o.type==0x29)o.frame=o.tick=0;}if(t>2000){restore(p);active_=false;advance_stage_requested=true;return true;}for(auto&o:actors_){const char*name;game::CollisionRect rect;int count,vx;if(o.type==0x3b){count=phase_==0?4:1;name=robot_names[phase_==0?robot0_frames[o.frame%count]:robot1_frames[0]];rect={83,28,157,239};vx=phase_==0?-3:0;}else if(o.type==0x29){count=phase_==0?4:1;name=nurse_names[phase_==0?nurse0_frames[o.frame%count]:nurse1_frames[0]];rect={105,26,178,148};vx=phase_==0?-3:0;}else if(o.type==0x27){count=3;name=first_names[first_frames[o.frame%count]];rect={33,28,79,201};vx=1;}else{count=3;name=second_names[second_frames[o.frame%count]];rect={36,9,88,191};vx=1;}f.blit(a.image(name),o.x-cx,o.y-cy);int oldx=o.x,oldy=o.y;o.x+=vx;if(o.vy<11)++o.vy;o.y+=o.vy;if(w.body(rect,o.x,o.y)){bool cleared=false;for(int n=0;n<26;++n)if(!w.body(rect,o.x,o.y-n)){o.y-=n;cleared=true;break;}if(!cleared){o.x=oldx;o.y=oldy;}}if(++o.tick>10){o.tick=0;o.frame=(o.frame+1)%count;}}f.blit_player(a.image(player_names[0]),p.at(8)-cx,p.at(12)-cy);}
 p.at(0x4c)=p.at(8);p.at(0x50)=p.at(12);p.at(0x54)=0;if(p.at(0x58)<11)++p.at(0x58);p.at(12)+=p.at(0x58);int material=w.body({p.at(0x2c),p.at(0x30),p.at(0x34),p.at(0x38)},p.at(8),p.at(12));if(material)game::respond_player_world(p,w,material);return false;
 }
};
}
