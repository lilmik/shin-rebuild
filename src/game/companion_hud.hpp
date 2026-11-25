#pragma once
#include "player_animation.hpp"
#include "collision_world.hpp"
#include <functional>
namespace shin {
// 439500 / 4395f0 / 439820: Shiro, also the lower-left life display.
class CompanionHud {
 int state_=0,frame_=0,tick_=0,delay_=10,x_=10,y_=400;
 int screen_x_=10,screen_y_=400,origin_x_=0,origin_y_=0;
 void pose(int state){state_=state;frame_=tick_=0;delay_=state==0?10:50;}
public:
 struct Target{int x,y;game::CollisionRect rect;std::function<void()> hit;};
 bool active()const{return state_!=0;}
 int state()const{return state_;}
 int screen_pixel_x()const{return screen_x_+origin_x_;}
 int screen_pixel_y()const{return screen_y_+origin_y_;}
 int screen_x()const{return screen_x_;}
 int screen_y()const{return screen_y_;}
 void activate(){if(!active())pose(1);}
 void tick(PlayerPayload&p,int cx,int cy,AssetStore&assets,IndexedSurface&surface,const std::vector<Target>&targets){
  if(state_==1&&tick_==49)pose(2);
  else if(state_==2&&tick_==49)pose(3);
  else if(state_==3){if(tick_==49)pose(4);const Target*nearest=nullptr;
   for(const auto&t:targets){
    // Preserve registry order as in 43a3f0; retain widescreen visibility filtering.
    if(!game::intersects(t.rect,t.x-cx,t.y-cy,{-surface.origin_x,-surface.origin_y,surface.width-surface.origin_x,surface.height-surface.origin_y},0,0))continue;
    nearest=&t;break;
   }
   if(nearest){x_+=(nearest->x-x_)/2;y_+=(nearest->y-y_)/2;}}
  else if(state_==4){if(tick_==49)pose(0);x_=10;y_=400;}
  int sx=10-surface.origin_x,sy=surface.height-80-surface.origin_y;if(state_==3){sx=x_-cx;sy=y_-cy;}else{x_=p.at(8);y_=p.at(12);}
  screen_x_=sx;screen_y_=sy;origin_x_=surface.origin_x;origin_y_=surface.origin_y;
  static constexpr const char* images[]={"siro0.app","siro1.app","siros1.app","siros2.app","siros3.app","siros4.app"};
  surface.blit(assets.image(images[state_==0?frame_:state_+1]),sx,sy);
  for(int i=0;i<p.at(0x8c);++i)surface.blit(assets.image("life.app"),110+i*50-surface.origin_x,surface.height-60-surface.origin_y);
  if(++tick_>delay_){tick_=0;if(++frame_>=(state_==0?2:1))frame_=0;}
  if(state_==3)for(auto&t:targets)if(game::intersects({0,0,74,71},x_,y_,t.rect,t.x,t.y))t.hit();
 }
};
}
