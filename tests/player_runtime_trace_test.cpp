#include "../src/game/player_animation.hpp"
#include "../src/game/collision_world.hpp"
#include "../src/game/player_world_response.hpp"
#include <map>
#include <iostream>
struct Context:shin::PlayerContext{
 shin::game::CollisionWorld& world;std::map<unsigned,int> globals;
 Context(shin::game::CollisionWorld&w):world(w){globals[0x45c0a8]=2;globals[0x46c500]=2;globals[0x46142c]=-1;}
 int&global(unsigned a)override{return globals[a];}int camera_x()const override{return 0;}int camera_y()const override{return 0;}
 void render(shin::PlayerPayload&)override{throw std::runtime_error("unexpected render request");}
 int call(unsigned a,shin::PlayerPayload&p,std::initializer_list<std::int64_t>args)override{
  auto v=std::vector<std::int64_t>(args);shin::game::CollisionRect r={p.at(0x2c),p.at(0x30),p.at(0x34),p.at(0x38)};
  if(a==0x431d60)return world.ground(r,int(v.at(1)),int(v.at(2)));
  if(a==0x431f90)return world.left(r,int(v.at(1)),int(v.at(2)));
  if(a==0x4320c0)return world.right(r,int(v.at(1)),int(v.at(2)));
  throw std::runtime_error("unexpected host dependency");
 }
};
int main(int argc,char**argv){if(argc!=2)return 1;shin::AssetStore assets(argv[1]);auto map=assets.map("stage1.map");auto block=shin::decode_app(assets.bytes("block1.app"));shin::game::CollisionWorld world(map,block);Context ctx(world);shin::PlayerPayload p;shin::initialize_player_payload(p,0,0);
// Original runtime state before tick 1.
if(p.at(8)!=0){std::cerr<<"tick 1, offset 0x8: "<<p.at(8)<<" != 0\n";return 2;}
if(p.at(12)!=0){std::cerr<<"tick 1, offset 0xc: "<<p.at(12)<<" != 0\n";return 2;}
if(p.at(16)!=76){std::cerr<<"tick 1, offset 0x10: "<<p.at(16)<<" != 76\n";return 2;}
if(p.at(20)!=72){std::cerr<<"tick 1, offset 0x14: "<<p.at(20)<<" != 72\n";return 2;}
if(p.at(24)!=4572948){std::cerr<<"tick 1, offset 0x18: "<<p.at(24)<<" != 4572948\n";return 2;}
if(p.at(28)!=0){std::cerr<<"tick 1, offset 0x1c: "<<p.at(28)<<" != 0\n";return 2;}
if(p.at(32)!=0){std::cerr<<"tick 1, offset 0x20: "<<p.at(32)<<" != 0\n";return 2;}
if(p.at(36)!=1){std::cerr<<"tick 1, offset 0x24: "<<p.at(36)<<" != 1\n";return 2;}
if(p.at(40)!=100){std::cerr<<"tick 1, offset 0x28: "<<p.at(40)<<" != 100\n";return 2;}
if(p.at(44)!=24){std::cerr<<"tick 1, offset 0x2c: "<<p.at(44)<<" != 24\n";return 2;}
if(p.at(48)!=14){std::cerr<<"tick 1, offset 0x30: "<<p.at(48)<<" != 14\n";return 2;}
if(p.at(52)!=50){std::cerr<<"tick 1, offset 0x34: "<<p.at(52)<<" != 50\n";return 2;}
if(p.at(56)!=71){std::cerr<<"tick 1, offset 0x38: "<<p.at(56)<<" != 71\n";return 2;}
if(p.at(68)!=0){std::cerr<<"tick 1, offset 0x44: "<<p.at(68)<<" != 0\n";return 2;}
if(p.at(72)!=0){std::cerr<<"tick 1, offset 0x48: "<<p.at(72)<<" != 0\n";return 2;}
if(p.at(76)!=0){std::cerr<<"tick 1, offset 0x4c: "<<p.at(76)<<" != 0\n";return 2;}
if(p.at(80)!=0){std::cerr<<"tick 1, offset 0x50: "<<p.at(80)<<" != 0\n";return 2;}
if(p.at(84)!=0){std::cerr<<"tick 1, offset 0x54: "<<p.at(84)<<" != 0\n";return 2;}
if(p.at(88)!=0){std::cerr<<"tick 1, offset 0x58: "<<p.at(88)<<" != 0\n";return 2;}
if(p.at(92)!=0){std::cerr<<"tick 1, offset 0x5c: "<<p.at(92)<<" != 0\n";return 2;}
if(p.at(96)!=0){std::cerr<<"tick 1, offset 0x60: "<<p.at(96)<<" != 0\n";return 2;}
if(p.at(100)!=0){std::cerr<<"tick 1, offset 0x64: "<<p.at(100)<<" != 0\n";return 2;}
if(p.at(104)!=0){std::cerr<<"tick 1, offset 0x68: "<<p.at(104)<<" != 0\n";return 2;}
if(p.at(112)!=0){std::cerr<<"tick 1, offset 0x70: "<<p.at(112)<<" != 0\n";return 2;}
if(p.at(116)!=0){std::cerr<<"tick 1, offset 0x74: "<<p.at(116)<<" != 0\n";return 2;}
if(p.at(136)!=1){std::cerr<<"tick 1, offset 0x88: "<<p.at(136)<<" != 1\n";return 2;}
if(p.at(140)!=5){std::cerr<<"tick 1, offset 0x8c: "<<p.at(140)<<" != 5\n";return 2;}
if(p.at(144)!=0){std::cerr<<"tick 1, offset 0x90: "<<p.at(144)<<" != 0\n";return 2;}
if(p.at(148)!=0){std::cerr<<"tick 1, offset 0x94: "<<p.at(148)<<" != 0\n";return 2;}
if(p.at(152)!=0){std::cerr<<"tick 1, offset 0x98: "<<p.at(152)<<" != 0\n";return 2;}
if(p.at(176)!=3){std::cerr<<"tick 1, offset 0xb0: "<<p.at(176)<<" != 3\n";return 2;}
if(p.at(180)!=0){std::cerr<<"tick 1, offset 0xb4: "<<p.at(180)<<" != 0\n";return 2;}
if(p.at(188)!=0){std::cerr<<"tick 1, offset 0xbc: "<<p.at(188)<<" != 0\n";return 2;}
if(p.at(192)!=0){std::cerr<<"tick 1, offset 0xc0: "<<p.at(192)<<" != 0\n";return 2;}
if(p.at(196)!=0){std::cerr<<"tick 1, offset 0xc4: "<<p.at(196)<<" != 0\n";return 2;}
if(p.at(200)!=0){std::cerr<<"tick 1, offset 0xc8: "<<p.at(200)<<" != 0\n";return 2;}
if(p.at(204)!=0){std::cerr<<"tick 1, offset 0xcc: "<<p.at(204)<<" != 0\n";return 2;}
if(p.at(208)!=0){std::cerr<<"tick 1, offset 0xd0: "<<p.at(208)<<" != 0\n";return 2;}
if(p.at(212)!=0){std::cerr<<"tick 1, offset 0xd4: "<<p.at(212)<<" != 0\n";return 2;}
if(p.at(216)!=0){std::cerr<<"tick 1, offset 0xd8: "<<p.at(216)<<" != 0\n";return 2;}
if(p.at(220)!=0){std::cerr<<"tick 1, offset 0xdc: "<<p.at(220)<<" != 0\n";return 2;}
if(p.at(224)!=0){std::cerr<<"tick 1, offset 0xe0: "<<p.at(224)<<" != 0\n";return 2;}
if(p.at(228)!=0){std::cerr<<"tick 1, offset 0xe4: "<<p.at(228)<<" != 0\n";return 2;}
if(p.at(232)!=0){std::cerr<<"tick 1, offset 0xe8: "<<p.at(232)<<" != 0\n";return 2;}
if(p.at(236)!=0){std::cerr<<"tick 1, offset 0xec: "<<p.at(236)<<" != 0\n";return 2;}
if(p.at(240)!=0){std::cerr<<"tick 1, offset 0xf0: "<<p.at(240)<<" != 0\n";return 2;}
if(p.at(244)!=0){std::cerr<<"tick 1, offset 0xf4: "<<p.at(244)<<" != 0\n";return 2;}
if(p.at(252)!=0){std::cerr<<"tick 1, offset 0xfc: "<<p.at(252)<<" != 0\n";return 2;}
if(p.at(256)!=0){std::cerr<<"tick 1, offset 0x100: "<<p.at(256)<<" != 0\n";return 2;}
if(p.at(260)!=0){std::cerr<<"tick 1, offset 0x104: "<<p.at(260)<<" != 0\n";return 2;}
shin::update_player(p,ctx);
// Original runtime state before tick 2.
if(p.at(8)!=0){std::cerr<<"tick 2, offset 0x8: "<<p.at(8)<<" != 0\n";return 2;}
if(p.at(12)!=5){std::cerr<<"tick 2, offset 0xc: "<<p.at(12)<<" != 5\n";return 2;}
if(p.at(16)!=76){std::cerr<<"tick 2, offset 0x10: "<<p.at(16)<<" != 76\n";return 2;}
if(p.at(20)!=72){std::cerr<<"tick 2, offset 0x14: "<<p.at(20)<<" != 72\n";return 2;}
if(p.at(24)!=4572968){std::cerr<<"tick 2, offset 0x18: "<<p.at(24)<<" != 4572968\n";return 2;}
if(p.at(28)!=0){std::cerr<<"tick 2, offset 0x1c: "<<p.at(28)<<" != 0\n";return 2;}
if(p.at(32)!=1){std::cerr<<"tick 2, offset 0x20: "<<p.at(32)<<" != 1\n";return 2;}
if(p.at(36)!=2){std::cerr<<"tick 2, offset 0x24: "<<p.at(36)<<" != 2\n";return 2;}
if(p.at(40)!=10){std::cerr<<"tick 2, offset 0x28: "<<p.at(40)<<" != 10\n";return 2;}
if(p.at(44)!=24){std::cerr<<"tick 2, offset 0x2c: "<<p.at(44)<<" != 24\n";return 2;}
if(p.at(48)!=14){std::cerr<<"tick 2, offset 0x30: "<<p.at(48)<<" != 14\n";return 2;}
if(p.at(52)!=50){std::cerr<<"tick 2, offset 0x34: "<<p.at(52)<<" != 50\n";return 2;}
if(p.at(56)!=71){std::cerr<<"tick 2, offset 0x38: "<<p.at(56)<<" != 71\n";return 2;}
if(p.at(68)!=0){std::cerr<<"tick 2, offset 0x44: "<<p.at(68)<<" != 0\n";return 2;}
if(p.at(72)!=0){std::cerr<<"tick 2, offset 0x48: "<<p.at(72)<<" != 0\n";return 2;}
if(p.at(76)!=0){std::cerr<<"tick 2, offset 0x4c: "<<p.at(76)<<" != 0\n";return 2;}
if(p.at(80)!=0){std::cerr<<"tick 2, offset 0x50: "<<p.at(80)<<" != 0\n";return 2;}
if(p.at(84)!=0){std::cerr<<"tick 2, offset 0x54: "<<p.at(84)<<" != 0\n";return 2;}
if(p.at(88)!=5){std::cerr<<"tick 2, offset 0x58: "<<p.at(88)<<" != 5\n";return 2;}
if(p.at(92)!=2){std::cerr<<"tick 2, offset 0x5c: "<<p.at(92)<<" != 2\n";return 2;}
if(p.at(96)!=0){std::cerr<<"tick 2, offset 0x60: "<<p.at(96)<<" != 0\n";return 2;}
if(p.at(100)!=0){std::cerr<<"tick 2, offset 0x64: "<<p.at(100)<<" != 0\n";return 2;}
if(p.at(104)!=0){std::cerr<<"tick 2, offset 0x68: "<<p.at(104)<<" != 0\n";return 2;}
if(p.at(112)!=0){std::cerr<<"tick 2, offset 0x70: "<<p.at(112)<<" != 0\n";return 2;}
if(p.at(116)!=0){std::cerr<<"tick 2, offset 0x74: "<<p.at(116)<<" != 0\n";return 2;}
if(p.at(136)!=1){std::cerr<<"tick 2, offset 0x88: "<<p.at(136)<<" != 1\n";return 2;}
if(p.at(140)!=5){std::cerr<<"tick 2, offset 0x8c: "<<p.at(140)<<" != 5\n";return 2;}
if(p.at(144)!=0){std::cerr<<"tick 2, offset 0x90: "<<p.at(144)<<" != 0\n";return 2;}
if(p.at(148)!=0){std::cerr<<"tick 2, offset 0x94: "<<p.at(148)<<" != 0\n";return 2;}
if(p.at(152)!=0){std::cerr<<"tick 2, offset 0x98: "<<p.at(152)<<" != 0\n";return 2;}
if(p.at(176)!=3){std::cerr<<"tick 2, offset 0xb0: "<<p.at(176)<<" != 3\n";return 2;}
if(p.at(180)!=0){std::cerr<<"tick 2, offset 0xb4: "<<p.at(180)<<" != 0\n";return 2;}
if(p.at(188)!=0){std::cerr<<"tick 2, offset 0xbc: "<<p.at(188)<<" != 0\n";return 2;}
if(p.at(192)!=0){std::cerr<<"tick 2, offset 0xc0: "<<p.at(192)<<" != 0\n";return 2;}
if(p.at(196)!=0){std::cerr<<"tick 2, offset 0xc4: "<<p.at(196)<<" != 0\n";return 2;}
if(p.at(200)!=0){std::cerr<<"tick 2, offset 0xc8: "<<p.at(200)<<" != 0\n";return 2;}
if(p.at(204)!=0){std::cerr<<"tick 2, offset 0xcc: "<<p.at(204)<<" != 0\n";return 2;}
if(p.at(208)!=0){std::cerr<<"tick 2, offset 0xd0: "<<p.at(208)<<" != 0\n";return 2;}
if(p.at(212)!=0){std::cerr<<"tick 2, offset 0xd4: "<<p.at(212)<<" != 0\n";return 2;}
if(p.at(216)!=0){std::cerr<<"tick 2, offset 0xd8: "<<p.at(216)<<" != 0\n";return 2;}
if(p.at(220)!=0){std::cerr<<"tick 2, offset 0xdc: "<<p.at(220)<<" != 0\n";return 2;}
if(p.at(224)!=0){std::cerr<<"tick 2, offset 0xe0: "<<p.at(224)<<" != 0\n";return 2;}
if(p.at(228)!=0){std::cerr<<"tick 2, offset 0xe4: "<<p.at(228)<<" != 0\n";return 2;}
if(p.at(232)!=0){std::cerr<<"tick 2, offset 0xe8: "<<p.at(232)<<" != 0\n";return 2;}
if(p.at(236)!=0){std::cerr<<"tick 2, offset 0xec: "<<p.at(236)<<" != 0\n";return 2;}
if(p.at(240)!=0){std::cerr<<"tick 2, offset 0xf0: "<<p.at(240)<<" != 0\n";return 2;}
if(p.at(244)!=0){std::cerr<<"tick 2, offset 0xf4: "<<p.at(244)<<" != 0\n";return 2;}
if(p.at(252)!=0){std::cerr<<"tick 2, offset 0xfc: "<<p.at(252)<<" != 0\n";return 2;}
if(p.at(256)!=0){std::cerr<<"tick 2, offset 0x100: "<<p.at(256)<<" != 0\n";return 2;}
if(p.at(260)!=0){std::cerr<<"tick 2, offset 0x104: "<<p.at(260)<<" != 0\n";return 2;}
shin::update_player(p,ctx);
// Original runtime state before tick 3.
if(p.at(8)!=0){std::cerr<<"tick 3, offset 0x8: "<<p.at(8)<<" != 0\n";return 2;}
if(p.at(12)!=11){std::cerr<<"tick 3, offset 0xc: "<<p.at(12)<<" != 11\n";return 2;}
if(p.at(16)!=76){std::cerr<<"tick 3, offset 0x10: "<<p.at(16)<<" != 76\n";return 2;}
if(p.at(20)!=72){std::cerr<<"tick 3, offset 0x14: "<<p.at(20)<<" != 72\n";return 2;}
if(p.at(24)!=4572968){std::cerr<<"tick 3, offset 0x18: "<<p.at(24)<<" != 4572968\n";return 2;}
if(p.at(28)!=1){std::cerr<<"tick 3, offset 0x1c: "<<p.at(28)<<" != 1\n";return 2;}
if(p.at(32)!=1){std::cerr<<"tick 3, offset 0x20: "<<p.at(32)<<" != 1\n";return 2;}
if(p.at(36)!=2){std::cerr<<"tick 3, offset 0x24: "<<p.at(36)<<" != 2\n";return 2;}
if(p.at(40)!=10){std::cerr<<"tick 3, offset 0x28: "<<p.at(40)<<" != 10\n";return 2;}
if(p.at(44)!=24){std::cerr<<"tick 3, offset 0x2c: "<<p.at(44)<<" != 24\n";return 2;}
if(p.at(48)!=14){std::cerr<<"tick 3, offset 0x30: "<<p.at(48)<<" != 14\n";return 2;}
if(p.at(52)!=50){std::cerr<<"tick 3, offset 0x34: "<<p.at(52)<<" != 50\n";return 2;}
if(p.at(56)!=71){std::cerr<<"tick 3, offset 0x38: "<<p.at(56)<<" != 71\n";return 2;}
if(p.at(68)!=0){std::cerr<<"tick 3, offset 0x44: "<<p.at(68)<<" != 0\n";return 2;}
if(p.at(72)!=5){std::cerr<<"tick 3, offset 0x48: "<<p.at(72)<<" != 5\n";return 2;}
if(p.at(76)!=0){std::cerr<<"tick 3, offset 0x4c: "<<p.at(76)<<" != 0\n";return 2;}
if(p.at(80)!=5){std::cerr<<"tick 3, offset 0x50: "<<p.at(80)<<" != 5\n";return 2;}
if(p.at(84)!=0){std::cerr<<"tick 3, offset 0x54: "<<p.at(84)<<" != 0\n";return 2;}
if(p.at(88)!=6){std::cerr<<"tick 3, offset 0x58: "<<p.at(88)<<" != 6\n";return 2;}
if(p.at(92)!=2){std::cerr<<"tick 3, offset 0x5c: "<<p.at(92)<<" != 2\n";return 2;}
if(p.at(96)!=0){std::cerr<<"tick 3, offset 0x60: "<<p.at(96)<<" != 0\n";return 2;}
if(p.at(100)!=0){std::cerr<<"tick 3, offset 0x64: "<<p.at(100)<<" != 0\n";return 2;}
if(p.at(104)!=0){std::cerr<<"tick 3, offset 0x68: "<<p.at(104)<<" != 0\n";return 2;}
if(p.at(112)!=0){std::cerr<<"tick 3, offset 0x70: "<<p.at(112)<<" != 0\n";return 2;}
if(p.at(116)!=0){std::cerr<<"tick 3, offset 0x74: "<<p.at(116)<<" != 0\n";return 2;}
if(p.at(136)!=1){std::cerr<<"tick 3, offset 0x88: "<<p.at(136)<<" != 1\n";return 2;}
if(p.at(140)!=5){std::cerr<<"tick 3, offset 0x8c: "<<p.at(140)<<" != 5\n";return 2;}
if(p.at(144)!=0){std::cerr<<"tick 3, offset 0x90: "<<p.at(144)<<" != 0\n";return 2;}
if(p.at(148)!=0){std::cerr<<"tick 3, offset 0x94: "<<p.at(148)<<" != 0\n";return 2;}
if(p.at(152)!=0){std::cerr<<"tick 3, offset 0x98: "<<p.at(152)<<" != 0\n";return 2;}
if(p.at(176)!=3){std::cerr<<"tick 3, offset 0xb0: "<<p.at(176)<<" != 3\n";return 2;}
if(p.at(180)!=0){std::cerr<<"tick 3, offset 0xb4: "<<p.at(180)<<" != 0\n";return 2;}
if(p.at(188)!=0){std::cerr<<"tick 3, offset 0xbc: "<<p.at(188)<<" != 0\n";return 2;}
if(p.at(192)!=0){std::cerr<<"tick 3, offset 0xc0: "<<p.at(192)<<" != 0\n";return 2;}
if(p.at(196)!=0){std::cerr<<"tick 3, offset 0xc4: "<<p.at(196)<<" != 0\n";return 2;}
if(p.at(200)!=0){std::cerr<<"tick 3, offset 0xc8: "<<p.at(200)<<" != 0\n";return 2;}
if(p.at(204)!=0){std::cerr<<"tick 3, offset 0xcc: "<<p.at(204)<<" != 0\n";return 2;}
if(p.at(208)!=0){std::cerr<<"tick 3, offset 0xd0: "<<p.at(208)<<" != 0\n";return 2;}
if(p.at(212)!=0){std::cerr<<"tick 3, offset 0xd4: "<<p.at(212)<<" != 0\n";return 2;}
if(p.at(216)!=0){std::cerr<<"tick 3, offset 0xd8: "<<p.at(216)<<" != 0\n";return 2;}
if(p.at(220)!=0){std::cerr<<"tick 3, offset 0xdc: "<<p.at(220)<<" != 0\n";return 2;}
if(p.at(224)!=0){std::cerr<<"tick 3, offset 0xe0: "<<p.at(224)<<" != 0\n";return 2;}
if(p.at(228)!=0){std::cerr<<"tick 3, offset 0xe4: "<<p.at(228)<<" != 0\n";return 2;}
if(p.at(232)!=0){std::cerr<<"tick 3, offset 0xe8: "<<p.at(232)<<" != 0\n";return 2;}
if(p.at(236)!=0){std::cerr<<"tick 3, offset 0xec: "<<p.at(236)<<" != 0\n";return 2;}
if(p.at(240)!=0){std::cerr<<"tick 3, offset 0xf0: "<<p.at(240)<<" != 0\n";return 2;}
if(p.at(244)!=0){std::cerr<<"tick 3, offset 0xf4: "<<p.at(244)<<" != 0\n";return 2;}
if(p.at(252)!=0){std::cerr<<"tick 3, offset 0xfc: "<<p.at(252)<<" != 0\n";return 2;}
if(p.at(256)!=0){std::cerr<<"tick 3, offset 0x100: "<<p.at(256)<<" != 0\n";return 2;}
if(p.at(260)!=0){std::cerr<<"tick 3, offset 0x104: "<<p.at(260)<<" != 0\n";return 2;}
shin::update_player(p,ctx);
// Original runtime state before tick 4.
if(p.at(8)!=0){std::cerr<<"tick 4, offset 0x8: "<<p.at(8)<<" != 0\n";return 2;}
if(p.at(12)!=18){std::cerr<<"tick 4, offset 0xc: "<<p.at(12)<<" != 18\n";return 2;}
if(p.at(16)!=76){std::cerr<<"tick 4, offset 0x10: "<<p.at(16)<<" != 76\n";return 2;}
if(p.at(20)!=72){std::cerr<<"tick 4, offset 0x14: "<<p.at(20)<<" != 72\n";return 2;}
if(p.at(24)!=4572968){std::cerr<<"tick 4, offset 0x18: "<<p.at(24)<<" != 4572968\n";return 2;}
if(p.at(28)!=1){std::cerr<<"tick 4, offset 0x1c: "<<p.at(28)<<" != 1\n";return 2;}
if(p.at(32)!=1){std::cerr<<"tick 4, offset 0x20: "<<p.at(32)<<" != 1\n";return 2;}
if(p.at(36)!=2){std::cerr<<"tick 4, offset 0x24: "<<p.at(36)<<" != 2\n";return 2;}
if(p.at(40)!=10){std::cerr<<"tick 4, offset 0x28: "<<p.at(40)<<" != 10\n";return 2;}
if(p.at(44)!=24){std::cerr<<"tick 4, offset 0x2c: "<<p.at(44)<<" != 24\n";return 2;}
if(p.at(48)!=14){std::cerr<<"tick 4, offset 0x30: "<<p.at(48)<<" != 14\n";return 2;}
if(p.at(52)!=50){std::cerr<<"tick 4, offset 0x34: "<<p.at(52)<<" != 50\n";return 2;}
if(p.at(56)!=71){std::cerr<<"tick 4, offset 0x38: "<<p.at(56)<<" != 71\n";return 2;}
if(p.at(68)!=0){std::cerr<<"tick 4, offset 0x44: "<<p.at(68)<<" != 0\n";return 2;}
if(p.at(72)!=11){std::cerr<<"tick 4, offset 0x48: "<<p.at(72)<<" != 11\n";return 2;}
if(p.at(76)!=0){std::cerr<<"tick 4, offset 0x4c: "<<p.at(76)<<" != 0\n";return 2;}
if(p.at(80)!=11){std::cerr<<"tick 4, offset 0x50: "<<p.at(80)<<" != 11\n";return 2;}
if(p.at(84)!=0){std::cerr<<"tick 4, offset 0x54: "<<p.at(84)<<" != 0\n";return 2;}
if(p.at(88)!=7){std::cerr<<"tick 4, offset 0x58: "<<p.at(88)<<" != 7\n";return 2;}
if(p.at(92)!=2){std::cerr<<"tick 4, offset 0x5c: "<<p.at(92)<<" != 2\n";return 2;}
if(p.at(96)!=0){std::cerr<<"tick 4, offset 0x60: "<<p.at(96)<<" != 0\n";return 2;}
if(p.at(100)!=0){std::cerr<<"tick 4, offset 0x64: "<<p.at(100)<<" != 0\n";return 2;}
if(p.at(104)!=0){std::cerr<<"tick 4, offset 0x68: "<<p.at(104)<<" != 0\n";return 2;}
if(p.at(112)!=0){std::cerr<<"tick 4, offset 0x70: "<<p.at(112)<<" != 0\n";return 2;}
if(p.at(116)!=0){std::cerr<<"tick 4, offset 0x74: "<<p.at(116)<<" != 0\n";return 2;}
if(p.at(136)!=1){std::cerr<<"tick 4, offset 0x88: "<<p.at(136)<<" != 1\n";return 2;}
if(p.at(140)!=5){std::cerr<<"tick 4, offset 0x8c: "<<p.at(140)<<" != 5\n";return 2;}
if(p.at(144)!=0){std::cerr<<"tick 4, offset 0x90: "<<p.at(144)<<" != 0\n";return 2;}
if(p.at(148)!=0){std::cerr<<"tick 4, offset 0x94: "<<p.at(148)<<" != 0\n";return 2;}
if(p.at(152)!=0){std::cerr<<"tick 4, offset 0x98: "<<p.at(152)<<" != 0\n";return 2;}
if(p.at(176)!=3){std::cerr<<"tick 4, offset 0xb0: "<<p.at(176)<<" != 3\n";return 2;}
if(p.at(180)!=0){std::cerr<<"tick 4, offset 0xb4: "<<p.at(180)<<" != 0\n";return 2;}
if(p.at(188)!=0){std::cerr<<"tick 4, offset 0xbc: "<<p.at(188)<<" != 0\n";return 2;}
if(p.at(192)!=0){std::cerr<<"tick 4, offset 0xc0: "<<p.at(192)<<" != 0\n";return 2;}
if(p.at(196)!=0){std::cerr<<"tick 4, offset 0xc4: "<<p.at(196)<<" != 0\n";return 2;}
if(p.at(200)!=0){std::cerr<<"tick 4, offset 0xc8: "<<p.at(200)<<" != 0\n";return 2;}
if(p.at(204)!=0){std::cerr<<"tick 4, offset 0xcc: "<<p.at(204)<<" != 0\n";return 2;}
if(p.at(208)!=0){std::cerr<<"tick 4, offset 0xd0: "<<p.at(208)<<" != 0\n";return 2;}
if(p.at(212)!=0){std::cerr<<"tick 4, offset 0xd4: "<<p.at(212)<<" != 0\n";return 2;}
if(p.at(216)!=0){std::cerr<<"tick 4, offset 0xd8: "<<p.at(216)<<" != 0\n";return 2;}
if(p.at(220)!=0){std::cerr<<"tick 4, offset 0xdc: "<<p.at(220)<<" != 0\n";return 2;}
if(p.at(224)!=0){std::cerr<<"tick 4, offset 0xe0: "<<p.at(224)<<" != 0\n";return 2;}
if(p.at(228)!=0){std::cerr<<"tick 4, offset 0xe4: "<<p.at(228)<<" != 0\n";return 2;}
if(p.at(232)!=0){std::cerr<<"tick 4, offset 0xe8: "<<p.at(232)<<" != 0\n";return 2;}
if(p.at(236)!=0){std::cerr<<"tick 4, offset 0xec: "<<p.at(236)<<" != 0\n";return 2;}
if(p.at(240)!=0){std::cerr<<"tick 4, offset 0xf0: "<<p.at(240)<<" != 0\n";return 2;}
if(p.at(244)!=0){std::cerr<<"tick 4, offset 0xf4: "<<p.at(244)<<" != 0\n";return 2;}
if(p.at(252)!=0){std::cerr<<"tick 4, offset 0xfc: "<<p.at(252)<<" != 0\n";return 2;}
if(p.at(256)!=0){std::cerr<<"tick 4, offset 0x100: "<<p.at(256)<<" != 0\n";return 2;}
if(p.at(260)!=0){std::cerr<<"tick 4, offset 0x104: "<<p.at(260)<<" != 0\n";return 2;}
std::cout<<"4 original-runtime snapshots, 50 scalar fields each, match reconstructed player/real map probes\n";
}
