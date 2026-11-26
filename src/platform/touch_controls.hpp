#pragma once
#include <map>
#include <vector>
#include <cstdint>
#include <algorithm>
namespace shin {
// Coordinates are normalized to the full drawable, independent of game camera.
class TouchControls {
 struct Point {float x,y;};
 std::map<int64_t,Point> fingers_;
public:
 struct Button {float x,y,w,h;unsigned key;const char*label;};
 static const std::vector<Button>& buttons(){static const std::vector<Button>b={
 {.035f,.69f,.075f,.13f,37,"<"},{.185f,.69f,.075f,.13f,39,">"},
 {.11f,.56f,.075f,.13f,38,"^"},{.11f,.82f,.075f,.13f,40,"v"},
 {.86f,.74f,.10f,.18f,32,"A"},{.74f,.57f,.10f,.18f,38,"B"},
 {.86f,.07f,.10f,.09f,13,"OK"},{.035f,.07f,.10f,.09f,27,"BACK"},
 {.15f,.07f,.10f,.09f,117,"EXIT"},{.745f,.07f,.10f,.09f,121,"FX"},
 {.63f,.82f,.07f,.10f,112,"1"},{.55f,.82f,.07f,.10f,113,"2"},
 {.47f,.82f,.07f,.10f,114,"3"},{.39f,.82f,.07f,.10f,115,"4"},{.31f,.82f,.07f,.10f,116,"5"}};return b;}
 void finger(int64_t id,float x,float y,bool down){if(down)fingers_[id]={x,y};else fingers_.erase(id);}
 void clear(){fingers_.clear();}
 bool held(unsigned key)const{for(auto&b:buttons())if(b.key==key)for(auto&f:fingers_)if(f.second.x>=b.x&&f.second.x<b.x+b.w&&f.second.y>=b.y&&f.second.y<b.y+b.h)return true;return false;}
 uint32_t state_mask()const{uint32_t mask=0;unsigned i=0;for(auto&b:buttons()){if(held(b.key))mask|=uint32_t(1)<<i;++i;}return mask;}
 std::vector<uint32_t> pixels(int w=960,int h=540)const{
  std::vector<uint32_t>out(size_t(w)*h,0);
  const char*letters="<>^vABOKCEXTF12345";
  const unsigned char glyph[][5]={{8,20,34,0,0},{34,20,8,0,0},{4,2,1,2,4},{1,2,4,2,1},{126,17,17,17,126},{127,73,73,73,54},{62,65,65,65,62},{127,8,20,34,65},{62,65,65,65,34},{127,73,73,73,65},{99,20,8,20,99},{1,1,127,1,1},{127,9,9,9,1},{0,66,127,64,0},{66,97,81,73,70},{33,65,69,75,49},{15,8,8,127,8},{39,69,69,69,57}};
  for(auto&b:buttons()){int x=int(b.x*w),y=int(b.y*h),bw=int(b.w*w),bh=int(b.h*h);bool on=held(b.key);for(int j=0;j<bh;++j)for(int i=0;i<bw;++i){bool edge=i<2||j<2||i>=bw-2||j>=bh-2;out[size_t(y+j)*w+x+i]=edge?0xbbb0bac8u:on?0xb04488ccu:0x70202c40u;}
   int n=0;while(b.label[n])++n;int scale=std::max(1,std::min(bh/12,bw/(n*7))),ox=x+(bw-n*6*scale)/2,oy=y+(bh-7*scale)/2;
   for(int c=0;c<n;++c){const char*at=std::find(letters,letters+sizeof(glyph)/5,b.label[c]);if(at==letters+sizeof(glyph)/5)continue;for(int i=0;i<5;++i)for(int j=0;j<7;++j)if(glyph[at-letters][i]&(1<<j))for(int dy=0;dy<scale;++dy)for(int dx=0;dx<scale;++dx)out[size_t(oy+j*scale+dy)*w+ox+(c*6+i)*scale+dx]=0xffeeeeeeu;}
  }return out;
 }
};
}
