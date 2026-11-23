#include "../src/game/level_runtime.hpp"
#include "../src/graphics/viewport.hpp"
#include <cassert>
#include <iostream>

int main(int argc,char**argv){
 if(argc!=2)return 1;
 shin::AssetStore assets(argv[1]);
 for(auto size:{std::pair<int,int>{640,480},{1280,720},{2560,1080}}){
  auto v=shin::ViewportLayout::for_client(size.first,size.second);
  shin::IndexedSurface frame(v.logical_width,v.logical_height,254);
  frame.origin_x=v.origin_x;frame.origin_y=v.origin_y;
  shin::LevelRuntime runtime(assets,argv[1],0);int sounds=0;
  shin::InputState input;runtime.tick(input,frame);
  runtime.original_sound=[&](unsigned id){assert(id==0x77ef54||id==0x77ef58);++sounds;return 1;};
  shin::Level1Objects objects(assets,{});
  shin::Level1Objects::Object object{};object.type=10;object.payload.kind=0x51;
  objects.objects().push_back(object);auto&e=objects.objects().front().payload;
  // A collected item starts at Shiro + (40,40), never at the collision site.
  runtime.player.at(8)=2000;runtime.player.at(12)=1000;
  runtime.call(0x43c0a0,runtime.player,e,{});
  assert(e.at(0x24)==50);assert(e.at(0x28)==440);
  assert(e.at(0x38)==-15&&e.at(0x3c)==1&&sounds==1);
  int x=e.at(0x24),y=440,velocity=-15,frames=0,apex=y;
  while(!objects.objects().front().removed){
   frame.clear(254);shin::IndexedSurface expected=frame;
   x+=e.at(0x34);y+=velocity;if(velocity<11)++velocity;
   expected.origin_x=expected.origin_y=0;expected.blit(assets.image("chocobi.app"),x,y,true);
   objects.tick(1900+frames*3,800,frame,runtime.player,runtime);
   assert(frame.pixels==expected.pixels);
   assert(e.at(0x24)==x&&e.at(0x28)==y&&e.at(0x38)==velocity);
   apex=std::min(apex,y);++frames;assert(frames<100);
  }
  assert(apex==320&&y>480&&e.state==2);
  assert(sounds==1&&runtime.player.at(0x70)==0);
 }
 std::cout<<"Chocobi original 43c0a0/43be80 trajectory, draw phase, HUD origin and camera independence verified\n";
}
