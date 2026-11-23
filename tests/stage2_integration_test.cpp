#include "../src/game/level_runtime.hpp"
#include <cassert>
#include <iostream>
namespace shin {struct LevelRuntimeTestAccess {static void rearm(LevelRuntime&l){std::fill(l.spawned_.begin(),l.spawned_.end(),false);}
static void align(LevelRuntime&l){auto&c=l.scene_.camera;c.pixel_x=std::max(0,l.player.at(8)-320);c.pixel_y=std::max(0,l.player.at(12)-240);c.tile_x=c.pixel_x/32;c.offset_x=c.pixel_x%32;c.tile_y=c.pixel_y/32;c.offset_y=c.pixel_y%32;l.player.at(0x44)=l.player.at(8)-c.pixel_x;l.player.at(0x48)=l.player.at(12)-c.pixel_y;}};}
int main(int argc,char**argv){
 assert(argc==2);shin::AssetStore assets(argv[1]);shin::ResourceArchive archive(argv[1]);shin::game::StageScene scene;scene.load(archive,1);
 for(int type:{0x46,0x47,0x48,0x4a,0x57}){
  auto it=std::find_if(scene.placements.begin(),scene.placements.end(),[&](auto&m){return m.type==type;});assert(it!=scene.placements.end());
  shin::LevelRuntime level(assets,argv[1],1);unsigned now=0,track=0;std::string palette;
  level.clock_ms=[&]{return now;};level.original_sound=[](unsigned){return 1;};level.play_sound_file=[](const auto&){};level.play_music=[&](unsigned n){track=n;};level.change_palette=[&](const char*n){palette=n;};
  shin::IndexedSurface frame(853,480);frame.origin_x=106;shin::InputState input;
  auto position=[&]{level.player.at(8)=it->pixel_x(type==0x57?79:32)-20;level.player.at(12)=it->pixel_y(type==0x57?91:32)-(type==0x57?30:240);level.player.at(0x54)=level.player.at(0x58)=0;shin::LevelRuntimeTestAccess::align(level);};
  for(int i=0;i<3&&!level.subgame_active()&&!level.scene_event_active();++i){position();now+=16;level.tick(input,frame);}
  if(type==0x57){assert(level.subgame_active());int x=level.player.at(8),y=level.player.at(12);now+=16;level.tick(input,frame);assert(palette=="mole.pal"&&track==12);assert(level.player.at(8)==x&&level.player.at(12)==y);input.message(0x100,117);now+=16;level.tick(input,frame);assert(!level.subgame_active()&&palette=="shin.pal"&&track==5);}
  else {std::cerr<<"type="<<type<<" x="<<level.player.at(8)<<" y="<<level.player.at(12)<<" trigger="<<it->pixel_x(32)<<","<<it->pixel_y(32)<<"\n";assert(level.scene_event_active());input.message(0x100,13);now+=16;level.tick(input,frame);input.message(0x101,13);now+=16;level.tick(input,frame);assert(!level.scene_event_active());shin::LevelRuntimeTestAccess::rearm(level);for(int repeat=0;repeat<6;++repeat){position();now+=16;level.tick(input,frame);assert(!level.scene_event_active());}}
 }
}
