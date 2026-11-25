#pragma once
#include "player_object_response.hpp"
#include "collision_world.hpp"
#include "../assets/asset_store.hpp"
#include <functional>
namespace shin {
namespace dialogue_data {
#include "dialogue_tables.inc"
}
// Recovered 443cd0/443e30 and 436030/436130. Owns dialogue animation;
// host continues ordinary terrain response and restores its normal update on completion.
class Level1Dialogues {
public:
 struct Actor {int identity,x,y,previous_x,previous_y,vx=0,vy=0,frame=0,ticks=0;bool speaking=false,removed=false;};
 using Sound=std::function<void(const char*)>;
 using ActorCollision=std::function<void(Actor&)>;
 using PlayerCollision=std::function<void(PlayerPayload&)>;
 std::vector<Actor> actors;
 std::function<void(const Actor&)> resume_enemy; // Required for original4040f0 handoff after75.
 // Exact shared44f040/429070/42e700 terrain response.
 static void collide_actor(Actor&a,const game::CollisionWorld&w){
  const game::CollisionRect r=a.identity==0?game::CollisionRect{32,25,67,87}:a.identity==1?game::CollisionRect{12,10,46,74}:a.identity==2?game::CollisionRect{11,8,46,96}:game::CollisionRect{28,14,46,71};
  const int hit=w.body(r,a.x,a.y);if(hit!=1&&hit!=2)return;
  for(int i=0;i<26;++i)if(!w.body(r,a.x,a.y-i)){a.y-=i;if(hit==2)a.vy+=2;return;}
  if(hit==1){
   if(a.previous_x<a.x){for(int x=a.x;x>=a.previous_x;--x)if(!w.body(r,x,a.previous_y)){a.x=x;break;}}
   else if(a.x<a.previous_x){for(int x=a.x;x<=a.previous_x;++x)if(!w.body(r,x,a.previous_y)){a.x=x;break;}}
   if(a.previous_y<a.y){for(int y=a.y;y>=a.previous_y;--y)if(!w.body(r,a.x,y)){a.y=y;return;}}
   else if(a.y<a.previous_y){for(int y=a.y;y<=a.previous_y;++y)if(!w.body(r,a.x,y)){a.y=y;return;}}
  }else if(a.previous_y<a.y){for(int y=a.y;y>=a.previous_y;--y)if(!w.body(r,a.x,y)){a.y=y;return;}}
 }
private:
 int type_=0,saved_state_=0;unsigned started_=0,phase_=0;bool active_=false,enter_=false,played_=false;
 const int* sequence_=nullptr;int count_=0;
 void pose(PlayerPayload&p,int state,const int*seq,int count){p.at(0x18)=state<100?0x45d3e8+int(seq-dialogue_data::pi41)*4:0x45d4d8+int(seq-dialogue_data::pi75)*4;p.at(0x5c)=state;p.at(0x1c)=p.at(0x20)=0;p.at(0x24)=count;p.at(0x28)=10;sequence_=seq;count_=count;}
 void speaker(int n,bool yes){auto&a=actors.at(n);a.speaking=yes;a.frame=a.ticks=0;}
 void finish(PlayerPayload&p){active_=false;p.at(0x18)=0x45c714;p.at(0x24)=1;p.at(0x28)=100;p.at(0x5c)=0;p.at(0x1c)=p.at(0x20)=0;for(auto&a:actors){a.speaking=false;a.frame=a.ticks=0;}if(type_==0x75){if(!resume_enemy)throw std::runtime_error("unbound stationary dialogue enemy");resume_enemy(actors.at(0));actors.at(0).removed=true;}}
public:
 bool active()const{return active_;} unsigned phase()const{return phase_;} int type()const{return type_;}
 // Retain dialogue actors after completion; the delinquent remains stationary.
 bool start(int type,PlayerPayload&p,unsigned now,int camera_x,int view_width=640){
  if(active_||!(type==0x41||type==0x75))return false;
  type_=type;saved_state_=p.at(0x5c);started_=now;phase_=0;active_=true;enter_=played_=false;actors.clear();
  if(type==0x41){pose(p,20,dialogue_data::pi41,4);int first=camera_x+640;if(view_width>640)first=std::max(first,p.at(8)+p.at(0x10)+272);for(int i=0;i<3;++i)actors.push_back({i,first+i*80,p.at(12)-30,first+i*80,p.at(12)-30});}
  else {pose(p,107,dialogue_data::pi75,1);actors.push_back({3,camera_x+600,p.at(12)-30,camera_x+600,p.at(12)-30});}return true;
 }
 // Returns true on the exact frame the replacement player callback finishes.
 bool tick(PlayerPayload&p,unsigned now,bool enter,int cx,int cy,AssetStore&assets,IndexedSurface&frame,const Sound&sound,const PlayerCollision&player_collision,const ActorCollision&actor_collision){
  if(!active_)return false;
  auto emit=[&](const char*s){if(sound)sound(s);};
  if(!played_){played_=true;if(type_==0x41){emit("RESOURCE/29.wav");emit("RESOURCE/30.wav");}else emit("RESOURCE/43.wav");}
  if(enter)enter_=true;else if(enter_){finish(p);return true;}
  const unsigned elapsed=now-started_;
  if(type_==0x41){
   static constexpr unsigned times[]={3930,7010,13930,19450,25830,33450,36060,37650,39650};
   while(phase_<9&&elapsed>times[phase_]){
    switch(phase_){
    case 0:pose(p,21,dialogue_data::pi41+4,1);emit("RESOURCE/31.wav");emit("RESOURCE/29.wav");emit("RESOURCE/30.wav");break;
    case 1:pose(p,22,dialogue_data::pi41+5,1);speaker(0,true);emit("RESOURCE/32.wav");emit("RESOURCE/30.wav");break;
    case 2:pose(p,23,dialogue_data::pi41+6,2);speaker(0,false);emit("RESOURCE/33.wav");emit("RESOURCE/29.wav");emit("RESOURCE/30.wav");break;
    case 3:pose(p,22,dialogue_data::pi41+5,1);speaker(1,true);emit("RESOURCE/34.wav");emit("RESOURCE/29.wav");break;
    case 4:pose(p,24,dialogue_data::pi41+8,2);speaker(1,false);emit("RESOURCE/35.wav");emit("RESOURCE/29.wav");emit("RESOURCE/30.wav");break;
    case 5:pose(p,22,dialogue_data::pi41+5,1);speaker(0,true);emit("RESOURCE/36.wav");emit("RESOURCE/30.wav");break;
    case 6:pose(p,25,dialogue_data::pi41+10,2);speaker(0,false);emit("RESOURCE/37.wav");break;
    case 7:speaker(2,true);emit("RESOURCE/38.wav");break;
    case 8:speaker(2,false);finish(p);break;
    }++phase_;
   }
  }else{
   static constexpr unsigned times[]={740,2790,4470,7480,8480,12160};
   while(phase_<6&&elapsed>times[phase_]){
    switch(phase_){
    case 0:pose(p,108,dialogue_data::pi75+1,1);emit("RESOURCE/44.wav");break;
    case 1:pose(p,109,dialogue_data::pi75+2,2);emit("RESOURCE/46.wav");break;
    case 2:pose(p,110,dialogue_data::pi75+4,2);emit("RESOURCE/47-1.wav");break;
    case 3:pose(p,108,dialogue_data::pi75+1,1);speaker(0,true);break;
    case 4:pose(p,111,dialogue_data::pi75+6,2);emit("RESOURCE/47-2.wav");break;
    case 5:finish(p);break;
    }++phase_;
   }
  }
  if(!active_)return true;
  const char*const* names=type_==0x41?dialogue_data::p41:dialogue_data::p75;
  p.at(0x54)=type_==0x41&&p.at(0x5c)==20?2:0;
  p.at(0x44)=p.at(8)-cx;p.at(0x48)=p.at(12)-cy;
  frame.blit_player(assets.image(names[sequence_[p.at(0x1c)]]),p.at(0x44),p.at(0x48)-(p.at(0x5c)==20?0:9),true);
  if(p.at(0x54)>0)--p.at(0x54);if(p.at(0x54)<0)++p.at(0x54);if(p.at(0x58)<11)++p.at(0x58);p.at(8)+=p.at(0x54);if(type_==0x41&&!actors.empty())p.at(8)=std::min(p.at(8),actors.front().x-p.at(0x10)-24);p.at(12)+=p.at(0x58);
  if(++p.at(0x20)>10){p.at(0x20)=0;p.at(0x1c)=(p.at(0x1c)+1)%count_;}
  if(player_collision)player_collision(p);
  tick_actors(cx,cy,assets,frame,actor_collision);
  return false;
 }
 // Host may keep invoking this after completion for the three retained background actors.
 void tick_actors(int cx,int cy,AssetStore&assets,IndexedSurface&frame,const ActorCollision&collision){
  for(auto&a:actors){
   if(a.removed)continue; // Retain participants when the wide view pans away.
   const char*const*names=a.identity==0?dialogue_data::n0:a.identity==1?dialogue_data::n1:a.identity==2?dialogue_data::n2:dialogue_data::n3;
   int offset=a.identity==2?(a.speaking?1:0):a.identity==3?(a.speaking?1:0):(a.speaking?2:0);
   int count=a.identity==2&&!a.speaking?1:a.identity==3?1:2;
   a.previous_x=a.x;a.previous_y=a.y;
   frame.blit(assets.image(names[offset+a.frame]),a.x-cx,a.y-cy,true);
   if(a.vx>0)--a.vx;if(a.vx<0)++a.vx;if(a.vy<11)++a.vy;a.x+=a.vx;a.y+=a.vy;
   if(++a.ticks>10){a.ticks=0;a.frame=(a.frame+1)%count;}
   if(collision)collision(a);
  }
 }
};
}
