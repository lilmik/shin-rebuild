#pragma once
#include "projectiles.hpp"
#include "player_animation.hpp"
#include "collision_world.hpp"
#include <vector>
namespace shin {
struct EnemyMissile {int kind=0;Projectile payload;};
class EnemyMissiles {
 AssetStore&assets_;const MapData&map_;const AppImage&blocks_;
 void ball_terrain(Projectile&,int);
 void later_terrain(Projectile&,int);
public:
 std::vector<EnemyMissile> missiles;
 EnemyMissiles(AssetStore&a,const MapData&m,const AppImage&b):assets_(a),map_(m),blocks_(b){}
 void spawn(int kind,int x,int y,int vx,int vy){if(kind!=0x5b&&kind!=0x5f&&kind!=0x5e)throw std::invalid_argument("enemy missile kind");EnemyMissile m;m.kind=kind;auto&p=m.payload;p.owner_state=0;p.at(4)=x;p.at(8)=y;p.at(0x34)=x;p.at(0x38)=y;p.at(0x3c)=vx;p.at(0x40)=vy;
  const auto&im=assets_.image(kind==0x5b?"ball.app":kind==0x5e?"ghost.raw":"tan_r.app");p.at(12)=im.width;p.at(16)=im.height;p.at(0x14)=kind!=0x5f?2:0;p.at(0x18)=kind!=0x5f?2:0;p.at(0x1c)=kind!=0x5f?12:im.width;p.at(0x20)=kind!=0x5f?12:im.height;missiles.push_back(std::move(m));}
 void tick(int camera_x,int camera_y,IndexedSurface&,bool present=true);
};
}
