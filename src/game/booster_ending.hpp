#pragma once
#include "level1_cutscenes.hpp"
namespace shin {
class BoosterEnding {
#include "booster_ending_data.inc"
 unsigned phase_=0,began_=0,ticks_=0;bool done_=false;
public:
 CutsceneFrame update(unsigned now,bool skip){CutsceneFrame f;if(done_)return f;f.draws={{"evframe.app",0,0,false},{names[seq[phase_]],140,115,true}};if(phase_==0&&ticks_==0)began_=now;if(phase_==0&&ticks_==1)f.sounds.push_back(0x4786ec);if(skip){done_=true;f.action=CutsceneFrame::resume_level;return f;}++ticks_;if(now-began_>duration[seq[phase_]]){began_=now;ticks_=0;if(++phase_==19){done_=true;f.action=CutsceneFrame::resume_level;}}return f;}
};
}
