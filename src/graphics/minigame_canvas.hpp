#pragma once
#include "../assets/asset_store.hpp"
namespace shin {
struct MinigameCanvas {
 IndexedSurface&f;
 MinigameCanvas(AssetStore&a,IndexedSurface&surface):f(surface){
  f.clear();f.border_rgb.assign(f.pixels.size(),0);auto&bg=a.image("tt0001.app");auto palette=a.palette("tt0002.pal");
  for(int y=0;y<f.height;++y)for(int x=0;x<f.width;++x){int u=x-f.origin_x,v=y-f.origin_y;if(u>=0&&u<640&&v>=0&&v<480)continue;u=(u%400+400)%400;v=(v%320+320)%320;auto index=bg.wallpaper[v*640+u];f.border_rgb[y*f.width+x]=0xff000000u|palette[index];}
  f.clip_core=true;
 }
 ~MinigameCanvas(){f.clip_core=false;}
};
}
