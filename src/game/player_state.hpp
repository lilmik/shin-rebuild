#pragma once
#include <cstdint>
#include <cstring>
namespace shin {
// Confirmed subset of 0x110-byte payload at global 00473d20; not its binary ABI.
struct PlayerState {
 std::int32_t x=0,y=0,screen_x=0,screen_y=0,previous_x=0,previous_y=0;
 std::int32_t animation_frame=0,animation_ticks=0,animation_count=1,animation_delay=100;
 std::int32_t horizontal_impulse=0,vertical_velocity=0,horizontal_enabled=0;
 std::int32_t collision_latch=0,pause_latch=0,blink_ticks=0;
 std::int32_t facing=1,field_8c=5,field_b0=3;
 std::uint32_t animation_table_id=0x45c714;
 static std::int32_t bits(std::uint32_t v){std::int32_t o;std::memcpy(&o,&v,4);return o;}
 static std::int32_t add(std::int32_t a,std::int32_t b){return bits(std::uint32_t(a)+std::uint32_t(b));}
 static PlayerState spawn(std::int32_t tile_x,std::int32_t tile_y){PlayerState p;p.x=bits(std::uint32_t(tile_x)<<5);p.y=bits(std::uint32_t(tile_y)<<5);p.previous_x=p.x;p.previous_y=p.y;return p;}
 void set_camera(std::int32_t camera_x,std::int32_t camera_y){screen_x=bits(std::uint32_t(x)-std::uint32_t(camera_x));screen_y=bits(std::uint32_t(y)-std::uint32_t(camera_y));}
 // Common tail of 00405cf0, invoked AFTER state-specific movement/collision/render.
 // Caller handles render before this method. Returns request to spawn pause menu.
 bool common_tail(bool enter){
  if(horizontal_impulse>0)--horizontal_impulse;
  if(horizontal_impulse<0)++horizontal_impulse;
  if(vertical_velocity<11)++vertical_velocity;
  if(horizontal_enabled==1)x=add(x,horizontal_impulse);
  y=add(y,vertical_velocity);if(x<0)x=0;if(y<0)y=0;
  animation_ticks=add(animation_ticks,1);
  if(animation_delay<animation_ticks){animation_ticks=0;animation_frame=add(animation_frame,1);if(add(animation_count,-1)<animation_frame)animation_frame=0;}
  collision_latch=0;
  if(enter)pause_latch=1;
  if(pause_latch==1&&!enter){pause_latch=0;return true;}
  if(blink_ticks>0)--blink_ticks;
  return false;
 }
};
}
