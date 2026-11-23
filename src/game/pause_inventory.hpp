#pragma once
#include "player_controller.hpp"
#include "../assets/asset_store.hpp"
#include "../core/input_state.hpp"
namespace shin {
// 0042f060 inventory overlay. Snapshot and lifecycle are supplied by LevelRuntime.
class PauseInventory {
 int choice_;bool enter_=false,up_=false,down_=false;
public:
 explicit PauseInventory(int choice):choice_(choice){}
 int choice()const{return choice_;}
 bool tick(const InputState&input,PlayerPayload&p,AssetStore&assets,IndexedSurface&frame){
  constexpr const char* names[]={"ball.app","wbomb.app","item_m.app","item_f.app","item_si.app","item_ym.app","item_fr.app","item_w.app"};
  constexpr unsigned fields[]={0xe0,0xe4,0xc8,0xcc,0xd0,0xd4,0xd8,0xdc};
  for(int i=0;i<8;++i)for(int count=0;count<p.at(fields[i]);++count)frame.blit(assets.image(names[i]),100+count*10,100+i*40);
  frame.blit(assets.image("arrow.app"),60,100+choice_*40);
  bool enter=input.read(0x46d4ec)!=0;
  if(enter)enter_=true;
  if(enter_&&!enter){enter_=false;return true;}
  bool up=input.read(0x46d4b0)!=0,down=input.read(0x46d4b4)!=0;
  if(up&&!up_&&choice_>0)--choice_;
  if(down&&!down_&&choice_<7)++choice_;
  up_=up;down_=down;return false;
 }
};
}
