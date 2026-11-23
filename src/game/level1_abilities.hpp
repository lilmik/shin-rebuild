#pragma once
#include "player_animation.hpp"
#include "collision_world.hpp"
#include <functional>
namespace shin {
class Level1Abilities {
 AssetStore& assets_;PlayerPayload*player_=nullptr;
 int choice_=0,x_=0,y_=0,state_=0,frame_=0,ticks_=0,delay_=0,count_=0;bool active_=false,completed_=false,double_=false,target_found_=false;
 int screen_x_=0,screen_y_=0,target_x_=0,target_y_=0;
 struct Bomb {int x,y,vx,vy,frame=0,ticks=0;};std::vector<Bomb> bombs_;
 void change(int state,int count,int delay);
public:
 // Return the number of newly defeated targets, not overlap count.
 std::function<int(const game::CollisionRect&,int,int)> damage;
 // 418d70/423ba0: first minimum |target.x-x| strictly below 10000.
 // Types 0x32, 0x65..0x7c, 0x7f; no dying/viewport filter.
 // False means no target; target coordinates remain unchanged in that case.
 std::function<bool(int,int,int&,int&)> nearest_target;
 // 419180/44ea20: invoke helpers on all types 0x32, 0x65..0x7c,
 // 0x7e, 0x7f, even dying targets, without awarding score.
 // Boss type 0x74 invokes 422680 once (HP--, state4), even in state4.
 std::function<void()> clear_enemies;
 std::vector<std::string> sounds;
 explicit Level1Abilities(AssetStore&a):assets_(a){}
 bool active()const{return active_;}int state()const{return state_;}int choice()const{return choice_;}
 bool activate(int choice,PlayerPayload&p,int cx,int cy);
 void tick(int cx,int cy,IndexedSurface& frame);
};
}
