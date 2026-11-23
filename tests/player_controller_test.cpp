#include "../src/game/player_controller.hpp"
#include <map>
#include <vector>
#include <stdexcept>
#include <iostream>
struct Host final:shin::PlayerContext {
 std::map<unsigned,int> globals;std::vector<std::pair<unsigned,std::vector<long long>>> effects;int ground=1,side=0,other=0,renders=0;int renderx=-1;
 int& global(unsigned a)override{return globals[a];}int camera_x()const override{return 0;}int camera_y()const override{return 0;}
 int call(unsigned id,shin::PlayerPayload&,std::initializer_list<std::int64_t> args)override{
  effects.push_back({id,{args.begin(),args.end()}});if(id==0x431d60)return ground;if(id==0x431f90)return side;if(id==0x4320c0)return other;
  switch(id){case 0x42d640:case 0x42fc60:case 0x44d440:case 0x44d790:case 0x427900:case 0x42eff0:case 0x7922c8:case 0x43f1d0:case 0x4321f0:case 0x449350:case 0x446c30:return 0;default:throw std::runtime_error("unexpected unimplemented dependency");}
 }
 void render(shin::PlayerPayload&p)override{++renders;renderx=p.at(8);}
 Host(){globals[0x45c0a8]=2;globals[0x46c500]=2;}
};
void check(bool v){if(!v)throw std::runtime_error("controller assertion");}
shin::PlayerPayload base(int state=0){shin::PlayerPayload p;p.at(0x5c)=state;p.at(8)=100;p.at(0xc)=100;p.at(0x24)=1;p.at(0x28)=100;p.at(0x88)=1;return p;}
int main(){
 {Host h;auto p=base();h.global(0x46d4b0)=1;shin::update_player(p,h);check(p.at(0x5c)==2&&p.at(0x58)==-14&&p.at(0xc)==86&&p.at(0x94)==1);}
 {Host h;h.ground=0;auto p=base();shin::update_player(p,h);check(p.at(0x5c)==2&&p.at(0x58)==5&&p.at(0xc)==105);}
 {Host h;auto p=base(2);p.at(0x58)=3;shin::update_player(p,h);check(p.at(0x5c)==5&&p.at(0x58)==1);}
 {Host h;auto p=base();h.global(0x46d4e8)=1;p.at(0xe0)=2;shin::update_player(p,h);check(p.at(0xe0)==1&&p.at(0xe8)==1);auto n=h.effects.size();shin::update_player(p,h);check(p.at(0xe0)==1&&h.effects.size()==n+1);}
 {Host h;auto p=base(11);h.global(0x46cd04)=1;h.global(0x46d4e8)=1;p.at(0xe4)=2;p.at(0xe8)=0;shin::update_player(p,h);check(p.at(0xe4)==2);shin::update_player(p,h);check(p.at(0xe4)==1);}
 {Host h;auto p=base(4);p.at(0x1c)=1;p.at(0x20)=4;p.at(0x8c)=1;shin::update_player(p,h);check(p.owner_state==2);int group=0;for(auto&e:h.effects)if(e.first==0x44d440)check(e.second[0]==group++);check(group==10);}
 {Host h;auto p=base();h.global(0x46d4ac)=1;p.at(0x54)=4;p.at(0x100)=1;shin::update_player(p,h);check(h.renders==1&&h.renderx==100&&p.at(8)==103);}
 {Host h;auto p=base(94);p.at(0x104)=39;shin::update_player(p,h);check(p.at(0x5c)==12&&p.at(0x104)==0);}
 std::cout<<"controller jumps/falls/landing/projectile edge/legacy case11/death/render ordering/state94 passed\n";
}
