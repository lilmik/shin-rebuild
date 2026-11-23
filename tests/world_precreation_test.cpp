#include "../src/game/level_runtime.hpp"
#include <iostream>
#include <stdexcept>
namespace shin {
struct LevelRuntimeTestAccess {
 static void check(LevelRuntime&r){
  int ordinary=0,later=0,springs=0,static_icons=0,emitters=0,fire=0;
  for(auto&m:r.scene_.placements){
   ordinary+=Level1Objects::supports(m.type);
   later+=LaterMechanisms::supports(m.type);
   springs+=Level1Springs::supports(m.type)||LaterSprings::supports(m.type);
   static_icons+=Level1SceneObjects::supports(m.type);
   emitters+=m.type==0x23;fire+=m.type==0x5f;
  }
  auto require=[](bool ok){if(!ok)throw std::runtime_error("precreation invariant failed");};
  require(r.objects_->objects().size()==std::size_t(ordinary));
  require(r.later_mechanisms_.objects.size()==std::size_t(later));
  require(r.springs_->size()+r.later_springs_.objects.size()==std::size_t(springs));
  require(r.scene_objects_.objects.size()==std::size_t(static_icons));
  if(r.scene_.stage==2){int clouds=0;for(auto&m:r.scene_.placements)clouds+=m.type==0x3a;require(r.flying_scenery_.active_count()==std::size_t(clouds));}
  if(r.scene_.stage==6)require(r.fixed_scenery_.size()==1);
  require(r.emitters_.objects.size()==std::size_t(emitters));require(r.fire_traps_.objects.size()==std::size_t(fire));
  for(auto&o:r.objects_->objects()){auto&m=r.scene_.placements[o.placement];auto&im=r.assets_.image(map_placement_asset(m.type));require(o.payload.at(4)==m.pixel_x(im.width)&&o.payload.at(8)==m.pixel_y(im.height));}
  for(auto&o:r.later_mechanisms_.objects){auto&m=r.scene_.placements[o.index];auto&im=r.assets_.image(map_placement_asset(m.type));require(o.e.at(4)==m.pixel_x(im.width)&&o.e.at(8)==m.pixel_y(im.height));}
  IndexedSurface frame(640,480);for(auto&o:r.later_mechanisms_.objects){int x=o.e.at(4),y=o.e.at(8);r.later_mechanisms_.tick(x,y,r.assets_,frame);r.later_mechanisms_.tick(x,y,r.assets_,frame);int savedx=o.e.at(4),savedy=o.e.at(8),mode=o.e.at(0x3c);for(int n=0;n<3;++n)r.later_mechanisms_.tick(1000000,1000000,r.assets_,frame);require(o.e.state!=2&&o.e.at(4)==savedx&&o.e.at(8)==savedy&&o.e.at(0x3c)==mode);}
  r.player.at(8)=r.player.at(12)=10000000;
  for(auto&o:r.objects_->objects())if((o.type>=3&&o.type<=5)||o.type==0x31||o.type==0x32){
   int x=o.payload.at(4),y=o.payload.at(8);
   r.objects_->tick(x,y,frame,r.player,r);r.objects_->tick(x,y,frame,r.player,r);
   int savedx=o.payload.at(4),savedy=o.payload.at(8),mode=o.payload.at(0x3c);
   r.objects_->tick(1000000,1000000,frame,r.player,r);
   require(!o.removed&&o.payload.state!=2&&o.payload.at(4)==savedx&&o.payload.at(8)==savedy&&o.payload.at(0x3c)==mode);
  }
  require(r.later_mechanisms_.respawns.empty());
  std::cout<<r.scene_.stage+1<<","<<ordinary<<","<<later<<","<<springs<<","<<static_icons<<","<<emitters<<","<<fire<<"\n";
 }
 static void run(LevelRuntime&r){check(r);r.reset_world();check(r);}
};
}
int main(int argc,char**argv){if(argc!=2)return 1;try{shin::AssetStore assets(argv[1]);std::cout<<"stage,objects,ruin_platforms,springs,icons,emitters,fire\n";for(int s=0;s<7;++s){shin::LevelRuntime r(assets,argv[1],s);shin::LevelRuntimeTestAccess::run(r);}return 0;}catch(const std::exception&e){std::cerr<<e.what()<<"\n";return 2;}}
