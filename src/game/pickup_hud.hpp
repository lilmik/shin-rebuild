#pragma once
#include "player_animation.hpp"
namespace shin {
// 413740/4137b0/413830: six score digits at (10,10), stride32.
// The original score at player+70 is shared by pickups and enemy defeats.
inline void draw_pickup_hud(AssetStore&assets,PlayerPayload&player,IndexedSurface&target){
 target.flush_player();
 int value=player.at(0x70);const int divisor[6]={100000,10000,1000,100,10,1};
 for(int i=0;i<6;++i){int digit=value/divisor[i];value%=divisor[i];
  if(digit>=0&&digit<10)target.blit(assets.image(std::to_string(digit)+".app"),10+32*i-target.origin_x,10-target.origin_y);
 }
}
}

