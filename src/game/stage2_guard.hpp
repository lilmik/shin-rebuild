#pragma once
#include "player_object_response.hpp"
#include "collision_world.hpp"
#include "../assets/asset_store.hpp"
#include <cstdlib>
namespace shin::game {
class Stage2Guard {
public:
 struct ProjectileEvent {int x,y,vx,vy;unsigned callback;};
 EntityPayload payload;bool respawn_requested=false,defeated=false;std::vector<ProjectileEvent>projectiles;std::vector<int>spawn_requests;std::vector<std::string>sound_requests;
static inline const char*names[]={"cht122aa.raw","cht122ab.raw","cht122ac.raw","cht122ad.raw","cht122ba.raw","cht122bb.raw","cht122bc.raw","cht122bd.raw","cht122ca.raw","cht122cb.raw","cht122cc.raw","cht122da.raw","cht122db.raw","cht122dc.raw","cht122dd.raw"};
static inline const int indices[]={0,1,2,3,4,5,6,7,8,9,10,0,11,12,13,14};
Stage2Guard(int x,int y,AssetStore&a){auto&e=payload;e.kind=114;e.at(4)=e.at(0x40)=x;e.at(8)=e.at(0x44)=y;e.at(32)=28;e.at(36)=14;e.at(40)=46;e.at(44)=71;const auto&last=a.image(names[14]);e.at(100)=last.width;e.at(104)=last.height;h44d190();}
void h44d160(){auto&e=payload;
  e.at(0x50) = 0;
  e.at(0x10) = 0;
  e.at(0xc) = 0x4718b0;
  e.at(0x18) = 4;
  e.at(0x1c) = 100;
  e.at(0x14) = 0;
  return;
}
void h44d190(){auto&e=payload;
  e.at(0x10) = 0;
  e.at(0x14) = 0;
  e.at(0x50) = 1;
  e.at(0xc) = 0x4718c0;
  e.at(0x18) = 4;
  e.at(0x1c) = 10;
  return;
}
void h44d1c0(){auto&e=payload;
  e.at(0x10) = 0;
  e.at(0x14) = 0;
  e.at(0x50) = 2;
  e.at(0xc) = 0x4718c0;
  e.at(0x18) = 4;
  e.at(0x1c) = 10;
  return;
}
void h44d1f0(){auto&e=payload;
  int uVar1;
  
  e.at(0xc) = 0x4718d0;
  e.at(0x18) = 3;
  e.at(0x1c) = 10;
  uVar1 = e.at(0x50);
  e.at(0x50) = 3;
  e.at(0x5c) = uVar1;
  e.at(0x10) = 0;
  e.at(0x14) = 0;
  return;
}
void h44d220(){auto&e=payload;
  e.at(0x50) = 4;
  e.at(0x10) = 0;
  e.at(0x14) = 0;
  e.at(0xc) = 0x4718e0;
  e.at(0x1c) = 0x14;
  e.at(0x18) = 4;
sound_requests.push_back("RESOURCE/dem-h2.wav");}
void hurt(){h44d220();}
void platform(int d){if(payload.at(0x50)==4)return;if(d>=0&&d<=2){switch(d){case 0:h44d160();break;case 1:h44d190();break;case 2:h44d1c0();break;}}}
void tick(int cx,int cy,int px,int pw,AssetStore&a,IndexedSurface&frame){if(payload.state==2)return;auto&e=payload;
  int iVar1=0;
  unsigned uVar2;
  int iVar3;
  int iVar4;
  unsigned uVar5;
  
                    
  iVar1 = e.at(0xc);
  uVar2 = (e.at(4) - cx) - 0x140;
  uVar5 = (int)uVar2 >> 0x1f;
  iVar3 = (uVar2 ^ uVar5) - uVar5;
  if (0x3c0 < iVar3) {
    respawn_requested=true;
    payload.state=2;
  }
  e.at(0x40) = e.at(4);
  e.at(0x44) = e.at(8);
  switch(e.at(0x50)) {
  case 0:
    if (e.at(0x58) == 200) {
      h44d1f0();
      e.at(0x58) = 0;
    }
    break;
  case 1:
    if (e.at(0x58) == 200) {
      h44d1f0();
      e.at(0x58) = 0;
    }
    e.at(0x48) = 0xfffffffe;
    e.at(0x60) = 0;
    break;
  case 2:
    if (e.at(0x58) == 200) {
      h44d1f0();
      e.at(0x58) = 0;
    }
    e.at(0x48) = 2;
    e.at(0x60) = 1;
    break;
  case 3:
    if (((e.at(0x10) == 0) && (e.at(0x14) == 1)) &&
       (iVar3 = pw, iVar4 = px,
       e.at(0x60) = 0,
       e.at(4) + e.at(100) / 2 <= iVar4 + iVar3 / 2)) {
      e.at(0x60) = 1;
    }
    if ((e.at(0x10) == 2) && (e.at(0x14) == 9)) {
      iVar3 = e.at(4);
      iVar4 = px - iVar3;
      iVar4 = (int)(iVar4 + (iVar4 >> 0x1f & 0x1fU)) >> 5;
      if (8 < iVar4) {
        iVar4 = 7;
      }
      if (iVar4 < -8) {
        iVar4 = -7;
      }
      if (e.at(0x60) != 0) {
        iVar3 = e.at(100) + iVar3 + -10;
      }
      projectiles.push_back({iVar3,e.at(8) + 0x1e,iVar4
                   ,-16,0x4366b0});
      iVar3 = e.at(0x5c);
      if (iVar3 == 0) {
        h44d160();
      }
      else if (iVar3 == 1) {
        h44d190();
      }
      else if (iVar3 == 2) {
        h44d1c0();
      }
    }
    break;
  case 4:
    if ((e.at(0x10) == 0) && (e.at(0x14) == 0)) {
      e.at(0x4c) = 0xfffffff6;
    }
    if (e.at(0x10) == 1) {
      if (e.at(0x14) == 0) {
        e.at(0x1c) = 10;
        e.at(0x4c) = 10;
        payload.state=2;
        defeated=true;
      }
      if ((e.at(0x10) == 1) && (e.at(0x14) == 9)) {
        payload.state=2;
      }
    }
  }
  e.at(0x38) = e.at(4) - cx;
  iVar3 = e.at(8) - cy;
  e.at(0x3c) = iVar3;
  frame.blit(a.image(names[indices[(e.at(0xc)-4659376)/4+e.at(0x10)]]),e.at(4)-cx,e.at(8)-cy,true,0,0,-1,-1,e.at(0x60)!=0);

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
  iVar3 = e.at(4);
  if (e.at(0x40) < iVar3) {
    do {
      iVar2 = probe(iVar4,iVar3,e.at(0x44));
      if (iVar2 == 0) goto LAB_0044d0a9;
      iVar3 = iVar3 + -1;
    } while (e.at(0x40) <= iVar3);
  }
  else if (iVar3 < e.at(0x40)) {
    do {
      iVar2 = probe(iVar4,iVar3,e.at(0x44));
      if (iVar2 == 0) goto LAB_0044d0a9;
      iVar3 = iVar3 + 1;
    } while (iVar3 <= e.at(0x40));
  }
LAB_0044d0ac:
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
LAB_0044d0a9:
  e.at(4) = iVar3;
  goto LAB_0044d0ac;
}
};
}
