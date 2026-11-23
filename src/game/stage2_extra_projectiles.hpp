#pragma once
#include "player_object_response.hpp"
#include "collision_world.hpp"
#include "../assets/asset_store.hpp"
namespace shin::game {
// Recovered 44a040/44a130/44a340: flame projectile, actor kind 5d.
class Stage2ExtraProjectiles {
 AssetStore&assets_;CollisionWorld world_;
 static constexpr const char* images[]={"fballaa.app","fballba.app","fballca.app","fballda.app"};
 static constexpr int sequence[]={0,1,2,3};
 void render(EntityPayload&p,IndexedSurface&f){int index=sequence[p.at(0x18)%4];f.blit(assets_.image(images[index]),p.at(0x40),p.at(0x44),true,0,0,-1,-1,p.at(0x68)!=0);}
 void terrain(EntityPayload&p,int param_2){auto probe=[&](int x,int y){return world_.body({10,10,40,40},x,y);};
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  
                    /* 0x4a340  567  callback_0044a340 */

  if (param_2 != 1) {
    if (param_2 != 2) {
      return;
    }
    iVar4 = 0;
    do {
      iVar3 = probe(p.at(4),p.at(8) - iVar4);
      if (iVar3 == 0) {
        p.at(8) = p.at(8) - iVar4;
        p.at(0x54) = p.at(0x54) + 2;
        return;
      }
      iVar4 = iVar4 + 1;
    } while (iVar4 < 0x1a);
    iVar4 = p.at(8);
    if (iVar4 <= p.at(0x4c)) {
      return;
    }
    if (iVar4 < p.at(0x4c)) {
      return;
    }
    do {
      iVar3 = probe(p.at(4),iVar4);
      if (iVar3 == 0) {
        p.at(8) = iVar4;
        return;
      }
      iVar4 = iVar4 + -1;
    } while (p.at(0x4c) <= iVar4);
    return;
  }
  iVar3 = 0;
  iVar4 = 0;
  do {
    iVar2 = probe(p.at(4),p.at(8) - iVar3);
    if (iVar2 == 0) {
      p.at(8) = p.at(8) - iVar3;
      return;
    }
    iVar3 = iVar3 + 1;
  } while (iVar3 < 0x1a);
  iVar3 = p.at(0x48);
  iVar2 = p.at(4);
  if (iVar3 < iVar2) {
    if (iVar3 <= iVar2) {
      do {
        iVar3 = probe(iVar2,p.at(0x4c));
        if (iVar3 == 0) goto LAB_0044a3d1;
        iVar2 = iVar2 + -1;
      } while (p.at(0x48) <= iVar2);
    }
  }
  else if ((iVar2 < iVar3) && (iVar2 <= iVar3)) {
    do {
      iVar3 = probe(iVar2,p.at(0x4c));
      if (iVar3 == 0) goto LAB_0044a3d1;
      iVar2 = iVar2 + 1;
    } while (iVar2 <= p.at(0x48));
  }
LAB_0044a3d4:
  iVar3 = p.at(0x4c);
  iVar2 = p.at(8);
  if (iVar3 < iVar2) {
    if (iVar3 <= iVar2) {
      do {
        iVar3 = probe(p.at(4),iVar2);
        if (iVar3 == 0) {
          p.at(8) = iVar2;
          return;
        }
        iVar2 = iVar2 + -1;
      } while (p.at(0x4c) <= iVar2);
      return;
    }
  }
  else if ((iVar2 < iVar3) && (iVar2 <= iVar3)) {
    do {
      iVar3 = probe(p.at(4),iVar2);
      if (iVar3 == 0) {
        p.at(8) = iVar2;
        return;
      }
      iVar2 = iVar2 + 1;
    } while (iVar2 <= p.at(0x4c));
    return;
  }
  return;
LAB_0044a3d1:
  p.at(4) = iVar2;
  goto LAB_0044a3d4;
}
public:
 std::vector<EntityPayload> targets;
 Stage2ExtraProjectiles(AssetStore&a,const MapData&m,const AppImage&b):assets_(a),world_(m,b){}
 void spawn(int x,int y,int vx,int vy){EntityPayload p;p.kind=0x5d;p.state=1;p.at(4)=p.at(0x48)=x;p.at(8)=p.at(0x4c)=y;p.at(0x50)=vx;p.at(0x54)=vy;p.at(0x28)=p.at(0x2c)=10;p.at(0x30)=p.at(0x34)=40;p.at(0x58)=1;p.at(0x20)=4;p.at(0x24)=4;auto&im=assets_.image(images[3]);p.at(12)=im.width;p.at(16)=im.height;targets.push_back(p);}
 void tick(int cx,int cy,IndexedSurface&f){for(auto&p:targets){if(p.state==2)continue;if(p.state==1){p.state=0;continue;} update(p,cx,cy,f);int material=world_.body({10,10,40,40},p.at(4),p.at(8));if(material)terrain(p,material);}targets.erase(std::remove_if(targets.begin(),targets.end(),[](auto&p){return p.state==2;}),targets.end());}
 void update(EntityPayload&p,int cx,int cy,IndexedSurface&f){
  int iVar1;
  int iVar2;
  unsigned uVar3;
  int iVar4;
  unsigned uVar5;
  int uVar6;
  int uVar7;
  int uVar8;
  int uVar9;
  int uVar10;
  
                    /* 0x4a130  564  callback_0044a130 */

  uVar3 = (p.at(4) - cx) - 0x140;
  uVar5 = (int)uVar3 >> 0x1f;
  if (0x3c0 < (int)((uVar3 ^ uVar5) - uVar5)) {
    p.state = 2;
  }
  p.at(0x48) = p.at(4);
  p.at(0x4c) = p.at(8);
  iVar4 = p.at(0x58);
  if (iVar4 == 1) {
    p.at(0x50) = 0xfffffffc;
    p.at(0x68) = 0;
  }
  else if (iVar4 == 2) {
    p.at(0x50) = 4;
    p.at(0x68) = 1;
  }
  else if (iVar4 == 3) {
    if ((p.at(0x18) == 0) && (p.at(0x1c) == 0)) {
      p.at(0x54) = 0xfffffff6;
    }
    if (p.at(0x18) == 1) {
      if (p.at(0x1c) == 0) {
        p.at(0x24) = 10;
        p.at(0x54) = 10;
        p.state = 2;
      }
      if ((p.at(0x18) == 1) && (p.at(0x1c) == 9)) {
        p.state = 2;
      }
    }
  }
  p.at(0x40) = p.at(4) - cx;
  iVar4 = p.at(8) - cy;
  p.at(0x44) = iVar4;
  render(p, f);
  if (0 < p.at(0x50)) {
    p.at(0x50) = p.at(0x50) + -1;
  }
  if (p.at(0x50) < 0) {
    p.at(0x50) = p.at(0x50) + 1;
  }
  if (p.at(0x54) < 0xb) {
    p.at(0x54) = p.at(0x54) + 1;
  }
  p.at(4) = p.at(4) + p.at(0x50);
  p.at(8) = p.at(8) + p.at(0x54);
  iVar4 = p.at(0x1c) + 1;
  p.at(0x1c) = iVar4;
  if (p.at(0x24) < iVar4) {
    p.at(0x1c) = 0;
    iVar4 = p.at(0x18) + 1;
    p.at(0x18) = iVar4;
    if (p.at(0x20) + -1 < iVar4) {
      p.at(0x18) = 0;
    }
  }
  return;
}
};
}
