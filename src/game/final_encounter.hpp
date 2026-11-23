#pragma once
#include "final_boss_actor.hpp"
#include "final_boss_intro.hpp"
#include "player_world_response.hpp"
#include <memory>
namespace shin {
// Executable 434110/4341d0 actor rendering; timeline kept in FinalBossIntro.
class FinalEncounter {
#include "final_encounter_names.inc"
 struct Pose{std::vector<int>frames;int delay=10,frame=0,tick=0;void set(unsigned h){switch(h){
#include "final_encounter_poses.inc"
 default:return;}frame=tick=0;}void advance(){if(++tick>delay){tick=0;frame=(frame+1)%frames.size();}}};
 FinalBossIntro timeline_;Pose pp_,rp_,np_,prop_;int rxstate_=6,nxstate_=6,propx_=0,propy_=0,propvy_=0;bool propactive_=false;
public:
 std::unique_ptr<FinalBossActor>robot,nurse;bool active()const{return timeline_.active();}bool completed()const{return timeline_.finished;}
 bool focus_bounds(int&left,int&right,int&top,int&bottom)const{bool found=false;for(auto*b:{robot.get(),nurse.get()})if(b&&!b->defeated){int x=b->payload.words[1],y=b->payload.words[2];auto r=b->rectangle();if(!found){left=x+r[0];right=x+r[2];top=y+r[1];bottom=y+r[3];}else{left=std::min(left,x+r[0]);right=std::max(right,x+r[2]);top=std::min(top,y+r[1]);bottom=std::max(bottom,y+r[3]);}found=true;}if(propactive_&&found){
 // Frame the complete 426px actor at its landing height, not its offscreen birth.
 const int visible_y=std::max(propy_,bottom-426);
 left=std::min(left,propx_);right=std::max(right,propx_+198);
 top=std::min(top,visible_y);bottom=std::max(bottom,visible_y+426);
 }return found;}

