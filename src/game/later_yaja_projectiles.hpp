#pragma once
#include "stage2_mouse.hpp"
namespace shin::game {
class LaterYajaProjectiles {
public:
 std::vector<EntityPayload>targets;
 static CollisionRect rectangle(){return {2,2,16,16};}
 void spawn(int x,int y,int vx,int vy){EntityPayload e;e.kind=0x61;e.at(4)=x;e.at(8)=y;e.at(0x3c)=vx;e.at(0x40)=vy;targets.push_back(e);}
 void terrain(EntityPayload&e,const CollisionWorld&w,int param_2){auto probe=[&](int,int x,int y){return w.body(rectangle(),x,y);};
#include "later_yaja_terrain.inc"
 }
 void tick(int cx,int cy,AssetStore&a,IndexedSurface&f,const CollisionWorld&w){for(auto&e:targets){if(e.state==2)continue;if(std::abs(e.at(4)-cx-320)>960){e.state=2;continue;}e.at(0x34)=e.at(4);e.at(0x38)=e.at(8);f.blit(a.image("yaja.raw"),e.at(4)-cx,e.at(8)-cy);if(e.at(0x40)<11)++e.at(0x40);e.at(4)+=e.at(0x3c);e.at(8)+=e.at(0x40);int mat=w.body(rectangle(),e.at(4),e.at(8));if(mat)terrain(e,w,mat);}targets.erase(std::remove_if(targets.begin(),targets.end(),[](auto&e){return e.state==2;}),targets.end());}
};
}
