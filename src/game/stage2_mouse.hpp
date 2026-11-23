#pragma once
#include "player_object_response.hpp"
#include "collision_world.hpp"
#include "../assets/asset_store.hpp"
namespace shin::game {
// Stage2 MAP31, actor70: recovered 4423e0/4424f0/442780/4427d0.
class Stage2Mouse {
 void pose(int state){auto&e=payload;e.at(0x58)=state;e.at(0x18)=e.at(0x1c)=0;e.at(0x14)=state==0?0x4701d0:state==3?0x4701f8:state>=4?0x4701e8:0x4701d8;e.at(0x20)=state==0?1:4;e.at(0x24)=state==0?100:state==3?10:5;}
public:
 std::vector<std::string> sound_requests;
 EntityPayload payload;bool respawn_requested=false,defeated=false;
 Stage2Mouse(int x,int y,AssetStore&a){auto&e=payload;e.kind=0x70;e.at(4)=e.at(0x48)=x;e.at(8)=e.at(0x4c)=y;e.at(0x28)=13;e.at(0x2c)=35;e.at(0x30)=59;e.at(0x34)=69;const auto&last=a.image(names[11]);e.at(0xc)=last.width;e.at(0x10)=last.height;pose(1);}
 static inline const char*names[]={"cht11aa.raw","cht11ab.raw","cht11ac.raw","cht11ad.raw","cht11ba.raw","cht11bb.raw","cht11bc.raw","cht11bd.raw","cht11ca.raw","cht11cb.raw","cht11cc.raw","cht11cd.raw"};
 static inline const int indices[]={0,0,0,1,2,3,8,9,10,11,4,5,6,7};
 void hurt(){if(payload.at(0x58)!=3){pose(3);sound_requests.push_back("RESOURCE/mouse1.wav");}}
 void platform(int direction){if(payload.at(0x58)==3)return;if(direction==0)pose(0);else if(direction==1)pose(1);else if(direction==2)pose(2);}
 void tick(int cx,int cy,AssetStore&a,IndexedSurface&frame){auto&e=payload;if(e.state==2)return;
 if(std::abs(e.at(4)-cx-320)>960){respawn_requested=true;e.state=2;}
 e.at(0x48)=e.at(4);e.at(0x4c)=e.at(8);
 switch(e.at(0x58)){
 case 1:if(++e.at(0x6c)>20){e.at(0x6c)=0;pose(4);}e.at(0x50)=-4;e.at(0x68)=0;break;
 case 2:if(++e.at(0x70)>20){e.at(0x70)=0;pose(5);}e.at(0x50)=4;e.at(0x68)=1;break;
 case 3:if(e.at(0x18)==0&&e.at(0x1c)==0)e.at(0x54)=-10;if(e.at(0x18)==1&&e.at(0x1c)==0){e.at(0x24)=10;e.at(0x54)=10;e.state=2;defeated=true;}break;
 case 4:e.at(0x50)=-6;e.at(0x68)=0;break;case 5:e.at(0x50)=6;e.at(0x68)=1;break;}
 int table=(e.at(0x14)-0x4701d0)/4;frame.blit(a.image(names[indices[table+e.at(0x18)]]),e.at(4)-cx,e.at(8)-cy,true,0,0,-1,-1,e.at(0x68)!=0);
 if(e.at(0x50)>0)--e.at(0x50);if(e.at(0x50)<0)++e.at(0x50);if(e.at(0x54)<11)++e.at(0x54);e.at(4)+=e.at(0x50);e.at(8)+=e.at(0x54);
 if(++e.at(0x1c)>e.at(0x24)){e.at(0x1c)=0;if(++e.at(0x18)>=e.at(0x20))e.at(0x18)=0;}
 }
 void terrain(const CollisionWorld&world,int param_2){auto&e=payload;int iVar1=0;auto probe=[&](int,int x,int y){return world.body({e.at(0x28),e.at(0x2c),e.at(0x30),e.at(0x34)},x,y);};

  int iVar2;
  int iVar3;
  int iVar4;
  
                    /* 0x427d0  492  callback_004427d0 */

  if (param_2 != 1) {
    if (param_2 != 2) {
      return;
    }
    iVar4 = 0;
    do {
      iVar3 = probe(iVar1 + 0x28,e.at(4),e.at(8) - iVar4);
      if (iVar3 == 0) {
        e.at(8) = e.at(8) - iVar4;
        e.at(0x54) = e.at(0x54) + 2;
        return;
      }
      iVar4 = iVar4 + 1;
    } while (iVar4 < 0x1a);
    iVar4 = e.at(8);
    if (iVar4 <= e.at(0x4c)) {
      return;
    }
    if (iVar4 < e.at(0x4c)) {
      return;
    }
    do {
      iVar3 = probe(iVar1 + 0x28,e.at(4),iVar4);
      if (iVar3 == 0) {
        e.at(8) = iVar4;
        return;
      }
      iVar4 = iVar4 + -1;
    } while (e.at(0x4c) <= iVar4);
    return;
  }
  iVar3 = 0;
  iVar4 = iVar1 + 0x28;
  do {
    iVar2 = probe(iVar4,e.at(4),e.at(8) - iVar3);
    if (iVar2 == 0) {
      e.at(8) = e.at(8) - iVar3;
      return;
    }
    iVar3 = iVar3 + 1;
  } while (iVar3 < 0x1a);
  iVar3 = e.at(0x48);
  iVar2 = e.at(4);
  if (iVar3 < iVar2) {
    if (iVar3 <= iVar2) {
      do {
        iVar3 = probe(iVar4,iVar2,e.at(0x4c));
        if (iVar3 == 0) goto LAB_00442861;
        iVar2 = iVar2 + -1;
      } while (e.at(0x48) <= iVar2);
    }
  }
  else if ((iVar2 < iVar3) && (iVar2 <= iVar3)) {
    do {
      iVar3 = probe(iVar4,iVar2,e.at(0x4c));
      if (iVar3 == 0) goto LAB_00442861;
      iVar2 = iVar2 + 1;
    } while (iVar2 <= e.at(0x48));
  }
LAB_00442864:
  iVar3 = e.at(0x4c);
  iVar2 = e.at(8);
  if (iVar3 < iVar2) {
    if (iVar3 <= iVar2) {
      do {
        iVar3 = probe(iVar4,e.at(4),iVar2);
        if (iVar3 == 0) {
          e.at(8) = iVar2;
          return;
        }
        iVar2 = iVar2 + -1;
      } while (e.at(0x4c) <= iVar2);
      return;
    }
  }
  else if ((iVar2 < iVar3) && (iVar2 <= iVar3)) {
    do {
      iVar3 = probe(iVar4,e.at(4),iVar2);
      if (iVar3 == 0) {
        e.at(8) = iVar2;
        return;
      }
      iVar2 = iVar2 + 1;
    } while (iVar2 <= e.at(0x4c));
    return;
  }
  return;
LAB_00442861:
  e.at(4) = iVar2;
  goto LAB_00442864;

 }
};
}
