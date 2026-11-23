#include "enemy_missiles.hpp"
#include <cstdlib>
namespace shin {
void EnemyMissiles::ball_terrain(Projectile&p,int param_2){game::CollisionWorld w(map_,blocks_);game::CollisionRect r={p.at(0x14),p.at(0x18),p.at(0x1c),p.at(0x20)};unsigned budget=65536;auto probe=[&](int x,int y){if(!budget--)throw std::runtime_error("enemy ball collision budget");return w.body(r,x,y);};
#include "enemy_ball_terrain.inc"
}
void EnemyMissiles::later_terrain(Projectile&p,int param_2){game::CollisionWorld w(map_,blocks_);auto probe=[&](int,int x,int y){return w.body({2,2,12,12},x,y);};
#include "later_enemy_missile_terrain.inc"
}
void EnemyMissiles::tick(int cx,int cy,IndexedSurface&f,bool present){game::CollisionWorld w(map_,blocks_);for(auto&m:missiles){auto&p=m.payload;if(p.owner_state==2||std::abs(p.at(4)-cx-320)>=961)continue;
 p.at(0x34)=p.at(4);p.at(0x38)=p.at(8);p.at(0x2c)=p.at(4)-cx;p.at(0x30)=p.at(8)-cy;
 if(present)f.blit(assets_.image(m.kind==0x5b?"ball.app":m.kind==0x5e?"ghost.raw":p.at(0x3c)<0?"tan_l.app":"tan_r.app"),p.at(0x2c),p.at(0x30),true,0,0,-1,-1,m.kind==0x5e&&p.at(0x3c)>0);
 if(m.kind==0x5e){p.at(0x40)=1;p.at(8)+=1;}if(m.kind==0x5b){if(p.at(0x40)<11)++p.at(0x40);p.at(8)+=p.at(0x40);}p.at(4)+=p.at(0x3c);
 game::CollisionRect r={p.at(0x14),p.at(0x18),p.at(0x1c),p.at(0x20)};int material=w.body(r,p.at(4),p.at(8));if(material){if(m.kind==0x5b)ball_terrain(p,material);else if(m.kind==0x5e)later_terrain(p,material);else p.owner_state=2;}
 }}
}