 void begin(unsigned now){timeline_.begin(now);pp_.set(0x413230);}
 // Starts combat only after the whole original 37.69s dialogue; caller must
 // continue normal Boss tick/terrain/projectile processing after false.
 bool tick(unsigned now,PlayerPayload&p,int cx,int cy,AssetStore&a,IndexedSurface&f,const game::CollisionWorld&w,const std::function<void(unsigned)>&sound,const std::function<void(unsigned)>&music){if(!timeline_.active())return false;timeline_.tick(now,p.at(8),p.at(12));for(auto&s:timeline_.spawns){if(s.type==0x3b){robot=std::make_unique<FinalBossActor>(s.type,s.x,s.y,a);rp_.set(0x443890);rxstate_=6;}else if(s.type==0x29){nurse=std::make_unique<FinalBossActor>(s.type,s.x,s.y,a);np_.set(0x42af90);nxstate_=6;}else if(s.constructor==0x41f150){propx_=s.x;propy_=s.y;propvy_=0;propactive_=true;prop_.set(0x41f4e0);}}timeline_.spawns.clear();
 for(auto h:timeline_.helpers){if(h.target==FinalBossIntro::player){if(h.address==0x4132f0){p.at(0x5c)=0;p.at(0x18)=0x45c714;p.at(0x1c)=p.at(0x20)=0;p.at(0x24)=1;p.at(0x28)=100;}else if(h.address!=0x413200)pp_.set(h.address);}else if(h.target==FinalBossIntro::robot){if(h.address>=0x443890&&h.address<=0x443920){rp_.set(h.address);rxstate_=h.address==0x443890?6:h.address==0x4438c0?7:h.address==0x4438f0?8:9;}}else if(h.target==FinalBossIntro::nurse){if(h.address>=0x42af90&&h.address<=0x42aff0){np_.set(h.address);nxstate_=h.address==0x42af90?6:h.address==0x42afc0?7:9;}}}timeline_.helpers.clear();for(auto flag:timeline_.flags)if(nurse)nurse->payload.at(flag.offset)=flag.value;timeline_.flags.clear();for(auto id:timeline_.sounds)if(sound)sound(id);timeline_.sounds.clear();if(timeline_.music_request){if(music)music(timeline_.music_request);timeline_.music_request=0;}
 if(!timeline_.active()){if(robot)robot->resume_combat(p.at(8));if(nurse)nurse->resume_combat(p.at(8));return false;}
 auto npc=[&](FinalBossActor*b,Pose&pose,const char*const*names,bool isrobot,int state){if(!b)return;auto&e=b->payload;int vxoff=isrobot?0x50:0x48,vyoff=isrobot?0x54:0x4c;e.at(isrobot?0x48:0x40)=e.at(4);e.at(isrobot?0x4c:0x44)=e.at(8);f.blit(a.image(names[pose.frames[pose.frame]]),e.at(4)-cx,e.at(8)-cy);e.at(vxoff)=state==6?-4:0;if(e.at(vxoff)<0)++e.at(vxoff);if(e.at(vyoff)<11)++e.at(vyoff);e.at(4)+=e.at(vxoff);e.at(8)+=e.at(vyoff);int m=w.body(b->rectangle(),e.at(4),e.at(8));if(m)b->terrain(w,m);pose.advance();};npc(robot.get(),rp_,robot_names,true,rxstate_);npc(nurse.get(),np_,nurse_names,false,nxstate_);
 f.blit_player(a.image(player_names[pp_.frames[pp_.frame]]),p.at(8)-cx,p.at(12)-cy-9);p.at(0x4c)=p.at(8);p.at(0x50)=p.at(12);p.at(0x54)=0;if(p.at(0x58)<11)++p.at(0x58);p.at(12)+=p.at(0x58);int m=w.body({p.at(0x2c),p.at(0x30),p.at(0x34),p.at(0x38)},p.at(8),p.at(12));if(m)game::respond_player_world(p,w,m);pp_.advance();
 if(propactive_){
  f.blit(a.image(prop_names[prop_.frames[prop_.frame]]),propx_-cx,propy_-cy);
  const int oldy=propy_;if(propvy_<11)++propvy_;propy_+=propvy_;
  // 41f150: offsets 28..34 are {100,200,200,426}; 41f3a0
  // resolves only after contact, including a sweep back to the previous Y.
  constexpr game::CollisionRect body{100,200,200,426};
  const int material=w.body(body,propx_,propy_);
  if(material==1||material==2){
   bool resolved=false;
   for(int n=0;n<26;++n)if(!w.body(body,propx_,propy_-n)){
    propy_-=n;if(material==2)propvy_+=2;resolved=true;break;
   }
   if(!resolved&&(material==1||propy_>oldy)){
    const int step=propy_>oldy?-1:1;
    for(int y=propy_;step<0?y>=oldy:y<=oldy;y+=step)
     if(!w.body(body,propx_,y)){propy_=y;break;}
   }
  }
  prop_.advance();
 }
 return true;}
};
class FinalBossMissiles {
#include "final_encounter_names.inc"
public:
 struct Shot{game::EntityPayload e;int frame=0,ticks=0,age=0;bool right;};std::vector<Shot>shots;
 void spawn(unsigned constructor,int x,int y,int vx){Shot s{};s.e.kind=constructor==0x448110?0x63:0x43;s.e.at(4)=x;s.e.at(8)=y;s.e.at(0x50)=vx;s.e.at(0x14)=0;s.e.at(0x18)=0;s.e.at(0x1c)=10;s.e.at(0x20)=10;if(s.e.kind==0x43){s.e.at(4)-=37;s.e.at(8)-=50;s.e.at(0x14)=-6;s.e.at(0x18)=-12;s.e.at(0x1c)=127;s.e.at(0x20)=114;}s.right=vx>=0;shots.push_back(s);}
 void tick(int cx,int cy,AssetStore&a,IndexedSurface&f,const game::CollisionWorld&w){for(auto&s:shots){auto&e=s.e;if(e.state==2)continue;if((e.at(4)<cx+std::min(-640,-f.origin_x-128)||e.at(4)>cx+std::max(1280,f.width-f.origin_x+128))||(e.kind==0x63&&e.at(8)>cy+std::max(400,f.height-f.origin_y+32))){e.state=2;continue;}f.blit(a.image(e.kind==0x63?missile_names[s.frame]:"shoke.raw"),e.at(4)-cx,e.at(8)-cy,true,0,0,-1,-1,s.right);if(e.kind==0x63){e.at(4)+=e.at(0x50);if(++s.ticks>4){s.ticks=0;s.frame=(s.frame+1)%2;}if(w.body({0,0,10,10},e.at(4),e.at(8)))e.state=2;}else if(++s.age>10)e.state=2;}shots.erase(std::remove_if(shots.begin(),shots.end(),[](auto&s){return s.e.state==2;}),shots.end());}
};
}
