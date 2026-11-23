#pragma once
#include "menu.hpp"
namespace shin {
struct OptionInput { bool up=false,down=false,enter=false,escape=false; };
enum class OptionAction { none, choice_zero_callback_42f9a0, choice_one_callback_4405c0, main_menu };
struct OptionFrame { std::vector<MenuDraw> draws;OptionAction action=OptionAction::none;bool stop_cd=false; };
// 0x44ae60: two-way submenu. Choice semantics deliberately use callback IDs.
class OptionMenu {
 int choice_=0; bool enter_=false;
public:
 OptionFrame update(OptionInput in){
  OptionFrame f;f.draws={{"tt0001.app",0,0,true},{choice_?"ssc2.app":"ssc1.app",180,200,true}};
  if(in.up)choice_=0;if(in.down)choice_=1;
  if(in.enter)enter_=true;else if(enter_){enter_=false;f.stop_cd=true;f.action=choice_?OptionAction::choice_one_callback_4405c0:OptionAction::choice_zero_callback_42f9a0;return f;}
  if(in.escape)f.action=OptionAction::main_menu;
  return f;
 }
 int choice()const{return choice_;}
};
}
