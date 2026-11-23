#pragma once
#include <vector>
#include <cstdint>
namespace shin {
// MAP70: 4415a0 ->441670 ->4378e0/437a80. Host must execute helper
// requests and transient spawns, then honor stage_advance_requested.
class LaterExitSequence {
 unsigned began_=0;int phase_=0;bool active_=false;
public:
 struct Spawn{int type,x,y;};std::vector<Spawn> spawns;std::vector<unsigned> helpers;
 bool stage_advance_requested=false;
 bool active()const{return active_;}
 bool begin(unsigned now,int x,int y){if(active_)return false;active_=true;began_=now;phase_=0;stage_advance_requested=false;
 helpers={0x4128e0,0x443b20,0x42b1f0,0x405820,0x444f50,0x427eb0};spawns={{0x3b,x+300,y-200},{0x29,x+360,y-200},{0x27,x-300,y-200},{0x28,x-340,y-200}};return true;}
 void tick(unsigned now){if(!active_)return;unsigned elapsed=now-began_;if(phase_==0&&elapsed>1000){helpers.push_back(0x443b80);helpers.push_back(0x42b250);phase_=1;}
 if(phase_==1&&elapsed>2000){for(unsigned h:{0x412940u,0x443bb0u,0x42b280u,0x405880u,0x444fb0u})helpers.push_back(h);phase_=2;active_=false;stage_advance_requested=true;}}
};
}
