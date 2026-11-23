#pragma once
#include "menu.hpp"
#include <algorithm>
namespace shin {
struct SettingsInput{bool up=false,down=false,left=false,right=false,enter=false,escape=false;};
struct SpeedConfig{int frame_divisor=1,speed_value=16,adjustment=0;bool persist=false;};
struct SettingsFrame{std::vector<MenuDraw> draws;enum Action{none,main_menu,option_menu,manual_speed,benchmark_auto,benchmark_manual}action=none;bool bonus_sound=false,stop_cd=false;};
// Functions 42f9a0,4405c0,4212c0. Host times 10 background blits for calibration.
class SettingsMenu{
public: enum Page{speed_mode,scroll_mode,speed_slider};
private:Page page_;int choice_=0,slider_=0;bool enter_=false;
public:
 SettingsMenu(Page p,const SpeedConfig& c):page_(p),slider_(209-c.speed_value){}
 int slider()const{return slider_;}
 SettingsFrame update(SettingsInput k,SpeedConfig& config,int& scrolling_mode){
  SettingsFrame f;
  if(page_==speed_mode){
   f.draws={{"tt0001.app",0,0,true},{choice_?"ssc3.app":"ssc4.app",180,200,true}};
   if(k.up)choice_=0;if(k.down)choice_=1;
  }else if(page_==scroll_mode){
   f.draws={{"scroll.app",0,0,true},{choice_?"scrollb.app":"scrolla.app",choice_?483:330,choice_?164:167,true}};
   if(k.left)choice_=0;if(k.right)choice_=1;
  }else{
   f.draws={{"tt0001.app",0,0,true},{"begin.app",60,200,true},{"expert.app",460,200,true},{"graph.app",100,260,true}};
   for(int i=0;i<slider_*2;++i)f.draws.push_back({"graph1.app",100+i,260,true});
   if(k.left&&slider_>0)--slider_;if(k.right&&slider_<209)++slider_;
  }
  if(k.enter)enter_=true;
  else if(enter_){enter_=false;
   if(page_==speed_mode){if(choice_){f.action=SettingsFrame::manual_speed;f.stop_cd=true;}else{f.action=SettingsFrame::benchmark_auto;}}
   else if(page_==scroll_mode){scrolling_mode=choice_!=0;f.action=SettingsFrame::main_menu;}
   else{config.speed_value=209-slider_;config.adjustment=0;f.action=SettingsFrame::benchmark_manual;}
   return f;
  }
  if(k.escape)f.action=page_==speed_mode?SettingsFrame::option_menu:SettingsFrame::main_menu;
  return f;
 }
 static void complete_calibration(SpeedConfig& config,unsigned elapsed_ms,bool automatic){
  config.frame_divisor=elapsed_ms<500?1:elapsed_ms<1200?2:3;
  if(automatic)config.speed_value=config.frame_divisor*16;
  config.persist=true;
 }
};
}

