#include "../src/game/level_runtime.hpp"
#include <iostream>
int main(int argc,char**argv){
 if(argc!=2)return 1;shin::AssetStore assets(argv[1]);
 // Each native stage host must retain its map when a hazard reloads it.
 // Stage dispatch and entity completeness are separate obligations.
 constexpr unsigned widths[]={1059,1401,1400,1131,999,1407,1402};
 for(int stage=0;stage<7;++stage){
  shin::LevelRuntime level(assets,argv[1],stage);
  level.original_sound=[](unsigned){return 1;};
  level.play_sound_file=[](const std::string&){};
  auto&p=level.player;p.at(0x8c)=2;p.at(0x5c)=4;p.at(0x1c)=1;p.at(0x20)=4;p.at(0xb4)=1;
  shin::IndexedSurface frame(640,480);shin::InputState input;
  level.tick(input,frame);level.tick(input,frame);
  if(level.global(0x461428)!=stage||level.stage_index()!=stage||level.map_width()!=widths[stage])return 2;
  std::cout<<"stage="<<stage<<" hazard reload completed\n";
 }
}
