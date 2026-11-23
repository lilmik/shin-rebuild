#pragma once
#include "level1_cutscenes.hpp"
namespace shin {
#include "stage2_exit_tables.inc"
// resume_level means this exit completed: host advances stage and reloads spawn.
class Stage2ExitCutscene {
 unsigned phase_=0,ticks_=0,started_=0;bool done_=false;
public:
 unsigned phase()const{return phase_;}
 CutsceneFrame update(unsigned now,bool space,bool render=true){
  CutsceneFrame f;if(done_)return f;
  if(render)f.draws={{"evframe.app",0,0,false},{stage2_exit_assets[stage2_exit_sequence[phase_]],140,115,true}};
  if(phase_==0&&ticks_==0)started_=now;
  if(phase_==0&&ticks_==1)f.sounds.push_back(0x77fc44);
  if(phase_==6&&ticks_==0)f.sounds.push_back(0x77fbac);
  if(phase_==24&&ticks_==0)f.sounds.push_back(0x77fc40);
  if(space){done_=true;f.action=CutsceneFrame::resume_level;return f;}
  ++ticks_;if(unsigned(now-started_)>stage2_exit_duration[stage2_exit_sequence[phase_]]){started_=now;ticks_=0;if(++phase_==36){done_=true;f.action=CutsceneFrame::resume_level;}}
  return f;
 }
};
}
