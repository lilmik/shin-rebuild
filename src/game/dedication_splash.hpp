#pragma once
#include "../assets/rgba_overlay.hpp"
#include "../graphics/indexed_surface.hpp"
#include <cmath>
namespace shin {
// Optional user artwork, rendered before the original startup logos. Wall-clock timing.
class DedicationSplash {
 std::vector<unsigned char> source_;std::vector<std::uint32_t> scaled_;int width_=0,height_=0;unsigned began_=0;bool started_=false;
public:
 explicit DedicationSplash(const std::filesystem::path&path):source_(read_rgba_overlay(path,1000,1000)){}
 bool render(unsigned now,IndexedSurface&f){
  if(source_.empty())return false;if(!started_){started_=true;began_=now;}unsigned elapsed=now-began_;
  if(elapsed>=5000){f.border_rgb.clear();return false;}
  if(width_!=f.width||height_!=f.height){width_=f.width;height_=f.height;scaled_.assign(size_t(width_)*height_,0xffffff);
   int side=std::max(1,int(std::lround(height_*.9))),left=(width_-side)/2,top=(height_-side)/2;
   for(int y=std::max(0,top);y<std::min(height_,top+side);++y)for(int x=std::max(0,left);x<std::min(width_,left+side);++x){
    double u=std::clamp((x-left+.5)*1000/side-.5,0.0,999.0),v=std::clamp((y-top+.5)*1000/side-.5,0.0,999.0);int ix=int(u),iy=int(v);double fx=u-ix,fy=v-iy;unsigned rgb=0;
    for(int c=0;c<3;++c){auto sample=[&](int sx,int sy){size_t i=(size_t(sy)*1000+sx)*4;return 255.0+(source_[i+c]-255.0)*source_[i+3]/255.0;};double a=sample(ix,iy)*(1-fx)+sample(std::min(ix+1,999),iy)*fx,b=sample(ix,std::min(iy+1,999))*(1-fx)+sample(std::min(ix+1,999),std::min(iy+1,999))*fx;rgb=(rgb<<8)|unsigned(std::lround(a*(1-fy)+b*fy));}scaled_[size_t(y)*width_+x]=rgb;
   }
  }
  double opacity=elapsed<1000?elapsed/1000.0:elapsed<4000?1.0:(5000-elapsed)/1000.0;
  // Smoothstep avoids abrupt velocity changes; compute a 256-entry blend table once per frame.
  opacity=opacity*opacity*(3-2*opacity);unsigned alpha=unsigned(opacity*65536+.5);unsigned lut[256];for(unsigned c=0;c<256;++c)lut[c]=255-(((255-c)*alpha+32768)>>16);
  f.clear();f.border_rgb.resize(scaled_.size());for(size_t i=0;i<scaled_.size();++i){unsigned c=scaled_[i];f.border_rgb[i]=0xff000000u|(lut[(c>>16)&255]<<16)|(lut[(c>>8)&255]<<8)|lut[c&255];}
  return true;
 }
};
}
