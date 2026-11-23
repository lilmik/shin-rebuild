#pragma once
#include "stage2_mouse.hpp"
#include <list>
#include <set>
namespace shin::game {
class LaterSpecialEnemies {
 struct Actor{EntityPayload e;int index;};std::list<Actor>actors;std::set<int>seen;
#include "later_special_enemy_data.inc"
 static void pose(EntityPayload&e,int s){if(s==3)e.at(0x5c)=e.at(0x50);e.at(0x50)=s;e.at(0x10)=e.at(0x14)=0;e.at(0x18)=s==0?1:s==3?5:4;e.at(0x1c)=s==0?100:s==4?20:10;}
public:
 struct ProjectileEvent{int x,y,vx,vy;unsigned callback;int kind;};std::vector<ProjectileEvent>projectile_events;std::vector<std::string>sounds;
 static bool supports(int t){return t==0x22;}static const char*placement_image(int){return "chs22xx.raw";}
 static CollisionRect rectangle(const EntityPayload&){return {22,44,66,120};}
 void spawn(int type,int x,int y,int index){if(!supports(type)||!seen.insert(index).second)return;actors.push_back({{},index});auto&e=actors.back().e;e.kind=0x6f;e.at(4)=x;e.at(8)=y;e.at(0x20)=22;e.at(0x24)=44;e.at(0x28)=66;e.at(0x2c)=120;pose(e,1);}
 void spawn_cinematic(int x,int y){spawn(0x22,x,y,-72);for(auto&a:actors)if(a.index==-72){a.e.at(0x50)=5;a.e.at(0x48)=0;}}
 void resume_cinematic(){for(auto&a:actors)if(a.index==-72)pose(a.e,1);}
 std::vector<EntityPayload*>targets(){std::vector<EntityPayload*>out;for(auto&a:actors)if(a.e.state!=2&&a.e.at(0x50)!=4)out.push_back(&a.e);return out;}
 void trigger(EntityPayload&e,int direction){if(e.state!=2&&e.at(0x50)!=4&&e.at(0x50)!=5&&direction>=0&&direction<=2)pose(e,direction);}
 bool hurt(EntityPayload&e){if(e.state==2||e.at(0x50)==4)return false;pose(e,4);sounds.push_back("RESOURCE/punch2.wav");return true;}
 void terrain(EntityPayload&e,const CollisionWorld&world,int param_2){auto probe=[&](int,int x,int y){return world.body(rectangle(e),x,y);};
#include "later_special_enemy_terrain.inc"
 }
 void tick(int cx,int cy,PlayerPayload&p,AssetStore&a,IndexedSurface&f){for(auto&actor:actors){auto&e=actor.e;if(e.state==2)continue;const auto&size=a.image(names[12]);e.at(0x64)=size.width;e.at(0x68)=size.height;e.at(0x40)=e.at(4);e.at(0x44)=e.at(8);int state=e.at(0x50);
 if(state==5){f.blit(a.image("8#211.raw"),e.at(4)-cx,e.at(8)-cy);e.at(0x48)=0;if(e.at(0x4c)<11)++e.at(0x4c);e.at(8)+=e.at(0x4c);continue;}
 if(state<=2){if(e.at(0x58)==200){pose(e,3);e.at(0x58)=0;}if(state==1){e.at(0x48)=-2;e.at(0x60)=0;}if(state==2){e.at(0x48)=2;e.at(0x60)=1;}}
 else if(state==3){if(e.at(0x10)==0&&e.at(0x14)==1)e.at(0x60)=e.at(4)+e.at(0x64)/2<=p.at(8)+p.at(0x10)/2;if(e.at(0x10)==3&&e.at(0x14)==9){int vx=(p.at(8)-e.at(4))/32;if(vx>8)vx=7;if(vx<-8)vx=-7;projectile_events.push_back({e.at(4)+(e.at(0x60)?e.at(0x64)-10:0),e.at(8)+30,vx,-16,0x43b830,0x5e});pose(e,e.at(0x5c));}}
 else if(state==4){if(e.at(0x10)==0&&e.at(0x14)==0)e.at(0x4c)=-10;if(e.at(0x10)==1&&e.at(0x14)==0){e.at(0x4c)=10;e.state=2;}}
 int group=e.at(0x50)==0?0:e.at(0x50)==3?2:e.at(0x50)==4?3:1;f.blit(a.image(names[poses[group][e.at(0x10)]]),e.at(4)-cx,e.at(8)-cy,true,0,0,-1,-1,e.at(0x60)!=0);if(e.at(0x48)>0)--e.at(0x48);if(e.at(0x48)<0)++e.at(0x48);if(e.at(0x4c)<11)++e.at(0x4c);e.at(4)+=e.at(0x48);e.at(8)+=e.at(0x4c);if(++e.at(0x14)>e.at(0x1c)){e.at(0x14)=0;e.at(0x10)=(e.at(0x10)+1)%e.at(0x18);}++e.at(0x58);
 }}
};
}
