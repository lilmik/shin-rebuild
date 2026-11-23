#include "../src/game/player_animation.hpp"
#include "../src/game/level1_springs.hpp"
#include <cassert>
struct Context:shin::game::ObjectResponseContext{
 int value=0,hits=0;int&global(unsigned)override{return value;}int sound(unsigned)override{return 1;}
 int call(unsigned id,shin::PlayerPayload&p,shin::game::EntityPayload&e,std::initializer_list<std::int64_t>)override{
  if(id==0x434ad0)return shin::game::intersects({p.at(0x2c),p.at(0x38)-6,p.at(0x34),p.at(0x38)},p.at(8),p.at(12),{0,0,56,10},e.at(4),e.at(8));
  assert(id==0x434f90);++hits;e.at(0x50)=1;e.at(0x20)=e.at(0x24)=0;e.at(0x28)=5;e.at(0x2c)=10;return 1;
 }
};
int main(int argc,char**argv){assert(argc==2);shin::AssetStore a(argv[1]);for(int type:{0x45,0x59}){
 assert(shin::Level1Springs::supports(type));shin::game::MapPlacement m{5,5,type,0,0};shin::Level1Springs springs(a,{m});shin::PlayerPayload p;shin::initialize_player_payload(p,0,0);shin::IndexedSurface f(853,480);f.origin_x=106;Context c;
 auto&im=a.image(type==0x45?"gspring3.app":"gspring9.app");p.at(8)=m.pixel_x(im.width);p.at(12)=m.pixel_y(im.height)-65;p.at(0x58)=4;
 springs.tick(0,0,f,p,c);springs.tick(0,0,f,p,c);assert(c.hits==1);assert(p.at(0x58)==-35&&p.at(100)==1);
 p.at(8)=2000;for(int i=0;i<50;++i)springs.tick(0,0,f,p,c);assert(c.hits==1);
 }return 0;}
