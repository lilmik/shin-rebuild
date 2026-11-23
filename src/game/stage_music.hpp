#pragma once
#include <stdexcept>
namespace shin {
// 424e00: unsigned elapsed comparison is strict; stage2 is shoot.map.
class StageMusic {
 unsigned stage_,started_=0;bool playing_=false;
public:
 explicit StageMusic(unsigned stage):stage_(stage){if(stage>=7)throw std::out_of_range("music stage");}
 unsigned track()const{return stage_+4;}
 bool flying_stage()const{return stage_==2;}
 unsigned period()const{static constexpr unsigned periods[]={0x40740,98000,0x3efd0,0x36718,0x3e418,0x3dc48,0x2cad8};return periods[stage_];}
 bool update(unsigned now){if(!playing_||unsigned(now-started_)>period()){playing_=true;started_=now;return true;}return false;}
};
}
