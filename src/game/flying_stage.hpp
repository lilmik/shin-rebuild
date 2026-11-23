#pragma once
#include "player_animation.hpp"
#include "camera_controller.hpp"
#include "player_object_response.hpp"
#include "collision_world.hpp"
#include "../core/input_state.hpp"
#include <algorithm>
namespace shin {
// Original flying-player callback406ff0; camera flying branch424e00.
// No ground gravity/jump physics are applied in this controller.
class FlyingStageMotion {
 bool initialized_=false;
public:
 std::vector<game::EntityPayload> shots;
 std::vector<unsigned> sounds;
 bool wants_menu=false,game_over=false;
 static game::CollisionRect player_rectangle(PlayerPayload&p){return {p.at(0x2c),p.at(0x30),p.at(0x34),p.at(0x38)};}
 static bool player_hits(PlayerPayload&p,const game::CollisionRect&r,int x,int y){return game::intersects(player_rectangle(p),p.at(8),p.at(12),r,x,y);}
 static game::CollisionRect shot_rectangle(){return {2,2,12,12};}
 // 40dad0/4077d0: damage pose completes before consuming one life.
 bool hurt(PlayerPayload&p){if(p.at(0xb8)!=0||p.at(0x5c)==0x13)return false;p.at(0x5c)=0x13;p.at(0x18)=0x45c7c0;p.at(0x1c)=p.at(0x20)=0;p.at(0x24)=1;p.at(0x28)=10;p.at(0x54)=-5;sounds.push_back(0x4752e8);return true;}
 // True means the contacted enemy must receive its original defeat helper.
 bool contact(PlayerPayload&p,game::EntityPayload&e){
  if(e.kind==0x1f||e.kind==0x20||e.kind==0x21||e.kind==0x6b)return hurt(p);
  if(e.kind==0x24||e.kind==0x25){if(p.at(0x5c)==0x13)return false;hurt(p);e.state=2;}
  return false;
 }
 static bool shot_hits(game::EntityPayload&s,const game::CollisionRect&r,int x,int y){if(s.state==2)return false;if(!game::intersects(shot_rectangle(),s.at(4),s.at(8),r,x,y))return false;s.state=2;return true;}
 // 44bbd0 destroys a projectile on its first terrain collision. Retiring
 // here avoids reproducing writes to an already-destroyed original object.
 void collide_shots(const game::CollisionWorld&w){for(auto&s:shots)if(s.state!=2){const int material=w.body(shot_rectangle(),s.at(4),s.at(8));if(material==1||material==2)s.state=2;}}

 static void pose(PlayerPayload&p){p.at(0x5c)=0x12;p.at(0x18)=0x45c7b8;p.at(0x1c)=p.at(0x20)=0;p.at(0x24)=2;p.at(0x28)=10;}
 void initialize(PlayerPayload&p){pose(p);p.at(0x54)=p.at(0x58)=0;p.at(0x88)=1;initialized_=true;}
 static void camera(game::SceneCamera&c,PlayerPayload&p,int width,int height,bool stopped){
  c.left=60;c.right=520;c.top=200;c.bottom=280; // 427ee0
  game::begin_camera_frame(c,width,height);
  if(p.at(12)<c.pixel_y+c.top)c.offset_y+=(p.at(12)-c.pixel_y-c.top)/2;
  if(p.at(12)>c.pixel_y+c.bottom)c.offset_y+=(p.at(12)-c.pixel_y-c.bottom)/2;
  if(c.tile_y==0&&c.offset_y<0)c.offset_y=0;if(height-c.tile_y==16&&c.offset_y>31)c.offset_y=31;
  if(!stopped){c.offset_x+=3;if(width-c.tile_x==21&&c.offset_x>31)c.offset_x=31;}
 }
 void tick(PlayerPayload&p,const InputState&in,int cx,int cy,int width,int height,bool stopped,AssetStore&a,IndexedSurface&f){
  if(!initialized_)initialize(p);p.at(0x88)=1;p.at(0x4c)=p.at(8);p.at(0x50)=p.at(12);
  if(p.at(0x5c)==0x13&&p.at(0x1c)==0&&p.at(0x20)==9){pose(p);--p.at(0x8c);p.grant_damage_immunity();if(p.at(0x8c)<=0){game_over=true;return;}}
  if(in.read(0x46d4b0))p.at(0x58)=-12;if(in.read(0x46d4b4))p.at(0x58)=12;
  if(in.read(0x46d4b8))p.at(0x54)=-8;if(in.read(0x46d4bc))p.at(0x54)=8;
  if(!in.read(0x46d4e8))p.at(0xe8)=0;else{if(p.at(0xe8)==0)p.at(0xfc)=3;p.at(0xe8)=1;}
  if(p.at(0xfc)>0){game::EntityPayload s;s.kind=0x23;s.at(4)=p.at(8)+93;s.at(8)=p.at(12)+49;s.at(0x3c)=24;s.at(0x14)=s.at(0x18)=2;s.at(0x1c)=s.at(0x20)=12;shots.push_back(s);--p.at(0xfc);}
  wants_menu=bool(in.read(0x46d4f0));p.at(0x44)=p.at(8)-cx;p.at(0x48)=p.at(12)-cy;
  if(p.at(0xb8)%5==0)f.blit_player(a.image(p.at(0x5c)==0x13?"sss2.app":p.at(0x1c)?"sss1.app":"sss0.app"),p.at(0x44),p.at(0x48));
  if(p.at(0x54)>0)--p.at(0x54);if(p.at(0x54)<0)++p.at(0x54);if(p.at(0x58)>0)--p.at(0x58);if(p.at(0x58)<0)++p.at(0x58);
  p.at(8)+=p.at(0x54)+(stopped?0:3);p.at(12)+=p.at(0x58);
  p.at(8)=std::clamp(p.at(8),std::max(0,cx-f.origin_x),std::min(cx-f.origin_x+f.width-160,width*32));p.at(12)=std::clamp(p.at(12),-30,height*32-80);
  if(++p.at(0x20)>p.at(0x28)){p.at(0x20)=0;if(++p.at(0x1c)>=p.at(0x24))p.at(0x1c)=0;}
  for(auto&s:shots){if(s.state==2)continue;f.blit(a.image("shutga.app"),s.at(4)-cx,s.at(8)-cy);s.at(4)+=24;}
  shots.erase(std::remove_if(shots.begin(),shots.end(),[&](auto&s){return s.state==2||std::abs(s.at(4)-cx-320)>960;}),shots.end());
 }
};
}

