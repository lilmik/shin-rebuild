#pragma once
#include <algorithm>
#include <array>
#include <cstdint>
#include <stdexcept>
#include <vector>
namespace shin {
// Software replacement for indexed DirectDraw surfaces. Original key: 0xfe
// (00433410); palette bytes are RGB6 expanded by <<2 (00401d10).
struct IndexedSurface {
 int width=0,height=0;
 int origin_x=0,origin_y=0;
 int repeat_x=0,repeat_y=0;
 bool clip_core=false;std::vector<std::uint32_t> border_rgb;
 std::vector<std::uint8_t> wallpaper;
 std::vector<std::uint8_t> pixels;
 // Optional opaque RGB overrides for manually edited artwork. Kept separate
 // from the game's indexed pixels so edits are not quantized to the old palette.
 std::vector<std::uint32_t> color_pixels;
 struct PlayerDraw{const IndexedSurface*image;int x,y,sx,sy,w,h;bool keyed,flip;};
 std::vector<PlayerDraw> player_draws;
 void blit_player(const IndexedSurface&image,int x,int y,bool keyed=true,int sx=0,int sy=0,int w=-1,int h=-1,bool flip=false){blit(image,x,y,keyed,sx,sy,w,h,flip);player_draws.push_back({&image,x+origin_x,y+origin_y,sx,sy,w,h,keyed,flip});}
 void flush_player(){auto draws=std::move(player_draws);player_draws.clear();int ox=origin_x,oy=origin_y;bool clip=clip_core;origin_x=origin_y=0;clip_core=false;for(auto&d:draws)blit(*d.image,d.x,d.y,d.keyed,d.sx,d.sy,d.w,d.h,d.flip);origin_x=ox;origin_y=oy;clip_core=clip;}

 IndexedSurface()=default;
 IndexedSurface(int w,int h,std::uint8_t fill=0):width(w),height(h){
  if(w<0||h<0||std::uint64_t(w)*h>268435456)throw std::runtime_error("invalid surface size");
  pixels.assign(std::size_t(w)*h,fill);
 }
 void clear(std::uint8_t color=0){player_draws.clear();std::fill(pixels.begin(),pixels.end(),color);std::fill(border_rgb.begin(),border_rgb.end(),0);}
 void blit(const IndexedSurface& source,int x,int y,bool keyed=true,int sx=0,int sy=0,int w=-1,int h=-1,bool flip_x=false){
  // Preserve native UI art at one scale; extend its background outside the
  // original design rectangle using the same unscaled resource tiles.
  if(!clip_core&&(origin_x||origin_y)&&x==0&&y==0&&sx==0&&sy==0&&w<0&&h<0&&source.width==640&&source.height==480){
   for(int yy=0;yy<height;++yy)for(int xx=0;xx<width;++xx){if(xx>=origin_x&&xx<origin_x+640&&yy>=origin_y&&yy<origin_y+480)continue;
    int u=xx-origin_x,v=yy-origin_y;u=u<0?(u%32+32)%32:u>=640?608+(u-640)%32:u;v=v<0?(v%32+32)%32:v>=480?448+(v-480)%32:v;if(source.repeat_x>0){int raw=xx-origin_x;u=(raw%source.repeat_x+source.repeat_x)%source.repeat_x;}if(source.repeat_y>0){int raw=yy-origin_y;v=(raw%source.repeat_y+source.repeat_y)%source.repeat_y;}auto c=(source.wallpaper.empty()?source.pixels:source.wallpaper)[std::size_t(v)*640+u];if(!keyed||c!=254)pixels[std::size_t(yy)*width+xx]=c;
   }
  }
  x+=origin_x;y+=origin_y;
  if(w<0)w=source.width-sx;if(h<0)h=source.height-sy;
  // Clipping retains source offsets; no stretching or interpolation.
  int left=std::max(0,-x),top=std::max(0,-y);
  int right=std::min(w,width-x),bottom=std::min(h,height-y);
  if(clip_core){left=std::max(left,origin_x-x);top=std::max(top,origin_y-y);right=std::min(right,origin_x+640-x);bottom=std::min(bottom,origin_y+480-y); }
  if(!source.color_pixels.empty()&&border_rgb.empty())border_rgb.assign(pixels.size(),0);
  for(int j=top;j<bottom;++j){int yy=sy+j;if(yy<0||yy>=source.height)continue;
   for(int i=left;i<right;++i){int xx=sx+(flip_x?w-1-i:i);if(xx<0||xx>=source.width)continue;
    auto c=source.pixels[std::size_t(yy)*source.width+xx];
    if(!keyed||c!=0xfe){auto dest=std::size_t(y+j)*width+x+i;pixels[dest]=c;
     if(!border_rgb.empty())border_rgb[dest]=source.color_pixels.empty()?0:source.color_pixels[std::size_t(yy)*source.width+xx];
    }
   }
  }
 }
};
}
