#pragma once
#include "stage2_mouse.hpp"
namespace shin::game {
// MAP25: constructor41af40, update41b040, hurt41b4b0.
class Stage2RemainingEnemy {
 int type_;
 static constexpr const char* names[]={"cht121aa.app","cht121ab.app","cht121ac.app","cht121ad.app","cht121ae.app","cht121af.app","cht121ag.app","cht121ah.app","cht121ba.app","cht121bb.app","cht121bc.app","cht121bd.app"};
 static constexpr int sequence[]={8,9,10,11,4,5,6,7};
 void pose(int state){auto&e=payload;e.at(0x58)=state;e.at(0x18)=e.at(0x1c)=0;e.at(0x20)=4;e.at(0x24)=state==3?10:10;}
public:
 struct ProjectileEvent{int x,y,vx,vy;unsigned callback;};
 EntityPayload payload;bool respawn_requested=false,defeated=false;
 std::vector<std::string>sound_requests;std::vector<ProjectileEvent>projectiles;
 static bool supports(int t){return t==0x25;}
 static std::string image_name(int,int i){return names[sequence[i]];}
 Stage2RemainingEnemy(int type,int x,int y,AssetStore&a):type_(type){if(!supports(type))throw std::invalid_argument("map25 type");auto&e=payload;e.kind=0x71;e.at(4)=e.at(0x48)=x;e.at(8)=e.at(0x4c)=y;auto&im=a.image(names[11]);e.at(0xc)=im.width;e.at(0x10)=im.height;e.at(0x28)=22;e.at(0x2c)=33;e.at(0x30)=59;e.at(0x34)=193;pose(1);}
 void hurt(){if(payload.at(0x58)!=3){pose(3);sound_requests.push_back(type_==0x1d?"RESOURCE/dem-h1.wav":"RESOURCE/dem-h1.wav");}}
 void platform(int direction){if(payload.at(0x58)!=3&&(direction==1||direction==2))pose(direction);}
 void tick(int cx,int cy,AssetStore&a,IndexedSurface&frame){auto&e=payload;if(e.state==2)return;
 if(std::abs(e.at(4)-cx-320)>960){respawn_requested=true;e.state=2;}
 e.at(0x48)=e.at(4);e.at(0x4c)=e.at(8);
 if(e.at(0x58)==1){e.at(0x50)=-2;e.at(0x68)=0;}else if(e.at(0x58)==2){e.at(0x50)=2;e.at(0x68)=1;}else if(e.at(0x58)==3){if(e.at(0x18)==0&&e.at(0x1c)==0)e.at(0x54)=-10;if(e.at(0x18)==1&&e.at(0x1c)==0){e.at(0x24)=10;e.at(0x54)=10;e.state=2;defeated=true;}if(e.at(0x18)==1&&e.at(0x1c)==9)e.state=2;}
 int index=e.at(0x18)+(e.at(0x58)==3?4:0);frame.blit(a.image(image_name(type_,index)),e.at(4)-cx,e.at(8)-cy,true,0,0,-1,-1,e.at(0x68)!=0);
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
