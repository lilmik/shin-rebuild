#pragma once
#include <array>
#include <cstdint>
#include <string>
#include <vector>
#include "../assets/resource_archive.hpp"
namespace shin::game {
struct StageBinding {const char* map;const char* tiles;const char* collision_blocks;const char* background;std::uint32_t init,update,cleanup;};
const std::array<StageBinding,7>& stage_bindings();
struct SceneCamera {int pixel_x=0,pixel_y=0,tile_x=0,tile_y=0,offset_x=0,offset_y=0,previous_x=0,previous_y=0,field24=0,field28=0,field2c=0;int left=170,right=470,top=240,bottom=280;};
struct MapPlacement {int cell_x,cell_y,type,offset_x,offset_y;int pixel_x(int sprite_width)const{return cell_x*32+16+offset_x-sprite_width/2;}int pixel_y(int sprite_height)const{return (cell_y+1)*32+offset_y-sprite_height;}};
struct StageScene {
 int stage=-1;MapData map{};AppImage tiles{},collision_blocks{},background{};SceneCamera camera{};
 // No palette is selected here: per-stage palette binding has not been established.
 std::vector<MapPlacement> placements;
 void load(ResourceArchive&,int stage_index);
 // Initial spawn globals are reset to zero by 0x427900; object callbacks may change them.
 int spawn_cell_x=0,spawn_cell_y=0;
};
// 0x43ee50: save.dat is little-endian signed highest-unlocked-stage + 1.
int next_saved_progress(int existing_value,int stage_index);
}

