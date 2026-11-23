#pragma once
#include "stage2_mouse.hpp"
namespace shin::game {
class LaterPatrolEnemy {
 int type_;std::vector<const char*>names;std::vector<int>frames;
 void pose(int state){int delay=10;if(type_==93){switch(state){
#include "later_patrol_93_poses.inc"
 }}else if(type_==22){switch(state){
#include "later_patrol_22_poses.inc"
 }}else if(type_==23){switch(state){
#include "later_patrol_23_poses.inc"
 }}else{switch(state){
#include "later_patrol_51_poses.inc"
 }}auto&e=payload;e.at(0x58)=state;e.at(0x18)=e.at(0x1c)=0;e.at(0x20)=int(frames.size());e.at(0x24)=delay;}
public:
 struct ProjectileEvent{int x,y,vx,vy;unsigned callback;int kind;};
 EntityPayload payload;bool respawn_requested=false,defeated=false;std::vector<std::string>sound_requests;std::vector<ProjectileEvent>projectile_events;
 static bool supports(int t){return t==22||t==23||t==51||t==93;}
 LaterPatrolEnemy(int t,int x,int y,AssetStore&a):type_(t){if(t==22){
#include "later_patrol_22_data.inc"
 }else if(t==23||t==93){
#include "later_patrol_23_data.inc"
 }else if(t==51){
#include "later_patrol_51_data.inc"
 }else throw std::invalid_argument("patrol type");auto&e=payload;e.kind=t==93?0x7e:t==22?0x6d:t==23?0x6e:0x7b;e.at(4)=e.at(0x48)=x;e.at(8)=e.at(0x4c)=y;auto&im=a.image(names.back());e.at(0xc)=im.width;e.at(0x10)=im.height;e.at(0x28)=t==22?20:(t==23||t==93)?17:34;e.at(0x2c)=t==22?0:(t==23||t==93)?49:20;e.at(0x30)=t==22?60:(t==23||t==93)?84:149;e.at(0x34)=t==22?80:(t==23||t==93)?130:57;pose(1);}
 void trigger(int direction){if(payload.at(0x58)==(type_==51?3:4)||direction<0||direction>2)return;pose(direction);}
 static CollisionRect rectangle(EntityPayload&e){return {e.at(0x28),e.at(0x2c),e.at(0x30),e.at(0x34)};}
 void hurt(){if(payload.at(0x58)!=(type_==51?3:4)){pose(type_==51?3:4);sound_requests.push_back("RESOURCE/punch2.wav");}}
 void tick(int cx,int cy,AssetStore&a,IndexedSurface&f,PlayerPayload*player=nullptr){auto&e=payload;if(e.state==2)return;if(std::abs(e.at(4)-cx-320)>960){respawn_requested=true;e.state=2;return;}e.at(0x48)=e.at(4);e.at(0x4c)=e.at(8);int state=e.at(0x58);if(state<=2){if((type_==22||type_==93)&&e.at(0x6c)>100){pose(3);e.at(0x6c)=0;}if(state==1){e.at(0x50)=type_==51?-2:-4;e.at(0x68)=0;}if(state==2){e.at(0x50)=type_==51?2:4;e.at(0x68)=1;}}
 else if((type_==22||type_==93)&&state==3){if(e.at(0x18)==2&&e.at(0x1c)==0){int direction=player&&player->at(8)>=e.at(4)?1:0;projectile_events.push_back({e.at(4)+16,e.at(8)+40,type_==93?(direction?10:-10):0,type_==93?-16:6,0x445480,0x61});pose(type_==93&&direction?2:1);}}
 else if(state==(type_==51?3:4)){if(e.at(0x18)==0&&e.at(0x1c)==0)e.at(0x54)=-10;if(e.at(0x18)==1&&e.at(0x1c)==0){e.at(0x54)=10;e.state=2;defeated=true;}}
 f.blit(a.image(names.at(frames.at(e.at(0x18)))),e.at(4)-cx,e.at(8)-cy,true,0,0,-1,-1,e.at(0x68)!=0);if(e.at(0x50)>0)--e.at(0x50);if(e.at(0x50)<0)++e.at(0x50);if((type_==23||type_==93)&&e.at(0x54)<11)++e.at(0x54);e.at(4)+=e.at(0x50);e.at(8)+=e.at(0x54);if(++e.at(0x1c)>e.at(0x24)){e.at(0x1c)=0;e.at(0x18)=(e.at(0x18)+1)%e.at(0x20);}if(type_==22||type_==93)++e.at(0x6c);
 }
 void terrain(const CollisionWorld&w,int param_2){auto&e=payload;auto probe=[&](int,int x,int y){return w.body(rectangle(e),x,y);};if(type_==93){
#include "later_patrol_93_terrain.inc"
 }else if(type_==22){
#include "later_patrol_22_terrain.inc"
 }else if(type_==23){
#include "later_patrol_23_terrain.inc"
 }else{
#include "later_patrol_51_terrain.inc"
 }}
};
}
