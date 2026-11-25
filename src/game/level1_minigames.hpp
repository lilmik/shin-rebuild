#pragma once
#include "../assets/asset_store.hpp"
#include <functional>
#include <algorithm>
namespace shin {
namespace minigame_data {
#include "minigame_tables.inc"
}
// MAP7b/7c interactive subgames. Level host freezes its scene while active,
// switches requested palette/CD, then resumes on returned finished flag.
class Level1Minigames {
public:
 struct Input {bool up=false,down=false,left=false,right=false,exit=false;};
 struct Output {bool finished=false;const char*palette=nullptr;int cd=-1;const char*loop_sound=nullptr;const char*stop_sound=nullptr;std::vector<std::string> sounds;};
private:
 int type_=0;bool active_=false,first_=true;unsigned music_=0;
 int car_frame_=0,lane_=0,row_=0;unsigned step_=0,crashed_at_=0;bool crashing_=false,crash_sound_=false,hit_enabled_=true,left_=false,right_=false;
 int flag_index_=0,stage_=0,lives_=5,pose_=2;bool pose_set_=false,pending_=false,answer_=false,transition_=false,transition_image_=true,feedback_=false,failed_=false;
 unsigned flag_clock_=0,response_start_=0,response_now_=0,transition_start_=0,feedback_start_=0;
 Output finish(Output o){active_=false;o.finished=true;o.palette="shin.pal";if(type_==0x7b)o.stop_sound="RESOURCE/car.wav";return o;}
 static void draw(AssetStore&a,IndexedSurface&f,const char*n,int x,int y,bool transparent=true){f.blit(a.image(n),x,y,transparent);}
public:
 bool active()const{return active_;}int type()const{return type_;}int car_row()const{return row_;}int car_lane()const{return lane_;}int flag_lives()const{return lives_;}int flag_stage()const{return stage_;}bool flag_response_pending()const{return answer_;}int flag_expected()const{return minigame_data::flag_expected0[flag_index_];}
 bool start(int type){if(active_||(type!=0x7b&&type!=0x7c))return false;*this=Level1Minigames();type_=type;active_=true;return true;}
 Output tick(unsigned now,const Input&input,AssetStore&assets,IndexedSurface&frame){
  Output out;if(!active_)return out;
  if(first_){first_=false;out.palette=type_==0x7b?"shin.pal":"flag.pal";out.cd=type_==0x7b?13:0;music_=now;}
  if(input.exit)return finish(out);
  if(type_==0x7b){
   if(now-music_>46000){out.cd=13;music_=now;}
   if(row_>192)return finish(out);
   if(step_==0){step_=now;out.loop_sound="RESOURCE/car.wav";}
   if(now-step_>500){++row_;step_=now;++car_frame_;car_frame_=car_frame_%2;}
   if(crashing_&&now-crashed_at_>2000)return finish(out);
   lane_=std::clamp(lane_,0,7);
   if(minigame_data::car_obstacles[8+row_*8+lane_]&&hit_enabled_){crashing_=true;crashed_at_=now;hit_enabled_=false;}
   if(crashing_&&!crash_sound_){out.sounds.push_back("RESOURCE/crash.wav");crash_sound_=true;}
   draw(assets,frame,car_frame_?"carbg1.app":"carbg.app",0,0,false);
   for(int x=0;x<8;++x)for(int y=0;y<8;++y)if(minigame_data::car_obstacles[(row_+y)*8+x])draw(assets,frame,"trap.app",64+x*64,448-y*64);
   draw(assets,frame,car_frame_?"car2.app":"car1.app",(lane_+1)*64,384);
   if(!crashing_){if(input.left&&!left_)--lane_;if(input.right&&!right_)++lane_;left_=input.left;right_=input.right;}
   return out;
  }
  if(flag_clock_==0)flag_clock_=now;
  if(feedback_&&now-feedback_start_>1000)feedback_=false;
  if(now-flag_clock_>unsigned(minigame_data::flag_interval0[flag_index_])){flag_clock_=now;pending_=true;}
  if(!pose_set_)pose_=2;
  if(response_start_==0)response_start_=now;
  if(flag_index_>29&&!answer_&&!pending_){++stage_;flag_index_=0;pending_=false;flag_clock_=0;response_now_=response_start_=0;answer_=false;transition_=true;transition_image_=true;}
  if(stage_>2||lives_==0)return finish(out);
  if(answer_&&!transition_){
   const int*deadline=stage_==0?minigame_data::flag_deadline0:stage_==1?minigame_data::flag_deadline1:minigame_data::flag_deadline2;
   int wanted=minigame_data::flag_expected0[flag_index_];
   bool timeout=int(response_now_-response_start_)>=deadline[flag_index_]+500;
   bool correct=(!timeout&&wanted==pose_&&pose_!=2)||(timeout&&pose_==2&&wanted==2);
   bool wrong=(!timeout&&pose_!=2&&wanted!=pose_)||(timeout&&!(pose_==2&&wanted==2));
   if(correct||wrong){out.sounds.push_back(correct?"RESOURCE/eat.wav":"RESOURCE/fail.wav");if(wrong)--lives_;failed_=wrong;feedback_=true;feedback_start_=now;answer_=false;}
  }
  if(!transition_start_&&transition_)transition_start_=now;
  if(transition_){pending_=false;if(now-transition_start_>2000){transition_image_=false;transition_=false;transition_start_=0;}}
  if(pending_&&!transition_){
   const int*indices=stage_==0?minigame_data::flag_sound0:stage_==1?minigame_data::flag_sound1:minigame_data::flag_sound2;
   int i=indices[flag_index_];if(i<0||i>=20)throw std::runtime_error("original flag voice index outside table");
   const char*const*voices=stage_==0?minigame_data::flag_voice0:stage_==1?minigame_data::flag_voice1:minigame_data::flag_voice2;
   out.sounds.push_back(voices[i]);pending_=false;answer_=true;++flag_index_;response_start_=now;
  }
  response_now_=now;
  if(!transition_||!transition_image_){
   const char*bgs[]={"flbg01.app","flbg02.app","flbg03.app"};const char*poses[]={"flblup.app","flbldn.app","fldeft.app","flwhup.app","flwhdn.app"};
   draw(assets,frame,bgs[stage_],0,0,false);draw(assets,frame,poses[pose_],93,19);
   for(int i=0;i<lives_;++i)draw(assets,frame,"lifeitem.app",minigame_data::flag_lives_xy[i*2],minigame_data::flag_lives_xy[i*2+1]);
   if(feedback_)draw(assets,frame,failed_?"x.app":"o.app",500,340);
   pose_set_=false;
  }else{draw(assets,frame,"nextbg.app",0,0,false);draw(assets,frame,"nextsp.app",52,98);}
  if(input.up){pose_=3;pose_set_=true;}else if(input.left){pose_=4;pose_set_=true;}else if(input.right){pose_=0;pose_set_=true;}else if(input.down){pose_=1;pose_set_=true;}
  return out;
 }
};
}
