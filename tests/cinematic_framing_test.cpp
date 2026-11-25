#include "../src/game/level_runtime.hpp"
#include <iostream>
#include <stdexcept>
namespace shin {struct LevelRuntimeTestAccess {
 static void check(LevelRuntime&r,AssetStore&a,int stage){
  auto require=[](bool ok){if(!ok)throw std::runtime_error("cinematic framing invariant");};
  unsigned now=0;r.clock_ms=[&]{return now;};r.original_sound=[](unsigned){return 1;};
  r.player.at(8)=std::max(700,int(r.scene_.map.width)*16);r.player.at(12)=300;
  r.scene_.camera.pixel_x=std::max(0,r.player.at(8)-580);r.scene_.camera.pixel_y=0;
  int left=r.player.at(8),right=left+r.player.at(0x10);
  if(stage==0){r.dialogues_.start(0x41,r.player,0,r.camera_x(),1280);for(auto&actor:r.dialogues_.actors){left=std::min(left,actor.x);const char*const*names=actor.identity==0?dialogue_data::n0:actor.identity==1?dialogue_data::n1:dialogue_data::n2;right=std::max(right,actor.x+a.image(names[0]).width);}}
  else if(stage==1){r.ensure_later_actors();r.later_actors_->spawn(32,r.player.at(8)+480,250,99999);auto*b=r.later_actors_->boss();r.later_sequence_.begin_intro(*b,0,0,true);right=b->payload.at(4)+a.image("2#421.app").width;}
  else if(stage==2)r.flying_events_.start(76,0);
  else if(stage==3)r.flying_events_.start(77,0);
  else if(stage==4){r.jungle_bosses_.push_back(std::make_unique<JungleBoss>(0x27,r.player.at(8)+300,150,a));r.jungle_bosses_.push_back(std::make_unique<JungleBoss>(0x28,r.player.at(8)+460,150,a));r.jungle_intro_.begin(0,r.player,{});right=r.player.at(8)+460+r.jungle_bosses_.back()->payload.at(0x64);}
  else if(stage==5){r.sixth_scenes_.begin(0x70,0,r.player,{});r.sixth_scenes_.visit_actors([&](int type,int x,int){const char*name=type==0x3b?"mrh1aa.raw":type==0x29?"ana2aa.raw":type==0x27?"7#731.raw":"7#741.raw";left=std::min(left,x);right=std::max(right,x+a.image(name).width);});}
  else {r.ensure_scene_callbacks();r.later_monologues_.start(0x72,r.player,0,[](unsigned){});right=r.player.at(8)+460;}
  const int cx=r.camera_x(),cy=r.camera_y();IndexedSurface f(1280,720);int previous=0;
  for(int i=0;i<500;++i){now+=16;f.origin_x=cx-std::clamp(cx-320,0,std::max(0,int(r.scene_.map.width)*32-f.width));f.origin_y=cy;r.pan_scene_view(f);int view=cx-f.origin_x;if(i)require(std::abs(view-previous)<=6);previous=view;}
  require(r.camera_x()==cx&&r.camera_y()==cy); // Rendering must not alter physics/activation coordinates.
  std::cerr<<"cast="<<left<<","<<right<<" view="<<previous<<","<<previous+f.width<<"\n";require(left>=previous&&right<=previous+f.width);
  IndexedSurface expected(1280,720);expected.origin_x=320;expected.origin_y=120;expected.blit(a.image("evframe.app"),0,0,false);
  f.origin_x=-237;f.origin_y=189;f.clear(99);r.draw_story_panel(f,{{"evframe.app",0,0,false}});
  require(f.pixels==expected.pixels);require(f.origin_x==-237&&f.origin_y==189);
  std::cout<<"stage "<<stage+1<<" cast visible, smooth pan, world camera unchanged\n";
 }
};}
int main(int argc,char**argv){if(argc!=2)return 2;shin::AssetStore a(argv[1]);for(int stage=0;stage<7;++stage){std::cerr<<"checking "<<stage+1<<"\n";try{shin::LevelRuntime r(a,argv[1],stage);shin::LevelRuntimeTestAccess::check(r,a,stage);}catch(const std::exception&e){std::cerr<<e.what()<<"\n";return 1;}}}
