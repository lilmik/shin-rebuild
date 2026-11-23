#include "../src/game/level_runtime.hpp"
#include <cassert>
namespace shin {struct LevelRuntimeTestAccess{
static void aim(LevelRuntime&l,int type){auto it=std::find_if(l.scene_.placements.begin(),l.scene_.placements.end(),[&](auto&m){return m.type==type;});assert(it!=l.scene_.placements.end());auto&c=l.scene_.camera;c.tile_x=std::max(0,it->cell_x-12);c.tile_y=std::max(0,it->cell_y-6);c.offset_x=c.offset_y=0;c.pixel_x=c.tile_x*32;c.pixel_y=c.tile_y*32;l.player.at(8)=c.pixel_x+80;l.player.at(12)=c.pixel_y+240;l.player.at(0xb8)=10000;}
static std::size_t enemies(LevelRuntime&l){return l.flying_enemies_.targets().size();}
static void collect(LevelRuntime&l){l.objects_->spawn_visible(l.camera_x(),l.camera_y());auto&v=l.objects_->objects();assert(!v.empty());auto&e=v.front().payload;e.state=0;e.at(4)=l.player.at(8);e.at(8)=l.player.at(12);}
static FlyingBoss*boss(LevelRuntime&l){return l.flying_boss_.get();}
static void contact(LevelRuntime&l){auto targets=l.flying_enemies_.targets();assert(!targets.empty());auto*e=targets.front();l.player.at(8)=e->at(4);l.player.at(12)=e->at(8);l.player.at(0xb8)=0;}
};}
int main(int argc,char**argv){assert(argc==2);shin::AssetStore a(argv[1]);shin::LevelRuntime l(a,argv[1],2);l.original_sound=[](unsigned){return 1;};shin::IndexedSurface f(853,480);f.origin_x=106;shin::InputState in;
shin::LevelRuntimeTestAccess::aim(l,0x37);l.tick(in,f);assert(shin::LevelRuntimeTestAccess::enemies(l)>0);shin::LevelRuntimeTestAccess::contact(l);int score=l.player.at(0x70);l.tick(in,f);assert(l.player.at(0x5c)==0x13&&l.player.at(0x70)==score+1);assert(f.origin_x==106);shin::LevelRuntimeTestAccess::collect(l);score=l.player.at(0x70);l.tick(in,f);assert(l.player.at(0x70)>score&&l.player.at(0x74)>0);
shin::LevelRuntime bosslevel(a,argv[1],2);bosslevel.original_sound=[](unsigned){return 1;};shin::LevelRuntimeTestAccess::aim(bosslevel,0x21);bosslevel.tick(in,f);auto*b=shin::LevelRuntimeTestAccess::boss(bosslevel);assert(b&&b->payload.at(0x6c)==20);bosslevel.tick(in,f);int cx=bosslevel.camera_x();for(int hp=20;hp>0;--hp){assert(b->hurt());for(int i=0;i<22;++i)bosslevel.tick(in,f);}assert(bosslevel.completed()&&bosslevel.camera_x()==cx&&f.origin_x==106);
}
