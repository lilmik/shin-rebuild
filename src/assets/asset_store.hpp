#pragma once
#include "resource_archive.hpp"
#include "rgba_overlay.hpp"
#include "../graphics/indexed_surface.hpp"
#include <array>
#include <map>
#include <string>
namespace shin {
class AssetStore {
 ResourceArchive archive_;
 std::filesystem::path root_;
 std::map<std::string,std::size_t> entries_;
 std::map<std::string,IndexedSurface> images_;
public:
 explicit AssetStore(const std::string& path):archive_(path),root_(std::filesystem::path(path).parent_path()){
  for(std::size_t i=0;i<archive_.entries().size();++i)entries_.emplace(archive_.entries()[i].name,i);
 }
 std::vector<unsigned char> bytes(const std::string& name)const{
  auto it=entries_.find(name);if(it==entries_.end())throw std::runtime_error("missing resource: "+name);
  return archive_.read(it->second);
 }
 const IndexedSurface& image(std::string name){
  auto dot=name.rfind('.');if(dot!=std::string::npos&&name.substr(dot)==".raw")name.replace(dot,4,".app");
  auto it=images_.find(name);if(it!=images_.end())return it->second;
  if(name=="selector_fixed_tapes.app"){
   const auto&background=image("savebg.app");
   if(!background.color_pixels.empty()){
    IndexedSurface fixed(69,117,254);fixed.color_pixels.assign(69*117,0);
    for(int y=0;y<117;++y)for(int x=0;x<69;++x){auto c=background.color_pixels[(199+y)*640+438+x];int r=(c>>16)&255,g=(c>>8)&255,b=c&255;
     if(std::max({r,g,b})-std::min({r,g,b})<=8){fixed.pixels[y*69+x]=0;fixed.color_pixels[y*69+x]=c;}
    }
    return images_.emplace(name,std::move(fixed)).first->second;
   }
   auto original=decode_app(bytes("savebg.app"));auto colors=decode_palette(bytes("shin.pal"));
   IndexedSurface fixed(69,117);fixed.pixels.assign(69*117,254);
   for(int y=0;y<117;++y)for(int x=0;x<69;++x){auto index=original.indices[(199+y)*640+438+x];auto c=colors[index];
    if(std::max({c.red,c.green,c.blue})-std::min({c.red,c.green,c.blue})<=8)fixed.pixels[y*69+x]=index;
   }
   return images_.emplace(name,std::move(fixed)).first->second;
  }
  auto app=decode_app(bytes(name));IndexedSurface surface(int(app.width),int(app.height));surface.pixels=std::move(app.indices);
  // CUSTOM's selected sprite has six stray pixels on its final row, separated
  // from the lettering by two empty rows. Preserve the glyphs and alignment.
  if(name=="ssc3.app"&&surface.width==301&&surface.height==70)
   std::fill(surface.pixels.begin()+69*301+135,surface.pixels.begin()+69*301+141,254);
  if(name=="scrolla.app"||name=="scrollb.app"){
   auto base=decode_app(bytes("scroll.app"));auto colors=decode_palette(bytes("shin.pal"));
   int ox=name=="scrolla.app"?330:483,oy=name=="scrolla.app"?167:164;
   for(int y=0;y<surface.height;++y)for(int x=0;x<surface.width;++x){auto &index=surface.pixels[y*surface.width+x];auto c=colors[index];
    if(index==base.indices[(oy+y)*640+ox+x]||(std::min({c.red,c.green,c.blue})>=190&&std::max({c.red,c.green,c.blue})-std::min({c.red,c.green,c.blue})<=80))index=254;
   }
  }
  if((name=="tt0001.app"||name=="scroll.app"||name=="evframe.app"||name=="gmover.app"||name=="savebg.app")&&surface.width==640&&surface.height==480){
   auto original=decode_app(bytes("tt0001.app"));
   // One complete slanted logo, copied from the original art. Rebuild the
   // wallpaper on a staggered lattice across BOTH the core and extensions.
   auto background=original.indices[0];std::vector<unsigned char> pattern(640*480,background);
   for(int row=-2;row<5;++row)for(int col=-3;col<6;++col){
    int ox=65+col*200+((row%2+2)%2)*20,oy=35+row*160;
    for(int y=0;y<170;++y)for(int x=0;x<190;++x){auto c=original.indices[(y+35)*640+x+260];int dx=ox+x,dy=oy+y;
     int sx=x+260,sy=y+35;bool glyph=(sx>=380&&sx<=441&&sy>=48&&sy<=115)||(sx>=354&&sx<=392&&sy>=88&&sy<=133)||(sx>=308&&sx<=361&&sy>=100&&sy<=162)||(sx>=351&&sx<=367&&sy>=104&&sy<=127)||(sx>=262&&sx<=343&&sy>=130&&sy<=204);
     if(glyph&&c!=background&&dx>=0&&dx<640&&dy>=0&&dy<480)pattern[dy*640+dx]=c;
    }
   }
   if(name=="evframe.app"){
    std::vector<unsigned> counts(256*256,0);
    for(int y=0;y<480;++y)for(int x=0;x<640;++x)if(x<138||x>501||y<114||y>366)++counts[unsigned(original.indices[y*640+x])*256+surface.pixels[y*640+x]];
    std::array<unsigned char,256> colors{};
    for(int i=0;i<256;++i){unsigned best=0;colors[i]=surface.pixels[0];for(int j=0;j<256;++j)if(counts[i*256+j]>best){best=counts[i*256+j];colors[i]=static_cast<unsigned char>(j);}}
    for(auto&c:pattern)c=colors[c];
   }
   if(name=="savebg.app"){
    auto edited=read_rgba_overlay(root_/"overrides"/"savebg.rgba",640,480);
    if(!edited.empty()){
     // Manual artwork replaces the complete foreground. Alpha reveals the
     // existing wallpaper; do not run the old foot cleanup or contour repair.
     auto colors=decode_palette(bytes("shin.pal"));surface.pixels=pattern;surface.color_pixels.resize(pattern.size());
     for(size_t i=0;i<pattern.size();++i){auto bg=colors[pattern[i]];unsigned alpha=edited[i*4+3];
      auto blend=[&](unsigned c,unsigned background){return (c*alpha+background*(255-alpha)+127)/255;};
      surface.color_pixels[i]=0xff000000u|(blend(edited[i*4],bg.red)<<16)|(blend(edited[i*4+1],bg.green)<<8)|blend(edited[i*4+2],bg.blue);
     }
    }else{
    auto untouched=surface.pixels;
    auto colors=decode_palette(bytes("shin.pal"));std::vector<unsigned char> seen(640*480,0);std::vector<int> queue;
    auto add=[&](int i){if(seen[i])return;auto c=colors[surface.pixels[i]];if(c.red<144||c.green<144||c.blue<144||std::max({c.red,c.green,c.blue})-std::min({c.red,c.green,c.blue})>80)return;seen[i]=1;queue.push_back(i);};
    for(int x=0;x<640;++x){add(x);add(479*640+x);}for(int y=0;y<480;++y){add(y*640);add(y*640+639);}
    for(std::size_t n=0;n<queue.size();++n){int i=queue[n],x=i%640,y=i/640;if(x)add(i-1);if(x<639)add(i+1);if(y)add(i-640);if(y<479)add(i+640);}
    for(std::size_t i=0;i<pattern.size();++i)if(seen[i])surface.pixels[i]=pattern[i];
    // Keep the original foot silhouette and its pale contours pixel-for-pixel.
    for(int y=408;y<=420;++y)for(int x=44;x<=142;++x){
     int i=y*640+x;auto c=colors[untouched[i]];
     // Restore only the neutral foot pixels, not the adjacent pastel logo.
     if(std::max({c.red,c.green,c.blue})-std::min({c.red,c.green,c.blue})<=8)
      surface.pixels[i]=untouched[i];
    }
    // Strengthen existing dark contour pixels only, inside the foot silhouette.
    static constexpr int feet[][2]={{54,103},{53,104},{51,131},{52,131},{54,131},{55,131},{56,130},{58,129},{60,125},{63,119},{65,110},{86,96}};
    unsigned char contour=0;int best=1000000;
    for(int k=0;k<256;++k){auto c=colors[k];int d=(int(c.red)-100)*(int(c.red)-100)+(int(c.green)-100)*(int(c.green)-100)+(int(c.blue)-100)*(int(c.blue)-100);if(d<best){best=d;contour=static_cast<unsigned char>(k);}}
    for(int row=0;row<12;++row)for(int x=feet[row][0];x<=feet[row][1];++x){int i=(410+row)*640+x;auto c=colors[untouched[i]];if(std::max({c.red,c.green,c.blue})<=184&&std::max({c.red,c.green,c.blue})-std::min({c.red,c.green,c.blue})<=8)surface.pixels[i]=contour;}
    // Only the upper baked tape is replaced by a selectable sprite.
    auto upper=untouched[100*640+420];
    for(int y=74;y<187;++y)for(int x=383;x<411;++x)surface.pixels[y*640+x]=upper;
    // The two white lower tapes are fixed scenery, not selection sprites.
    for(int y=199;y<316;++y)for(int x=445;x<507;++x)surface.pixels[y*640+x]=untouched[y*640+x];
    }
   }
   surface.wallpaper=pattern;
   for(std::size_t i=0;i<pattern.size();++i)if(name!="savebg.app"&&(name=="tt0001.app"||(name=="gmover.app"?surface.pixels[i]==original.indices[i]:(name=="evframe.app"?(int(i%640)<138||int(i%640)>501||int(i/640)<114||int(i/640)>366):surface.pixels[i]==original.indices[i]))))surface.pixels[i]=pattern[i];
   surface.repeat_x=400;surface.repeat_y=320;
  }
  if(name.rfind("tvnoise",0)==0&&surface.width==138&&surface.height==123){surface.height=120;surface.pixels.resize(138*120);}
  return images_.emplace(name,std::move(surface)).first->second;
 }
 void preload_images(std::initializer_list<const char*> names){for(auto name:names)image(name);}
 // Use the same decoder and repairs as on-demand access; cached surfaces
 // remain owned by this store for its entire lifetime.
 void preload_all_images(){
  for(const auto&entry:entries_)if(entry.first.size()>=4&&entry.first.compare(entry.first.size()-4,4,".app")==0)image(entry.first);
  image("selector_fixed_tapes.app");
 }
 std::array<std::uint32_t,256> palette(const std::string& name)const{
  auto colors=decode_palette(bytes(name));std::array<std::uint32_t,256> out{};
  for(std::size_t i=0;i<256;++i)out[i]=(std::uint32_t(colors[i].red)<<16)|(std::uint32_t(colors[i].green)<<8)|colors[i].blue;
  return out;
 }
 MapData map(const std::string& name)const{return decode_map(bytes(name));}
};
}
