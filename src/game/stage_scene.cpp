#include "stage_scene.hpp"
#include <stdexcept>
namespace shin::game {
const std::array<StageBinding,7>& stage_bindings(){static const std::array<StageBinding,7> b{{
 {"stage1.map","tile1.app","block1.app","gra.app",0x43e930,0x43fe50,0x440000},
 {"stage2.map","tile2.app","block2.app","gra.app",0x43e9c0,0x41ede0,0x41efa0},
 {"shoot.map","stile.app","sblock.app","gra.app",0x43ea90,0x413e40,0x413f90},
 {"stage3.map","tile3.app","block3.app","gra.app",0x43eb40,0x435be0,0x435d10},
 {"stage4.map","tile4.app","block4.app","gra.app",0x43ebd0,0x4347a0,0x434910},
 {"stage5.map","tile5.app","block5.app","gra.app",0x43ec70,0x43e010,0x43e110},
 {"stage6.map","tile6.app","block6.app","gra.app",0x43ed60,0x4299f0,0x429b40}}};return b;}
static std::vector<unsigned char> read_named(ResourceArchive&a,const std::string&name){for(std::size_t i=0;i<a.entries().size();++i)if(a.entries()[i].name==name)return a.read(i);throw std::runtime_error("missing stage resource: "+name);}
void StageScene::load(ResourceArchive&a,int index){if(index<0||index>=7)throw std::out_of_range("stage index");const auto&b=stage_bindings()[index];auto m=decode_map(read_named(a,b.map));if(m.width>1500||m.height>210)throw std::runtime_error("map exceeds original grid limits");auto t=decode_app(read_named(a,b.tiles));auto c=decode_app(read_named(a,b.collision_blocks));auto g=decode_app(read_named(a,b.background));std::vector<MapPlacement>p;for(unsigned y=0;y<m.height;++y)for(unsigned x=0;x<m.width;++x){const auto&v=m.cells[std::size_t(y)*m.width+x];if(v.raw2!=-1)p.push_back({int(x),int(y),v.raw2,v.raw3,v.raw4});}map=std::move(m);tiles=std::move(t);collision_blocks=std::move(c);background=std::move(g);placements=std::move(p);stage=index;camera={};spawn_cell_x=spawn_cell_y=0;}
int next_saved_progress(int existing,int stage){if(stage<0||stage>=7)throw std::out_of_range("stage index");return existing-std::int64_t(1)<stage?stage+1:existing;}
}
