#pragma once
#include "stage2_mouse.hpp"
namespace shin::game {
class LaterBat {
 void pose(int s){auto&e=payload;e.at(0x58)=s;e.at(0x18)=e.at(0x1c)=0;e.at(0x20)=s==1||s==2?2:1;e.at(0x24)=s==0?100:10;if(s==1||s==2)sound_requests.push_back("RESOURCE/bat.wav");}
public:
 EntityPayload payload;bool one_way_left=false;bool respawn_requested=false,defeated=false;std::vector<std::string>sound_requests;
 LaterBat(int x,int y,AssetStore&a){auto&e=payload;e.kind=0x7f;e.at(4)=e.at(0x48)=x;e.at(8)=e.at(0x4c)=y;auto&im=a.image("bat2.raw");e.at(0xc)=im.width;e.at(0x10)=im.height;e.at(0x28)=6;e.at(0x2c)=4;e.at(0x30)=36;e.at(0x34)=30;pose(1);}
 static CollisionRect rectangle(EntityPayload&e){return {6,4,36,30};}
 void trigger(int direction){if(!one_way_left&&payload.state!=2&&payload.at(0x58)!=3&&direction>=0&&direction<=2)pose(direction);}
 void hurt(){if(payload.at(0x58)!=3){pose(3);sound_requests.push_back("RESOURCE/puk.wav");}}
 void tick(int cx,int cy,AssetStore&a,IndexedSurface&f){auto&e=payload;if(e.state==2)return;if(std::abs(e.at(4)-cx-320)>960){respawn_requested=true;e.state=2;return;}e.at(0x48)=e.at(4);e.at(0x4c)=e.at(8);if(e.at(0x58)==1){e.at(0x50)=-2;e.at(0x68)=0;}else if(e.at(0x58)==2){e.at(0x50)=2;e.at(0x68)=1;}else if(e.at(0x58)==3&&e.at(0x18)==0){if(e.at(0x1c)==0)e.at(0x54)=-10;if(e.at(0x1c)==9){e.at(0x54)=10;e.state=2;defeated=true;}}
 f.blit(a.image(e.at(0x58)==0?"bat2.raw":e.at(0x58)==3?"bat1.raw":e.at(0x18)?"bat2.raw":"bat1.raw"),e.at(4)-cx,e.at(8)-cy,true,0,0,-1,-1,e.at(0x68)!=0);if(e.at(0x50)>0)--e.at(0x50);if(e.at(0x50)<0)++e.at(0x50);e.at(4)+=e.at(0x50);e.at(8)+=e.at(0x54);if(++e.at(0x1c)>e.at(0x24)){e.at(0x1c)=0;e.at(0x18)=(e.at(0x18)+1)%e.at(0x20);}}
 void terrain(const CollisionWorld&w,int param_2){auto&e=payload;auto probe=[&](int,int x,int y){return w.body(rectangle(e),x,y);};
#include "later_bat_terrain.inc"
 }
};
}

