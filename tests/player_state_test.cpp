#include "../src/game/player_state.hpp"
#include <stdexcept>
#include <iostream>
void check(bool v){if(!v)throw std::runtime_error("player test failed");}
int main(){auto p=shin::PlayerState::spawn(3,4);check(p.x==96&&p.y==128&&p.previous_x==96&&p.animation_delay==100);p.set_camera(32,64);check(p.screen_x==64&&p.screen_y==64);
p.horizontal_impulse=3;p.horizontal_enabled=1;p.vertical_velocity=-2;p.common_tail(false);check(p.x==98&&p.y==127&&p.horizontal_impulse==2&&p.vertical_velocity==-1);
p.horizontal_impulse=-1;p.horizontal_enabled=2;p.vertical_velocity=11;p.common_tail(false);check(p.x==98&&p.y==138&&p.horizontal_impulse==0&&p.vertical_velocity==11);
p.x=-4;p.y=-10;p.vertical_velocity=0;p.common_tail(false);check(p.x==0&&p.y==0);
p.animation_delay=1;p.animation_count=2;p.animation_ticks=0;p.animation_frame=0;p.common_tail(false);check(p.animation_frame==0);p.common_tail(false);check(p.animation_frame==1&&p.animation_ticks==0);p.common_tail(false);p.common_tail(false);check(p.animation_frame==0);
p.blink_ticks=2;p.collision_latch=9;p.common_tail(true);check(p.pause_latch==1&&p.blink_ticks==1&&p.collision_latch==0);check(p.common_tail(false)&&p.blink_ticks==1&&p.pause_latch==0);check(!p.common_tail(false)&&p.blink_ticks==0);
std::cout<<"player spawn/camera/physics tail/animation/pause passed\n";}
