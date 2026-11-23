#pragma once
#include "player_controller.hpp"
namespace shin::game {
// Confirmed end-of-first-stage transition in 4220a0 state4/frame0/tick9/HP0.
struct Level1Music {
 unsigned started=0;bool playing=false;
 // 424e00 case0, confirmed runtime CD4..5; strict 264000ms replay.
 bool update(unsigned now){if(!playing||unsigned(now-started)>264000u){started=now;playing=true;return true;}return false;}
 static constexpr unsigned cd_track=4;
};
struct Level1Events {
 int defeated_bosses=0,current_stage=0,previous_stage=-1;bool completed=false;
 bool boss_transition(int state,int frame,int frame_tick,int hp,PlayerPayload&player){
  if(state!=4||frame!=0||frame_tick!=9||hp!=0)return false;
  if(completed)return false;
  ++defeated_bosses;previous_stage=current_stage;++current_stage;player.at(0xbc)=0;player.at(0xb4)=0;completed=true;return true;
 }
};
}
