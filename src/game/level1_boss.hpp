#pragma once
#include "player_object_response.hpp"
#include "collision_world.hpp"
#include "../assets/asset_store.hpp"
namespace shin::game {
struct BossFrame {bool completed=false;unsigned sound=0;};
class FirstBoss {
 bool alternate_sound_=false;
 void pose(int n){auto&e=payload;e.at(0x10)=e.at(0x14)=0;e.at(0x50)=n;e.at(0x18)=n==3?6:n==4?1:4;e.at(0x1c)=n==4?20:10;e.at(0xc)=n==3?0x4610e8:n==4?0x461100:0x4610d8;}
public:
 EntityPayload payload;
 FirstBoss(int x,int y,int width,int height){auto&e=payload;e.kind=0x74;e.at(4)=e.at(0x40)=x;e.at(8)=e.at(0x44)=y;e.at(0x20)=48;e.at(0x24)=6;e.at(0x28)=100;e.at(0x2c)=176;e.at(0x6c)=width;e.at(0x70)=height;e.at(0x74)=10;pose(1);}
 unsigned hurt(){pose(4);--payload.at(0x74);alternate_sound_=!alternate_sound_;return alternate_sound_?0x4788fc:0x478764;}
 BossFrame update(int player_x,int player_width){
  auto&e=payload;BossFrame out;e.at(0x40)=e.at(4);e.at(0x44)=e.at(8);int state=e.at(0x50),f=e.at(0x10),tick=e.at(0x14);
  if(state==1||state==2){if(e.at(0x60)>200){e.at(0x64)=state;pose(3);e.at(0x60)=0;}e.at(0x48)=state==1?-2:2;e.at(0x68)=state==1?0:1;}
  else if(state==3){if(f==0&&tick==1){bool left=player_x+player_width/2<e.at(4)+e.at(0x6c)/2;e.at(0x68)=left?0:1;e.at(0x48)=left?-10:10;}if(f==5&&tick==9)pose(player_x+player_width/2<e.at(4)+e.at(0x6c)/2?1:2);}
  else if(state==4){if(f==0&&tick==0&&e.at(0x74)==0)e.at(0x4c)=-10;if(f==0&&tick==9){if(e.at(0x74)==0){payload.state=2;out.completed=true;return out;}e.at(0x64)=4;pose(3);}}
  if(e.at(0x48)>0)--e.at(0x48);if(e.at(0x48)<0)++e.at(0x48);if(e.at(0x4c)<11)++e.at(0x4c);e.at(4)+=e.at(0x48);e.at(8)+=e.at(0x4c);
  if(++e.at(0x14)>e.at(0x1c)){e.at(0x14)=0;if(++e.at(0x10)>=e.at(0x18))e.at(0x10)=0;}++e.at(0x60);return out;
 }
 void terrain(const CollisionWorld&world,int param_2){auto&e=payload;CollisionRect rect={e.at(0x20),e.at(0x24),e.at(0x28),e.at(0x2c)};auto probe=[&](int,int x,int y){return world.body(rect,x,y);};

  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  
                    /* 0x224b0  229  callback_004224b0 */
  iVar1 = 0;
  if (param_2 != 1) {
    if (param_2 != 2) {
      return;
    }
    iVar4 = 0;
    do {
      iVar3 = probe(iVar1 + 0x20,e.at(4),e.at(8) - iVar4);
      if (iVar3 == 0) {
        e.at(8) = e.at(8) - iVar4;
        e.at(0x4c) = e.at(0x4c) + 2;
        return;
      }
      iVar4 = iVar4 + 1;
    } while (iVar4 < 0x1a);
    iVar4 = e.at(8);
    if (iVar4 <= e.at(0x44)) {
      return;
    }
    do {
      iVar3 = probe(iVar1 + 0x20,e.at(4),iVar4);
      if (iVar3 == 0) {
        e.at(8) = iVar4;
        return;
      }
      iVar4 = iVar4 + -1;
    } while (e.at(0x44) <= iVar4);
    return;
  }
  iVar3 = 0;
  iVar4 = iVar1 + 0x20;
  do {
    iVar2 = probe(iVar4,e.at(4),e.at(8) - iVar3);
    if (iVar2 == 0) {
      e.at(8) = e.at(8) - iVar3;
      return;
    }
    iVar3 = iVar3 + 1;
  } while (iVar3 < 0x1a);
  iVar3 = e.at(4);
  if (e.at(0x40) < iVar3) {
    do {
      iVar2 = probe(iVar4,iVar3,e.at(0x44));
      if (iVar2 == 0) goto LAB_00422539;
      iVar3 = iVar3 + -1;
    } while (e.at(0x40) <= iVar3);
  }
  else if (iVar3 < e.at(0x40)) {
    do {
      iVar2 = probe(iVar4,iVar3,e.at(0x44));
      if (iVar2 == 0) goto LAB_00422539;
      iVar3 = iVar3 + 1;
    } while (iVar3 <= e.at(0x40));
  }
LAB_0042253c:
  iVar3 = e.at(8);
  if (e.at(0x44) < iVar3) {
    do {
      iVar2 = probe(iVar4,e.at(4),iVar3);
      if (iVar2 == 0) {
        e.at(8) = iVar3;
        return;
      }
      iVar3 = iVar3 + -1;
    } while (e.at(0x44) <= iVar3);
    return;
  }
  if (e.at(0x44) <= iVar3) {
    return;
  }
  do {
    iVar2 = probe(iVar4,e.at(4),iVar3);
    if (iVar2 == 0) {
      e.at(8) = iVar3;
      return;
    }
    iVar3 = iVar3 + 1;
  } while (iVar3 <= e.at(0x44));
  return;
LAB_00422539:
  e.at(4) = iVar3;
  goto LAB_0042253c;

 }
 void render(AssetStore&assets,IndexedSurface&frame,int camera_x,int camera_y){
  static const char*names[]={
#include "boss_assets.inc"
  };static const int sequence[]={1,2,3,4,6,7,6,7,6,7,5};
  int i=(payload.at(0xc)-0x4610d8)/4+payload.at(0x10);if(i<0||i>=11)throw std::out_of_range("boss animation");
  frame.blit(assets.image(names[sequence[i]]),payload.at(4)-camera_x,payload.at(8)-camera_y,true,0,0,-1,-1,payload.at(0x68)!=0);
  frame.blit(assets.image("ryoko_h.app"),10,50);for(int n=0;n<payload.at(0x74);++n)frame.blit(assets.image("rlife.app"),50+n*20,50);
 }
};
}
