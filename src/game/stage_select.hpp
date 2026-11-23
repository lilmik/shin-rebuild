#pragma once
#include <array>
#include <algorithm>
#include "menu.hpp"
#include <cstdint>
namespace shin {
struct SelectInput{bool left=false,right=false,up=false,down=false,space=false,enter=false,escape=false;};
struct SelectResult{enum Action{none,main_menu,start_stage,start_bonus}action=none;int index=-1;bool navigation_sound=false;};
// Control flow lifted from 0x448a20 assembly. Rendering/assets are separate.
class StageSelect {
 std::array<bool,5> unlocked_bonus_{};
 std::array<bool,5> armed_{}; // left,right,up,down,space; original initializes globals, caller supplies release first.
 int stage_count_,stage_=0,bonus_=0,mode_=1;
public:

 // Rendering order/positions from 0x448a20 and original constant tables.
 std::vector<MenuDraw> draw(unsigned noise_frame)const {
  std::vector<MenuDraw> out;
  out.push_back({"savebg.app",0,0,false});
  out.push_back({"tvnoise"+std::to_string(noise_frame%3+1)+".app",146,99,true});
  static constexpr int stage[][2]={{383,74},{405,74},{427,74},{449,74},{393,203},{412,203},{431,203}};
  static constexpr int selected_stage[][2]={{382,27},{404,27},{426,27},{448,27},{392,156},{411,156},{430,156}};
  static constexpr int bonus[][2]={{515,66},{515,127},{515,188},{515,249},{515,310}};
  static constexpr int selected_bonus[][2]={{500,55},{500,116},{500,177},{500,238},{500,299}};
  static constexpr int bubble[][2]={{5,40},{52,37},{104,41},{149,68},{189,115},{200,158},{204,215}};
  for(int i=0;i<std::min(7,stage_count_);++i)out.push_back({"svvdo"+std::to_string(i+1)+".app",stage[i][0],stage[i][1],true});
  out.push_back({"selector_fixed_tapes.app",438,199,true});
  static const char* bonus_names[]={"svpaccar.app","svpaccrd.app","svpacflg.app","svpacm.app","svpacpz.app"};
  for(int i=0;i<5;++i)if(unlocked_bonus_[i])out.push_back({bonus_names[i],bonus[i][0],bonus[i][1],true});
  if(mode_==1 && stage_>=0 && stage_<7){out.push_back({"svbubl"+std::to_string(stage_+1)+".app",bubble[stage_][0],bubble[stage_][1],true});out.push_back({"svvdo"+std::to_string(stage_+1)+"b.app",selected_stage[stage_][0],selected_stage[stage_][1],true});}
  else if(mode_==0){static const char* names[]={"fuckcar.app","fuckcrd.app","fuckflg.app","fuckm.app","fuckpz.app"};out.push_back({names[bonus_],selected_bonus[bonus_][0],selected_bonus[bonus_][1],true});}
  return out;
 }
 static constexpr const char* palette(){return "shin.pal";}
 // 0x4484f0: missing save.dat =>1, max7; five game.dat LE int32 flags.
 static int unlocked_stages(int saved){return std::min(saved,7);}
 StageSelect(int stages,std::array<bool,5> bonus):unlocked_bonus_(bonus),stage_count_(stages){}
 int stage()const{return stage_;}int bonus()const{return bonus_;}int mode()const{return mode_;}
 SelectResult update(SelectInput k){
  SelectResult r;if(k.escape){r.action=SelectResult::main_menu;return r;}
  auto edge=[&](int i,bool down){if(!down){armed_[i]=true;return false;}bool e=armed_[i];armed_[i]=false;return e;};
  if(edge(0,k.left)&&mode_==1){if(stage_>0){--stage_;r.navigation_sound=true;}}
  if(edge(1,k.right)&&mode_==1){if(stage_<stage_count_-1){++stage_;r.navigation_sound=true;}}
  if(edge(2,k.up)){
   if(mode_==1){if(stage_>=4){stage_-=4;r.navigation_sound=true;}}
   else for(int b=bonus_-1;b>=0;--b)if(unlocked_bonus_[b]){bonus_=b;r.navigation_sound=true;break;}
  }
  if(edge(3,k.down)){
   if(mode_==1){int n=stage_+4;if(n<stage_count_){stage_=n;r.navigation_sound=true;}else if(stage_count_>3)stage_=stage_count_-1;}
   else for(int b=bonus_+1;b<5;++b)if(unlocked_bonus_[b]){bonus_=b;r.navigation_sound=true;break;}
  }
  if(edge(4,k.space))for(int b=0;b<5;++b)if(unlocked_bonus_[b]){bonus_=b;mode_=1-mode_;break;}
  if(k.enter){if(mode_==1){r.action=SelectResult::start_stage;r.index=stage_;}else if(unlocked_bonus_[bonus_]){r.action=SelectResult::start_bonus;r.index=bonus_;}}
  return r;
 }
};
}
