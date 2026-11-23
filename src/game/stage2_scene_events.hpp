#pragma once
#include "player_animation.hpp"
#include <functional>
namespace shin {
// 4416e0/4322b0/422720/446670: one-shot stage-two player monologues.
class Stage2SceneEvents {
 struct Pose{int state;std::vector<int> frames;};
 std::vector<std::string>names_,sounds_;std::vector<unsigned>times_;std::vector<Pose>poses_;
 int type_=0;unsigned started_=0,phase_=0;bool active_=false,enter_=false;
 void pose(PlayerPayload&p){p.at(0x5c)=poses_[phase_].state;p.at(0x1c)=p.at(0x20)=0;p.at(0x24)=int(poses_[phase_].frames.size());p.at(0x28)=10;}
 void finish(PlayerPayload&p){active_=false;p.at(0x5c)=0;p.at(0x18)=0x45c714;p.at(0x1c)=p.at(0x20)=0;p.at(0x24)=1;p.at(0x28)=100;}
public:
 using Sound=std::function<void(const char*)>;
 static bool supports(int type){return type==0x46||type==0x47||type==0x48||type==0x4a;}
 bool active()const{return active_;} unsigned phase()const{return phase_;}
 bool start(int type,PlayerPayload&p,unsigned now,const Sound&sound){if(active_||!supports(type))return false;type_=type;switch(type){
#include "stage2_scene_event_data.inc"
 }active_=true;enter_=false;started_=now;phase_=0;pose(p);if(sound)sound(sounds_[0].c_str());return true;}
 // Original 4416e0 -> 427eb0 restores normal camera following. The host
 // must keep following an airborne player and apply terrain response exactly once.
 bool tick(PlayerPayload&p,unsigned now,bool enter,int cx,int cy,AssetStore&a,IndexedSurface&f,const Sound&sound,const std::function<void(PlayerPayload&)>&collision){
  if(!active_)return false;if(enter)enter_=true;else if(enter_){finish(p);return true;}
  if(unsigned(now-started_)>times_[phase_]){
   if(phase_+1==poses_.size()){finish(p);return true;}
   ++phase_;pose(p);
   if(sound&&((type_==0x46&&phase_==3)||(type_==0x47&&phase_==3)||(type_==0x4a&&phase_==1)))sound(sounds_[1].c_str());
   if(sound&&type_==0x47&&phase_==4)sound(sounds_[2].c_str());
  }
  p.at(0x54)=0;p.at(0x44)=p.at(8)-cx;p.at(0x48)=p.at(12)-cy;
  f.blit_player(a.image(names_.at(poses_[phase_].frames.at(p.at(0x1c)))),p.at(0x44),p.at(0x48)-9,true);
  if(p.at(0x58)<11)++p.at(0x58);p.at(12)+=p.at(0x58);
  if(++p.at(0x20)>10){p.at(0x20)=0;p.at(0x1c)=(p.at(0x1c)+1)%p.at(0x24);}
  if(collision)collision(p);return false;
 }
};
}
