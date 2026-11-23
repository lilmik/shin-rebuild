#include "../src/game/projectiles.hpp"
#include <vector>
#include <stdexcept>
#include <iostream>
void check(bool b){if(!b)throw std::runtime_error("projectile assertion");}
struct Host:shin::ProjectileContext{
 int cx=0,cy=0,draws=0,detonations=0,drawx=-1;bool show=true;std::vector<unsigned> calls;std::vector<int> releases;
 int camera_x()const override{return cx;}int camera_y()const override{return cy;}bool present()const override{return show;}
 int register_collision(unsigned id,unsigned cb,shin::Projectile&)override{calls.push_back(id);calls.push_back(cb);return int(calls.size());}
 void unregister_collision(unsigned id,int)override{calls.push_back(id);}
 void render(shin::Projectile&p)override{++draws;drawx=p.at(4);}
 int call(unsigned id,shin::Projectile&,std::initializer_list<std::int64_t>)override{if(id==0x431130)return 0;if(id==0x42b300){++detonations;return 0;}throw std::runtime_error("unknown dependency");}
 void entity_collision(unsigned cb,shin::Projectile&,unsigned,void*)override{calls.push_back(cb);}
 void release_allocation(int n)override{releases.push_back(n);}
};
int main(){Host h;shin::Projectile p;p.words.fill(777);shin::initialize_projectile(p,shin::ProjectileKind::ball,100,200,10,14,14,h);
 check(p.at(0)==777&&p.at(0x14)==2&&p.at(0x20)==12&&p.at(0x40)==-10&&p.at(0x44)==0&&h.calls[1]==0x42d810&&h.calls[3]==0x42e130);
 shin::update_projectile(p,h);check(p.at(4)==110&&p.at(8)==191&&h.drawx==100&&p.at(0x34)==100);
 p.at(4)=1281;auto old=p.words;shin::update_projectile(p,h);check(p.words==old&&p.owner_state==1);p.at(4)=1280;shin::update_projectile(p,h);check(p.at(4)==1290);
 p.at(4)=100;p.at(8)=100;p.at(0x40)=5;shin::projectile_terrain_collision(p,1,h);check(p.at(0x40)==-5&&p.at(0x44)==1&&p.owner_state==1);
 shin::projectile_terrain_collision(p,1,h);shin::projectile_terrain_collision(p,1,h);check(p.at(0x44)==3&&p.owner_state==2);
 shin::destroy_projectile(p,999,h);check((h.releases==std::vector<int>{777,999})&&h.calls[h.calls.size()-2]==0x434bb0&&h.calls.back()==0x4310c0);
 shin::Projectile bomb;shin::initialize_projectile(bomb,shin::ProjectileKind::water_bomb,100,100,-10,14,14,h);check(h.calls[h.calls.size()-3]==0x42fe30&&h.calls.back()==0x430a50);
 bomb.at(0x40)=5;shin::projectile_terrain_collision(bomb,2,h);check(bomb.owner_state==2&&bomb.at(0x40)==-7&&h.detonations==1);
 shin::projectile_entity_collision(bomb,0x65,nullptr,h);check(h.calls.back()==0x42fe30);
 std::cout<<"projectile registration/ballistic render order/range freeze/three bounce/bomb detonation/cleanup passed\n";
}
