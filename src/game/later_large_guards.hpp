#pragma once
#include "stage2_mouse.hpp"
namespace shin::game {
// MAP14/15 second dispatcher427610, constructors428160/403190.
class LaterLargeGuard {
 int type_;std::vector<const char*>names;std::vector<std::vector<int>>poses;
 static void setpose(EntityPayload&e,int s){e.at(0x58)=s;e.at(0x18)=e.at(0x1c)=0;e.at(0x20)=s==0?1:4;e.at(0x24)=s==0?100:10;}
public:
 EntityPayload payload;bool respawn_requested=false,defeated=false;std::vector<std::string>sound_requests;
 static bool supports(int t){return t==20||t==21;}
 LaterLargeGuard(int type,int x,int y,AssetStore&a):type_(type){if(type==20){
#include "later_guard_20_data.inc"
 }else if(type==21){
#include "later_guard_21_data.inc"
 }else throw std::invalid_argument("guard type");auto&e=payload;e.kind=type==20?0x6a:0x6c;e.at(4)=e.at(0x48)=x;e.at(8)=e.at(0x4c)=y;auto&im=a.image(names.back());e.at(0xc)=im.width;e.at(0x10)=im.height;e.at(0x28)=50;e.at(0x2c)=50;e.at(0x30)=100;e.at(0x34)=type==20?220:216;setpose(e,1);}
 void trigger(int direction){if(payload.at(0x58)==4||direction<0||direction>2)return;setpose(payload,direction);}
 static CollisionRect rectangle(EntityPayload&e){return {e.at(0x28),e.at(0x2c),e.at(0x30),e.at(0x34)};}
 void hurt(){if(payload.at(0x58)!=4){setpose(payload,4);sound_requests.push_back("RESOURCE/punch2.wav");}}
 void tick(int cx,int cy,AssetStore&a,IndexedSurface&f){auto&e=payload;if(e.state==2)return;if(std::abs(e.at(4)-cx-320)>(type_==20?2240:960)||(type_==20&&std::abs(e.at(8)-cy-240)>1680)){respawn_requested=true;e.state=2;return;}e.at(0x48)=e.at(4);e.at(0x4c)=e.at(8);int s=e.at(0x58);if(s==1){e.at(0x50)=-2;e.at(0x68)=0;}if(s==2){e.at(0x50)=2;e.at(0x68)=1;}if(s==4){if(e.at(0x18)==0&&e.at(0x1c)==0)e.at(0x54)=-10;if(e.at(0x18)==1&&e.at(0x1c)==0){e.at(0x54)=10;e.state=2;defeated=true;}}
 f.blit(a.image(names[poses[s==4?2:s==2?1:0][e.at(0x18)]]),e.at(4)-cx,e.at(8)-cy,true,0,0,-1,-1,e.at(0x68)!=0);if(e.at(0x50)>0)--e.at(0x50);if(e.at(0x50)<0)++e.at(0x50);if(type_==21&&e.at(0x54)<11)++e.at(0x54);e.at(4)+=e.at(0x50);e.at(8)+=e.at(0x54);if(++e.at(0x1c)>e.at(0x24)){e.at(0x1c)=0;e.at(0x18)=(e.at(0x18)+1)%e.at(0x20);}}
 void terrain(const CollisionWorld&w,int param_2){auto&e=payload;auto probe=[&](int,int x,int y){return w.body(rectangle(e),x,y);};if(type_==20){
#include "later_guard_20_terrain.inc"
 }else{
#include "later_guard_21_terrain.inc"
 }}
};
}

