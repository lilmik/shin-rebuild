#pragma once
#include "player_object_response.hpp"
#include "collision_world.hpp"
#include "../assets/asset_store.hpp"
#include <cstdlib>
namespace shin::game {
// 402350/402440/402530; same actor5b code as ordinary ball but distinct bball art.
class Stage2Projectiles {
 AssetStore&assets_;CollisionWorld world_;
 void terrain(EntityPayload&p,int param_2){unsigned budget=65536;auto probe=[&](int,int x,int y){if(!budget--)throw std::runtime_error("stage2 ball terrain budget");return world_.body({2,2,12,12},x,y);};
#include "stage2_ball_terrain.inc"
 }
public:
 std::vector<EntityPayload>targets;
 Stage2Projectiles(AssetStore&a,const MapData&m,const AppImage&b):assets_(a),world_(m,b){}
 void spawn(int x,int y,int vx,int vy){EntityPayload p;p.kind=0x5b;p.at(4)=p.at(0x34)=x;p.at(8)=p.at(0x38)=y;p.at(0x3c)=vx;p.at(0x40)=vy;auto&im=assets_.image("bball.app");p.at(12)=im.width;p.at(16)=im.height;p.at(0x14)=2;p.at(0x18)=2;p.at(0x1c)=12;p.at(0x20)=12;targets.push_back(p);}
 void tick(int cx,int cy,IndexedSurface&f){for(auto&p:targets){if(p.state==2||std::abs(p.at(4)-cx-320)>=961)continue;p.at(0x34)=p.at(4);p.at(0x38)=p.at(8);p.at(0x2c)=p.at(4)-cx;p.at(0x30)=p.at(8)-cy;f.blit(assets_.image("bball.app"),p.at(0x2c),p.at(0x30));if(p.at(0x40)<11)++p.at(0x40);p.at(4)+=p.at(0x3c);p.at(8)+=p.at(0x40);int material=world_.body({2,2,12,12},p.at(4),p.at(8));if(material)terrain(p,material);}targets.erase(std::remove_if(targets.begin(),targets.end(),[](const auto&p){return p.state==2;}),targets.end());}
};
}
