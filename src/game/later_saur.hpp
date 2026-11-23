#pragma once
#include "stage2_mouse.hpp"
namespace shin::game {
class LaterSaur {
 std::vector<const char*>names;std::vector<int>frames;
 void pose(int s){int delay=10;switch(s){
#include "later_saur_poses.inc"
 }auto&e=payload;e.at(0x58)=s;e.at(0x18)=e.at(0x1c)=0;e.at(0x20)=int(frames.size());e.at(0x24)=delay;if(s==6)sound_requests.push_back("RESOURCE/sau_a.wav");}
public:
 EntityPayload payload;bool respawn_requested=false,defeated=false;std::vector<std::string>sound_requests;
 LaterSaur(int x,int y,AssetStore&a){
#include "later_saur_names.inc"
 auto&e=payload;e.kind=0x7a;e.at(4)=e.at(0x48)=x;e.at(8)=e.at(0x4c)=y;auto&im=a.image(names.back());e.at(0xc)=im.width;e.at(0x10)=im.height;e.at(0x28)=8;e.at(0x2c)=23;e.at(0x30)=76;e.at(0x34)=95;pose(1);}
 static CollisionRect rectangle(EntityPayload&e){return {8,23,76,95};}
 void trigger(int direction){if(payload.state!=2&&payload.at(0x58)!=3&&direction>=0&&direction<=2)pose(direction);}
 void hurt(){if(payload.at(0x58)!=3){pose(3);sound_requests.push_back("RESOURCE/sau_d.wav");}}
 void tick(int cx,int cy,PlayerPayload&p,AssetStore&a,IndexedSurface&f){auto&e=payload;if(e.state==2)return;if(std::abs(e.at(4)-cx-320)>960){respawn_requested=true;e.state=2;return;}e.at(0x48)=e.at(4);e.at(0x4c)=e.at(8);int iVar4=0;
#include "later_saur_ai.inc"
 f.blit(a.image(names.at(frames.at(e.at(0x18)))),e.at(4)-cx,e.at(8)-cy,true,0,0,-1,-1,e.at(0x68)!=0);if(e.at(0x50)>0)--e.at(0x50);if(e.at(0x50)<0)++e.at(0x50);if(e.at(0x54)<11)++e.at(0x54);e.at(4)+=e.at(0x50);e.at(8)+=e.at(0x54);if(++e.at(0x1c)>e.at(0x24)){e.at(0x1c)=0;e.at(0x18)=(e.at(0x18)+1)%e.at(0x20);}++e.at(0x6c);}
 void terrain(const CollisionWorld&w,int param_2){auto&e=payload;auto probe=[&](int,int x,int y){return w.body(rectangle(e),x,y);};
#include "later_saur_terrain.inc"
 }
};
}
