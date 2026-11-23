#pragma once
#include "player_object_response.hpp"
#include "collision_world.hpp"
#include "../assets/asset_store.hpp"
#include <cstdlib>
namespace shin::game {
class Stage2Boss {
public:
 struct ProjectileEvent {int x,y,vx,vy;unsigned callback;};
 EntityPayload payload;bool respawn_requested=false,defeated=false;std::vector<ProjectileEvent>projectiles;std::vector<int>spawn_requests;std::vector<std::string>sound_requests;
static inline const char*names[]={"cht141aa.raw","cht141ab.raw","cht141ac.raw","cht141ad.raw","cht141ba.raw","cht141bb.raw","cht141bc.raw","cht141bd.raw","cht143aa.raw","cht143ab.raw","cht143ac.raw","cht143ad.raw","cht143ba.raw","cht143bb.raw","cht143bc.raw","cht143bd.raw","cht144aa.raw","cht144ab.raw","cht144ac.raw"};
static inline const int indices[]={4,0,1,2,3,4,5,6,7,8,9,10,11,16,17,18,0,12,13,14,15};
Stage2Boss(int x,int y,AssetStore&a){auto&e=payload;e.kind=115;e.at(4)=e.at(0x40)=x;e.at(8)=e.at(0x44)=y;e.at(32)=38;e.at(36)=10;e.at(40)=113;e.at(44)=189;const auto&last=a.image(names[18]);e.at(100)=last.width;e.at(104)=last.height;h4385e0();e.at(0x6c)=10;spawn_requests.push_back(0x31);}
void h4385b0(){auto&e=payload;
  e.at(0x50) = 0;
  e.at(0x10) = 0;
  e.at(0xc) = 0x46e344;
  e.at(0x18) = 1;
  e.at(0x1c) = 100;
  e.at(0x14) = 0;
  return;
}
void h4385e0(){auto&e=payload;
  e.at(0x10) = 0;
  e.at(0x14) = 0;
  e.at(0x50) = 1;
  e.at(0xc) = 0x46e358;
  e.at(0x18) = 4;
  e.at(0x1c) = 10;
  return;
}
void h438610(){auto&e=payload;
  e.at(0x10) = 0;
  e.at(0x14) = 0;
  e.at(0x50) = 2;
  e.at(0xc) = 0x46e358;
  e.at(0x18) = 4;
  e.at(0x1c) = 10;
  return;
}
void h438640(){auto&e=payload;
  int uVar1;
  
  e.at(0xc) = 0x46e378;
  e.at(0x18) = 3;
  e.at(0x1c) = 10;
  uVar1 = e.at(0x50);
  e.at(0x50) = 6;
  e.at(0x5c) = uVar1;
  e.at(0x10) = 0;
  e.at(0x14) = 0;
  return;
}
void h438670(){auto&e=payload;
  e.at(0x10) = 0;
  e.at(0x14) = 0;
  e.at(0x50) = 7;
  e.at(0xc) = 0x46e388;
  e.at(0x18) = 4;
  e.at(0x1c) = 0x14;
sound_requests.push_back("RESOURCE/dem-tib.wav");}
void hurt(){h438670();}
void platform(int d){if(payload.at(0x50)==7)return;if(d>=0&&d<=2){switch(d){case 0:h4385b0();break;case 1:h4385e0();break;case 2:h438610();break;}}}
bool cinematic=false;
 void cinematic_pose(int state){auto&e=payload;e.at(0x50)=state;e.at(0x10)=e.at(0x14)=0;e.at(0xc)=state==8?0x46e3d0:0x46e3d8;e.at(0x18)=state==8?2:1;e.at(0x1c)=10;}
 void cinematic_tick(int cx,int cy,AssetStore&a,IndexedSurface&frame){auto&e=payload;e.at(0x48)=0;e.at(0x38)=e.at(4)-cx;e.at(0x3c)=e.at(8)-cy;frame.blit(a.image(e.at(0x50)==8&&e.at(0x10)==1?"2#422.app":"2#421.app"),e.at(0x38),e.at(0x3c));if(e.at(0x4c)<11)++e.at(0x4c);e.at(8)+=e.at(0x4c);if(++e.at(0x14)>e.at(0x1c)){e.at(0x14)=0;if(++e.at(0x10)>=e.at(0x18))e.at(0x10)=0;}}
void tick(int cx,int cy,int px,int pw,AssetStore&a,IndexedSurface&frame){if(payload.state==2)return;auto&e=payload;if(cinematic){cinematic_tick(cx,cy,a,frame);return;}
  int iVar1=0;
  int iVar2;
  int iVar3;
  
  iVar1 = e.at(0xc);
  e.at(0x40) = e.at(4);
  e.at(0x44) = e.at(8);
  switch(e.at(0x50)) {
  case 0:
    if (200 < e.at(0x58)) {
      h438640();
      e.at(0x58) = 0;
    }
    break;
  case 1:
    if (200 < e.at(0x58)) {
      h438640();
      e.at(0x58) = 0;
    }
    e.at(0x48) = 0xfffffffe;
    e.at(0x60) = 0;
    break;
  case 2:
    if (200 < e.at(0x58)) {
      h438640();
      e.at(0x58) = 0;
    }
    e.at(0x48) = 2;
    e.at(0x60) = 1;
    break;
  case 6:
    if (((e.at(0x10) == 0) && (e.at(0x14) == 1)) &&
       (iVar3 = pw, iVar2 = px,
       e.at(0x60) = 0,
       e.at(4) + e.at(100) / 2 <= iVar2 + iVar3 / 2)) {
      e.at(0x60) = 1;
    }
    if ((e.at(0x10) == 2) && (e.at(0x14) == 9)) {
      iVar3 = e.at(4);
      iVar2 = px - iVar3;
      iVar2 = (int)(iVar2 + (iVar2 >> 0x1f & 0x1fU)) >> 5;
      if (8 < iVar2) {
        iVar2 = 7;
      }
      if (iVar2 < -8) {
        iVar2 = -7;
      }
      if (e.at(0x60) != 0) {
        iVar3 = e.at(100) + iVar3 + -10;
      }
      projectiles.push_back({iVar3,e.at(8) + 0x1e,iVar2
                   ,-16,0x402440});
      iVar3 = e.at(0x5c);
      if (iVar3 == 0) {
        h4385b0();
      }
      else if (iVar3 == 1) {
        h4385e0();
      }
      else if (iVar3 == 2) {
        h438610();
      }
      else if (px < e.at(4)) {
        h4385e0();
      }
      else {
        h438610();
      }
    }
    break;
  case 7:
    if ((e.at(0x10) == 0) && (e.at(0x14) == 0)) {
      e.at(0x4c) = 0xfffffff6;
    }
    if ((e.at(0x10) == 1) && (e.at(0x14) == 0)) {
      iVar3 = e.at(0x6c) + -1;
      e.at(0x6c) = iVar3;
      if (iVar3 < 1) {
        e.at(0x1c) = 10;
        e.at(0x4c) = 10;
        spawn_requests.push_back(0x2d);
        payload.state=2;
        defeated=true;
      }
      else {
        h438640();
      }
    }
  }
  e.at(0x38) = e.at(4) - cx;
  iVar3 = e.at(8) - cy;
  e.at(0x3c) = iVar3;
  frame.blit(a.image(names[indices[(e.at(0xc)-4645700)/4+e.at(0x10)]]),e.at(4)-cx,e.at(8)-cy,true,0,0,-1,-1,e.at(0x60)!=0);

  frame.blit(a.image("bsfc21.app"),10,50);for(int hp=0;hp<e.at(0x6c);++hp)frame.blit(a.image("rlife.app"),50+20*hp,50);
  if (0 < e.at(0x48)) {
    e.at(0x48) = e.at(0x48) + -1;
  }
  if (e.at(0x48) < 0) {
    e.at(0x48) = e.at(0x48) + 1;
  }
  if (e.at(0x4c) < 0xb) {
    e.at(0x4c) = e.at(0x4c) + 1;
  }
  e.at(4) = e.at(4) + e.at(0x48);
  e.at(8) = e.at(8) + e.at(0x4c);
  iVar3 = e.at(0x14) + 1;
  e.at(0x14) = iVar3;
  if (e.at(0x1c) < iVar3) {
    e.at(0x14) = 0;
    iVar3 = e.at(0x10) + 1;
    e.at(0x10) = iVar3;
    if (e.at(0x18) + -1 < iVar3) {
      e.at(0x10) = 0;
    }
  }
  e.at(0x58) = e.at(0x58) + 1;
  return;
}
void terrain(const CollisionWorld&world,int param_2){auto&e=payload;auto probe=[&](int,int x,int y){return world.body({e.at(0x20),e.at(0x24),e.at(0x28),e.at(0x2c)},x,y);};
  int iVar1=0;
  int iVar2;
  int iVar3;
  int iVar4;
  
                    
  iVar1 = e.at(0xc);
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
  iVar3 = e.at(0x40);
  iVar2 = e.at(4);
  if (iVar3 < iVar2) {
    if (iVar3 <= iVar2) {
      do {
        iVar3 = probe(iVar4,iVar2,e.at(0x44));
        if (iVar3 == 0) goto LAB_00438501;
        iVar2 = iVar2 + -1;
      } while (e.at(0x40) <= iVar2);
    }
  }
  else if ((iVar2 < iVar3) && (iVar2 <= iVar3)) {
    do {
      iVar3 = probe(iVar4,iVar2,e.at(0x44));
      if (iVar3 == 0) goto LAB_00438501;
      iVar2 = iVar2 + 1;
    } while (iVar2 <= e.at(0x40));
  }
LAB_00438504:
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
LAB_00438501:
  e.at(4) = iVar2;
  goto LAB_00438504;
}
};
}
