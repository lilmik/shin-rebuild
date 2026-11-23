#pragma once
#include "player_animation.hpp"
#include <functional>
namespace shin {
class LaterMonologues{
 struct Pose{int state,delay;std::vector<int>frames;};std::vector<const char*>names;std::vector<Pose>poses;std::vector<unsigned>times,voices;unsigned start_=0,phase_=0;bool running_=false,enter_=false;int type_=0;
 void pose(PlayerPayload&p){auto&q=poses[phase_];p.at(0x5c)=q.state;p.at(0x1c)=p.at(0x20)=0;p.at(0x24)=int(q.frames.size());p.at(0x28)=q.delay;}
 void finish(PlayerPayload&p){if(type_==0x72&&resume_actor)resume_actor();running_=false;p.at(0x5c)=0;p.at(0x18)=0x45c714;p.at(0x1c)=p.at(0x20)=0;p.at(0x24)=1;p.at(0x28)=100;}
public:
 std::function<void(int,int)> spawn_actor;
 std::function<void()> resume_actor;
 static bool supports(int type){return type==0x55||type==0x6b||type==0x6c||type==0x6d||type==0x6e||type==0x71||type==0x72||type==0x73;}
 int type()const{return type_;}
 bool active()const{return running_;}
 bool start(int type,PlayerPayload&p,unsigned now,const std::function<void(unsigned)>&sound){if(running_)return false;switch(type){
#include "later_monologues_data.inc"
 default:return false;}type_=type;if(type==0x72&&spawn_actor)spawn_actor(p.at(8)+360,p.at(12)-200);running_=true;enter_=false;phase_=0;start_=now;pose(p);if(voices[0])sound(voices[0]);return true;}
 void tick(PlayerPayload&p,unsigned now,bool enter,int cx,int cy,AssetStore&a,IndexedSurface&f,const std::function<void(unsigned)>&sound){if(!running_)return;if(enter)enter_=true;else if(enter_){finish(p);return;}if(now-start_>times[phase_]){if(++phase_==poses.size()){finish(p);return;}pose(p);if(voices[phase_])sound(voices[phase_]);}auto&q=poses[phase_];f.blit_player(a.image(names.at(q.frames.at(p.at(0x1c)))),p.at(8)-cx,p.at(12)-cy-9);p.at(0x54)=0;if(p.at(0x58)<11)++p.at(0x58);p.at(12)+=p.at(0x58);if(++p.at(0x20)>q.delay){p.at(0x20)=0;p.at(0x1c)=(p.at(0x1c)+1)%q.frames.size();}}
};
}
