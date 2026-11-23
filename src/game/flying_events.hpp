#pragma once
#include "flying_stage.hpp"
namespace shin {
class FlyingEvents{
 struct Pose{int delay;std::vector<int>frames;};std::vector<const char*>names;std::vector<Pose>poses;std::vector<unsigned>times,voices;int type_=0;unsigned began=0,phase=0;bool running=false,enterheld=false;
public:
 bool active()const{return running;}
 bool stops_scroll()const{return running&&type_==76;}
 bool start(int type,unsigned now){if(running)return false;switch(type){
#include "flying_events_data.inc"
 default:return false;}type_=type;began=now;phase=0;running=true;enterheld=false;return true;}
 void tick(PlayerPayload&p,unsigned now,bool enter,int cx,int cy,AssetStore&a,IndexedSurface&f,const std::function<void(unsigned)>&sound){
 if(!running)return;p.at(0xfc)=0;p.at(0xe8)=1;if(enter)enterheld=true;if((!enter&&enterheld)||now-began>times.back()){running=false;p.at(0xfc)=0;p.at(0x54)=p.at(0x58)=0;FlyingStageMotion::pose(p);return;}
 unsigned next=0;while(next+1<times.size()&&now-began>times[next])++next;
 if(now==began||next!=phase){phase=next;p.at(0x1c)=p.at(0x20)=0;if(voices[phase])sound(voices[phase]);}
 auto&pose=poses[phase];f.blit_player(a.image(names.at(pose.frames.at(p.at(0x1c)%pose.frames.size()))),p.at(8)-cx,p.at(12)-cy);
 p.at(0x54)=type_==77?3:phase>=6?1:0;p.at(0x58)=type_==76&&phase<2?1:0;p.at(8)+=p.at(0x54);p.at(12)+=p.at(0x58);
 if(++p.at(0x20)>pose.delay){p.at(0x20)=0;p.at(0x1c)=(p.at(0x1c)+1)%pose.frames.size();}
 }
};
}
