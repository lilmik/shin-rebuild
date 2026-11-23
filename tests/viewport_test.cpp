#include "../src/graphics/viewport.hpp"
#include "../src/graphics/indexed_surface.hpp"
#include <stdexcept>
int main(){
 for(auto size:{std::pair<int,int>{640,480},{1280,720},{1920,1080},{2560,1080},{720,1280}}){
  auto v=shin::ViewportLayout::for_client(size.first,size.second);
  if(v.logical_width<640||v.logical_height<480)throw std::runtime_error("cropped base viewport");
  double sx=double(size.first)/v.logical_width,sy=double(size.second)/v.logical_height;
  if(std::abs(sx-sy)>0.01)throw std::runtime_error("nonuniform scale");
  shin::IndexedSurface frame(v.logical_width,v.logical_height,0);frame.origin_x=v.origin_x;frame.origin_y=v.origin_y;
  shin::IndexedSurface bg(640,480,7);frame.blit(bg,0,0,false);
  for(auto p:frame.pixels)if(p!=7)throw std::runtime_error("unfilled UI margin");
  shin::IndexedSurface sprite(10,10,9);frame.blit(sprite,100,100,false);
  if(frame.pixels[(v.origin_y+100)*frame.width+v.origin_x+100]!=9)throw std::runtime_error("sprite origin");
 }
}
