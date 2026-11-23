#pragma once
#include "player_object_response.hpp"
#include "collision_world.hpp"
#include "../assets/asset_store.hpp"
#include <set>
#include <functional>
namespace shin {
// 429290/4293e0/4295b0 and417710/417820/417980.
class LaterMechanisms {
public:
 struct Object{int type,index;game::EntityPayload e;bool activated=false,updating=false;};
 struct Respawn{int type,x,y,index;};
 std::vector<Object> objects;std::vector<Respawn>respawns;
 static bool supports(int t){return t>=0x60&&t<=0x67;}
 static const char*resource(int t){return t<=0x64?"dari1.raw":t==0x67?"dari3.raw":"dari2.raw";}
 static game::CollisionRect rectangle(){return {0,0,62,22};}
 void spawn(int type,int x,int y,int index){if(!supports(type))throw std::invalid_argument("platform type");Object o{type,index,{}};auto&e=o.e;e.kind=type<=0x64?0x4d:type==0x67?0x4f:0x4e;e.state=1;e.at(4)=e.at(0x2c)=x;e.at(8)=e.at(0x30)=y;e.at(0xc)=e.at(0x10)=0;e.at(0x14)=62;e.at(0x18)=22;static constexpr int modes[]={1,2,0,3,4,0,4,0};e.at(0x3c)=modes[type-0x60];objects.push_back(o);}
 std::vector<game::EntityPayload*>targets(){std::vector<game::EntityPayload*>r;for(auto&o:objects)if(o.e.state==0&&o.updating)r.push_back(&o.e);return r;}
 void trigger(game::EntityPayload&e,int direction){if(e.kind==0x4d&&direction>=0&&direction<=4)e.at(0x3c)=direction;}
 void contact(game::EntityPayload&e){if(e.kind==0x4e)e.at(0x3c)=4;}
 // Both original platform terrain callbacks share this resolution order.
 static void terrain(game::EntityPayload&e,const game::CollisionWorld&w,int material){
  if(material!=1&&material!=2)return;auto clear=[&](int x,int y){return w.body(rectangle(),x,y)==0;};
  for(int up=0;up<26;++up)if(clear(e.at(4),e.at(8)-up)){e.at(8)-=up;if(material==2)e.at(0x38)+=2;return;}
  if(material==2){if(e.at(8)<=e.at(0x30))return;for(int y=e.at(8);y>=e.at(0x30);--y)if(clear(e.at(4),y)){e.at(8)=y;return;}return;}
  int x=e.at(4),oldx=e.at(0x2c),oldy=e.at(0x30);
  if(x!=oldx){int step=x>oldx?-1:1;for(;;x+=step){if(clear(x,oldy)){e.at(4)=x;break;}if(x==oldx)break;}}
  int y=e.at(8);if(y!=oldy){int step=y>oldy?-1:1;for(;;y+=step){if(clear(e.at(4),y)){e.at(8)=y;return;}if(y==oldy)break;}}
 }
 void tick(int cx,int cy,AssetStore&a,IndexedSurface&f){for(auto&o:objects){auto&e=o.e;if(e.state==2)continue;o.updating=e.at(4)>=cx-f.origin_x-64&&e.at(4)<cx-f.origin_x+f.width+64&&e.at(8)>=cy-f.origin_y-64&&e.at(8)<cy-f.origin_y+f.height+64;
 if(!o.updating){e.at(0x2c)=e.at(4);e.at(0x30)=e.at(8);continue;}
 o.activated=true;if(e.state==1){e.state=0;continue;}e.at(0x2c)=e.at(4);e.at(0x30)=e.at(8);int mode=e.at(0x3c);e.at(0x34)=mode==1?-1:mode==2?1:0;e.at(0x38)=mode==3?-1:mode==4?1:0;f.blit(a.image(resource(o.type)),e.at(4)-cx,e.at(8)-cy);e.at(4)+=e.at(0x34);e.at(8)+=e.at(0x38);}}
};
}
