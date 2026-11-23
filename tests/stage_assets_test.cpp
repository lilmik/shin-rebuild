#include "../src/game/stage_scene.hpp"
#include "../src/game/collision_world.hpp"
#include <iostream>
#include <stdexcept>
int main(int argc,char**argv){
 try {
 if(argc!=2)return 1;
 shin::ResourceArchive archive(argv[1]);
 constexpr unsigned widths[]={1059,1401,1400,1131,999,1407,1402};
 constexpr unsigned heights[]={100,100,20,100,200,100,100};
 for(int i=0;i<7;++i){
  shin::game::StageScene scene;scene.load(archive,i);
  if(scene.map.width!=widths[i]||scene.map.height!=heights[i])throw std::runtime_error("stage dimensions");
  if(scene.tiles.width!=640||scene.collision_blocks.width!=640)throw std::runtime_error("tile atlas columns");
  if(scene.placements.empty())throw std::runtime_error("no object placements");
  shin::game::CollisionWorld world(scene.map,scene.collision_blocks);
  if(world.sample(-1,-1)!=0)throw std::runtime_error("safe out-of-bounds sample");
  std::cout<<i<<" "<<scene.map.width<<"x"<<scene.map.height<<" placements="<<scene.placements.size()<<std::endl;
 }
 }catch(const std::exception& e){std::cerr<<e.what()<<std::endl;return 1;}
}
