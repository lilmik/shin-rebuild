#pragma once
#include "level1_cutscenes.hpp"
#include <stdexcept>
namespace shin {
#include "stage2_intro_tables.inc"
#include "flying_intro_data.inc"
struct StageIntroFrame:CutsceneFrame {std::vector<unsigned> stop_sounds;};
// Stage indices follow stage_bindings; unsupported intros fail explicitly.
class StageIntro {
 unsigned stage_,phase_=0,ticks_=0,started_=0;bool done_=false,wait_skip_release_=false;Level1Intro first_;
public:
 explicit StageIntro(unsigned stage,bool wait_release=false):stage_(stage),wait_skip_release_(wait_release){if(stage>6)throw std::out_of_range("intro not recovered for stage");}
 unsigned phase()const{return stage_==0?first_.phase():phase_;}
 StageIntroFrame update(unsigned now,bool space,bool render=true){
  if(wait_skip_release_){if(!space)wait_skip_release_=false;space=false;}
  StageIntroFrame f;if(stage_==0){static_cast<CutsceneFrame&>(f)=first_.update(now,space,render);return f;}if(done_)return f;
  if(stage_>=4){std::vector<const char*>names;std::vector<unsigned>sequence,duration;std::vector<std::pair<unsigned,unsigned>>voices;switch(stage_){
#include "later_intro_data.inc"
 }if(render)f.draws={{"evframe.app",0,0,false},{names.at(sequence.at(phase_)),140,115,true}};if(ticks_==0){if(phase_==0)started_=now;for(auto v:voices)if(v.first==phase_)f.sounds.push_back(v.second);}if(space){done_=true;f.action=CutsceneFrame::resume_level;return f;}++ticks_;if(now-started_>duration.at(sequence.at(phase_))){started_=now;ticks_=0;if(++phase_==sequence.size()){done_=true;f.action=CutsceneFrame::resume_level;}}return f;}
  if(stage_==3){static const char*names[]={"eh1ed01.raw","eh1ed02.raw","eh1ed03.raw"};static constexpr unsigned sequence[]={0,1,2,1,0,1,2,1,0,1,2,1};if(render)f.draws={{"evframe.app",0,0,false},{names[sequence[phase_]],140,115,true}};if(ticks_==0&&phase_==0){started_=now;f.sounds.push_back(0x77e038);}if(space){done_=true;f.action=CutsceneFrame::resume_level;return f;}++ticks_;if(now-started_>210){started_=now;ticks_=0;if(++phase_==12){done_=true;f.action=CutsceneFrame::resume_level;}}return f;}
  if(stage_==2){if(render)f.draws={{"evframe.app",0,0,false},{flying_intro_assets[flying_intro_sequence[phase_]],140,115,true}};if(ticks_==0){if(phase_==0){started_=now;f.sounds.push_back(0x476758);}if(phase_==6)f.sounds.push_back(0x476760);}if(space){done_=true;f.action=CutsceneFrame::resume_level;return f;}++ticks_;if(unsigned(now-started_)>flying_intro_duration[flying_intro_sequence[phase_]]){started_=now;ticks_=0;if(++phase_==16){done_=true;f.action=CutsceneFrame::resume_level;}}return f;}
  if(render)f.draws={{"evframe.app",0,0,false},{stage2_intro_assets[stage2_intro_sequence[phase_]],140,115,true}};
  if(ticks_==0){if(phase_==0)started_=now;
   if(phase_==19)f.sounds.push_back(0x478150);
   if(phase_==27){f.stop_sounds.push_back(0x478150);f.sounds.push_back(0x478150);}
   switch(phase_){case 47:f.sounds.push_back(0x477e14);break;case 51:f.sounds.push_back(0x478158);break;case 53:f.sounds.push_back(0x477e10);break;case 77:f.sounds.push_back(0x477e90);break;}
  }
  if(space){done_=true;f.action=CutsceneFrame::resume_level;return f;}
  ++ticks_;if(unsigned(now-started_)>stage2_intro_duration[stage2_intro_sequence[phase_]]){started_=now;ticks_=0;if(++phase_==78){done_=true;f.action=CutsceneFrame::resume_level;}}
  return f;
 }
};
}
