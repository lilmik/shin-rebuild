#pragma once
#include "menu.hpp"
#include <cstdint>
namespace shin {
#include "cutscene_tables.inc"
struct CutsceneFrame {std::vector<MenuDraw>draws;std::vector<unsigned>sounds;unsigned cd_track=0;enum Action{none,resume_level,credits,main_menu}action=none;};
// Original callbacks43fe50/4493e0: drawing precedes skip/time advancement.
class Level1Intro {
 unsigned phase_=0,ticks_=0,started_=0;bool done_=false;
public:
 unsigned phase()const{return phase_;}
 CutsceneFrame update(unsigned now,bool space,bool render=true){
  CutsceneFrame f;if(done_)return f;
  if(render)f.draws={{"evframe.app",0,0,false},{intro_assets[intro_sequence[phase_]],140,115,true}};
  if(ticks_==0){if(phase_==0)started_=now;switch(phase_){case 4:f.sounds.push_back(0x77ff84);break;case 12:f.sounds.push_back(0x77fd50);break;case 13:f.sounds.push_back(0x77fcec);break;case 17:f.sounds.push_back(0x77ff80);break;case 29:f.sounds.push_back(0x77fce8);break;case 33:f.sounds.push_back(0x77ff8c);break;}}
  if(space){done_=true;f.action=CutsceneFrame::resume_level;return f;}
  ++ticks_;if(unsigned(now-started_)>intro_duration[intro_sequence[phase_]]){started_=now;ticks_=0;if(++phase_==41){done_=true;f.action=CutsceneFrame::resume_level;}}
  return f;
 }
};
// Global ending, NOT the first-level boss victory (which advances stage directly).
class GlobalEnding {
 unsigned phase_=0,ticks_=0,started_;int airplane_x_=140,airplane_y_=230;bool first_=true,done_=false;
public:
 explicit GlobalEnding(unsigned now):started_(now){}
 CutsceneFrame update(unsigned now,bool space,bool render=true){
  CutsceneFrame f;if(done_)return f;
  if(first_){first_=false;f.cd_track=11;f.sounds.push_back(0x78f8ac);}
  if(render){f.draws={{"evframe.app",0,0,false},{ending_assets[ending_sequence[phase_]],140,115,true}};if(phase_>6){f.draws.push_back({"airplane.app",airplane_x_,airplane_y_,true});airplane_x_+=2;--airplane_y_;}}
  if(space){done_=true;f.action=CutsceneFrame::credits;return f;}
  ++ticks_;if(unsigned(now-started_)>ending_duration[ending_sequence[phase_]]){started_=now;ticks_=0;if(++phase_==23){done_=true;f.action=CutsceneFrame::credits;}}
  return f;
 }
};
class EndingCredits {
 unsigned ticks_=0;bool done_=false;
public:
 CutsceneFrame update(bool skip=false){
  CutsceneFrame f;if(done_)return f;if(ticks_==0)f.cd_track=11;
  if(skip||ticks_==4000){done_=true;f.action=CutsceneFrame::main_menu;return f;}
  if(ticks_<3000)f.draws.push_back({ticks_<1000?"fuck-1.app":ticks_<2000?"fuck-3.app":"fuck-2.app",0,0,false});
  ++ticks_;return f; // 3000..3999 intentionally retain last framebuffer, as original.
 }
};
}
