#include "../src/game/level_runtime.hpp"
#include <cassert>
namespace shin {struct LevelRuntimeTestAccess{
static void warp(LevelRuntime&l,int type){auto it=std::find_if(l.scene_.placements.begin(),l.scene_.placements.end(),[&](auto&m){return m.type==type;});assert(it!=l.scene_.placements.end());auto&c=l.scene_.camera;c.tile_x=std::max(0,it->cell_x-10);c.offset_x=0;c.pixel_x=c.tile_x*32;c.tile_y=std::max(0,it->cell_y-7);c.offset_y=0;c.pixel_y=c.tile_y*32;l.player.at(8)=c.pixel_x+100;l.player.at(12)=c.pixel_y+240;l.player.at(0xb8)=10000;}
static bool guard(LevelRuntime&l){return !l.large_guards_.empty();}
static void hitguard(LevelRuntime&l){auto&e=l.large_guards_.front().actor->payload;game::EntityPayload s;s.at(4)=e.at(4)+55;s.at(8)=e.at(8)+60;s.at(0x3c)=0;l.flying_.shots.push_back(s);}
static bool dying(LevelRuntime&l){return l.large_guards_.front().actor->payload.at(0x58)==4;}
static bool platforms(LevelRuntime&l){return !l.later_mechanisms_.objects.empty();}
static bool special(LevelRuntime&l){return !l.special_enemies_.targets().empty();}
};}
int main(int argc,char**argv){assert(argc==2);shin::AssetStore a(argv[1]);shin::InputState input;shin::IndexedSurface f(853,480);f.origin_x=106;
auto bind=[](shin::LevelRuntime&l){l.original_sound=[](unsigned){return 1;};l.play_sound_file=[](const auto&){};};
shin::LevelRuntime l(a,argv[1],3);bind(l);shin::LevelRuntimeTestAccess::warp(l,0x14);l.tick(input,f);assert(shin::LevelRuntimeTestAccess::guard(l));shin::LevelRuntimeTestAccess::hitguard(l);l.tick(input,f);assert(shin::LevelRuntimeTestAccess::dying(l));
shin::LevelRuntime p(a,argv[1],5);bind(p);shin::LevelRuntimeTestAccess::warp(p,0x60);p.tick(input,f);assert(shin::LevelRuntimeTestAccess::platforms(p));
shin::LevelRuntime s(a,argv[1],6);bind(s);shin::LevelRuntimeTestAccess::warp(s,0x22);s.tick(input,f);assert(shin::LevelRuntimeTestAccess::special(s));}
