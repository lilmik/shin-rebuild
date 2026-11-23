#pragma once
#include "player_controller.hpp"
#include "collision_world.hpp"
namespace shin::game {
struct WorldResponse {bool safety_limit_reached=false;bool hurt_sound_requested=false;};
// 0x40d4c0, with a finite scan budget on original unbounded upward loops.
WorldResponse respond_player_world(PlayerPayload&,const CollisionWorld&,int material,int scan_budget=65536);
}
