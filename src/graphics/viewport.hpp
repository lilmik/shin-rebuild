#pragma once
#include <algorithm>
#include <cmath>
namespace shin {
struct ViewportLayout {
 int logical_width=640,logical_height=480,origin_x=0,origin_y=0;
 double scale=1;
 static ViewportLayout for_client(int w,int h){
  ViewportLayout out;if(w<=0||h<=0)return out;
  out.scale=std::min(double(w)/640,double(h)/480);
  out.logical_width=std::max(640,int(std::lround(w/out.scale)));
  out.logical_height=std::max(480,int(std::lround(h/out.scale)));
  out.origin_x=(out.logical_width-640)/2;out.origin_y=(out.logical_height-480)/2;
  return out;
 }
};
}
