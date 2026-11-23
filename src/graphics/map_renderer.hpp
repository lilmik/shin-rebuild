#pragma once
#include "../assets/resource_archive.hpp"
#include "indexed_surface.hpp"
namespace shin {
// 00427900: row-major 10-byte cells; first two int16 tile indices;
// tile atlas has 20 columns of 32x32 tiles, -1 means absent.
inline void draw_map_layers(IndexedSurface& target,const MapData& map,const IndexedSurface& atlas,int camera_x,int camera_y){
 for(std::uint32_t y=0;y<map.height;++y){
  auto dy=std::int64_t(y)*32-camera_y;if(dy<=-32||dy>=target.height)continue;
  for(std::uint32_t x=0;x<map.width;++x){
   auto dx=std::int64_t(x)*32-camera_x;if(dx<=-32||dx>=target.width)continue;
   const auto& c=map.cells[std::size_t(y)*map.width+x];
   for(auto tile:{c.background_tile,c.foreground_tile})if(tile>=0)
    target.blit(atlas,int(dx),int(dy),true,(tile%20)*32,(tile/20)*32,32,32);
  }
 }
}
}
