#pragma once
#include "stage2_boss.hpp"
namespace shin::game {
// Millisecond-driven actor31(404320/4043f0) and actor2d(43c140/43c200).
// Host must drain player_calls through its recovered player helper dispatcher.
class Stage2BossSequence {
 enum class Mode{none,intro,ending};Mode mode_=Mode::none;unsigned started_=0;int step_=0;bool pressed_=false;Stage2Boss*boss_=nullptr;int saved_state_=0;bool restore_follow_=true;
 void finish(){completed=true;mode_=Mode::none;restore_player_state=saved_state_;restore_state_requested=true;camera_frozen=!restore_follow_;camera_restore_callback=restore_follow_?0x427ee0:0x427eb0;}
public:
 bool camera_frozen=false,completed=false,restore_state_requested=false;int restore_player_state=0;unsigned camera_restore_callback=0;std::vector<unsigned>player_calls;std::vector<std::string>sounds;
 bool active()const{return mode_!=Mode::none;}
 void begin_intro(Stage2Boss&boss,unsigned now,int player_state,bool camera_follow){saved_state_=player_state;restore_follow_=camera_follow;restore_state_requested=false;mode_=Mode::intro;started_=now;step_=0;completed=false;boss_=&boss;camera_frozen=true;boss.cinematic=true;boss.cinematic_pose(8);player_calls.push_back(0x40f750);sounds.push_back("RESOURCE/67.wav");}
 void begin_ending(unsigned now,int player_state,bool camera_follow){saved_state_=player_state;restore_follow_=camera_follow;restore_state_requested=false;mode_=Mode::ending;started_=now;step_=0;pressed_=false;completed=false;camera_frozen=true;player_calls.push_back(0x40fc20);sounds.push_back("RESOURCE/72.wav");}
 void tick(unsigned now,bool action_down,int player_x){if(!active())return;unsigned elapsed=now-started_;
  if(mode_==Mode::intro){if(step_==0&&elapsed>3180){player_calls.push_back(0x40f780);boss_->cinematic_pose(9);sounds.push_back("RESOURCE/69.wav");++step_;}if(step_==1&&elapsed>6190){player_calls.push_back(0x40f7e0);boss_->cinematic=false;if(player_x<boss_->payload.at(4))boss_->h4385e0();else boss_->h438610();finish();}return;}
  if(action_down)pressed_=true;else if(pressed_){player_calls.push_back(0x40fd40);finish();return;}
  static constexpr unsigned thresholds[]={1000,2000,3000,4000,6400},helpers[]={0x40fc80,0x40fcb0,0x40fce0,0x40fd10,0x40fd40};
  // Sequential ifs in43c200 permit multiple stages when wall time jumps.
  while(step_<5&&elapsed>thresholds[step_]){player_calls.push_back(helpers[step_]);if(++step_==5){finish();}}
 }
};
}
