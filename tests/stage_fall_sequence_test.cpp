#include "../src/game/stage_fall_sequence.hpp"
int main(int argc,char**argv){if(argc!=2)return 1;shin::AssetStore assets(argv[1]);auto map=assets.map("stage2.map");auto blocks=shin::decode_app(assets.bytes("block2.app"));shin::game::CollisionWorld world(map,blocks);shin::IndexedSurface f(640,480);shin::PlayerPayload p;p.at(8)=24;p.at(12)=100;p.at(0x64)=1;shin::StageFallSequence s;s.start(p);for(int i=0;i<200&&s.active();++i)s.tick(p,world,0,0,assets,f);return s.active()||p.at(0x5c)!=0||p.at(0x100)!=0?2:0;}

