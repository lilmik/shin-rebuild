#pragma once
#include "stage_scene.hpp"
#include "player_controller.hpp"
namespace shin::game {
// Normal scrolling branch of 00424e00. Pending offsets are consumed before
// new following deltas are computed; player screen coordinates are last frame's.
inline void begin_camera_frame(SceneCamera&c,int width,int height){
 const int x=std::clamp(c.tile_x*32+c.offset_x,0,std::max(0,(width-20)*32));
 const int y=std::clamp(c.tile_y*32+c.offset_y,0,std::max(0,(height-15)*32));
 c.tile_x=x/32;c.offset_x=x%32;c.tile_y=y/32;c.offset_y=y%32;c.pixel_x=x;c.pixel_y=y;
}
inline void plan_normal_camera(SceneCamera&c,PlayerPayload&p,int width,int height){
 // A stable dead zone prevents facing changes from alternating the target
 // between opposite sides of the screen during ordinary movement.
 const int center_x=p.at(8)+p.at(0x10)/2;
 int target_x=c.pixel_x;
 if(center_x<c.pixel_x+c.left)target_x=center_x-c.left;
 else if(center_x>c.pixel_x+c.right)target_x=center_x-c.right;
 target_x=std::clamp(target_x,0,std::max(0,(width-20)*32));
 int delta_x=target_x-c.pixel_x;
 int dx=delta_x/6;if(dx==0&&delta_x!=0)dx=delta_x>0?1:-1;
 dx=std::clamp(dx,-16,16);
 const int planned_x=std::clamp(c.pixel_x+dx,0,std::max(0,(width-20)*32));
 c.offset_x=planned_x-c.tile_x*32;
 auto vertical=[&](int edge){int half=p.at(0x14)/2;
  int dy=(half-c.pixel_y-edge+p.at(12))/6;int center=p.at(12)-c.pixel_y+half;
  dy=std::clamp(dy,-16,16);
  
  c.offset_y+=dy;if(c.tile_y==0&&c.offset_y<0)c.offset_y=0;
  if(height-c.tile_y==16&&c.offset_y>31)c.offset_y=31;
 };
 if(p.at(12)<c.pixel_y+c.top)vertical(c.top+p.at(0x14)/2);
 if(c.bottom+c.pixel_y<p.at(12))vertical(c.bottom+p.at(0x14)/2);
}
}
