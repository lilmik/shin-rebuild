#pragma once
#include "level1_springs.hpp"
namespace shin {
// MAP5b 441ff0/4420c0 and MAP5e 41fed0/41ffa0.
class LaterSprings {
public:
 struct Spring{int type,index;game::EntityPayload e;};std::vector<Spring>objects;
 struct Respawn{int type,x,y,index;};std::vector<Respawn>respawns;
 static bool supports(int type){return type==0x44||type==0x5b||type==0x5e;}
 static const char*resource(int t){return t==0x44?"dspring1.raw":t==0x5b?"dspring6.raw":"sspring3.raw";}
 void spawn(int type,int x,int y,int index){Spring s{type,index,{}};auto&e=s.e;e.kind=0x40;e.state=0;e.at(4)=e.at(0x40)=x;e.at(8)=e.at(0x44)=y;e.at(0x14)=56;e.at(0x18)=10;e.at(0x28)=1;e.at(0x2c)=10;objects.push_back(s);}
 std::vector<game::EntityPayload*>targets(){std::vector<game::EntityPayload*>r;for(auto&o:objects)if(o.e.state==0)r.push_back(&o.e);return r;}
 void tick(int cx,int cy,AssetStore&a,IndexedSurface&f){for(auto&o:objects){auto&e=o.e;if(e.state==2)continue;if(e.state==1){e.state=0;continue;}if(e.at(0x50)==1&&e.at(0x20)==2&&e.at(0x24)==9){e.at(0x50)=0;e.at(0x20)=e.at(0x24)=0;e.at(0x28)=1;}int base=o.type==0x5b?4:1;int frame=e.at(0x50)==1?e.at(0x20):2;std::string name=(o.type==0x5b||o.type==0x44?"dspring":"sspring")+std::to_string(base+frame)+".raw";f.blit(a.image(name),e.at(4)-cx,e.at(8)-cy);if(e.at(0x48)>0)--e.at(0x48);if(e.at(0x48)<0)++e.at(0x48);e.at(4)+=e.at(0x48);e.at(8)+=e.at(0x4c);if(++e.at(0x24)>e.at(0x2c)){e.at(0x24)=0;e.at(0x20)=(e.at(0x20)+1)%e.at(0x28);}}}
};
}
