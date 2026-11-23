#pragma once
#include "stage_scene.hpp"
#include "player_animation.hpp"
namespace shin {
inline void preload_bootstrap(AssetStore&a,unsigned index){
 if(index<7){const auto&b=game::stage_bindings()[index];a.preload_images({b.tiles,b.collision_blocks,b.background});}
 else if(index<7+player_resources.size())a.image(player_resources[index-7]);
}
}
