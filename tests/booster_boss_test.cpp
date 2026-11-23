#include "../src/game/booster_boss.hpp"
#include <cassert>
#include <iostream>
#include "../src/game/booster_ending.hpp"
#include "../src/game/booster_missiles.hpp"
#include "../src/game/stage_scene.hpp"
int main(int argc,char**argv){try{assert(argc==2);shin::AssetStore a(argv[1]);shin::BoosterBoss b(400,0,a);shin::IndexedSurface f(640,480);assert(b.payload.at(0x74)==30);for(int hp=30;hp>0;--hp){assert(b.hurt());assert(!b.hurt());for(int i=0;i<10;++i)b.tick(0,0,0,100,a,f);assert(b.payload.at(0x74)==hp-1);if(hp>1)for(int i=0;i<45;++i)b.tick(0,0,0,100,a,f);}assert(b.ending_requested&&b.payload.state==2);shin::BoosterEnding end;int voices=0;bool done=false;for(unsigned t=0;t<60000&&!done;t+=16){auto out=end.update(t,false);for(auto&d:out.draws)a.image(d.resource);voices+=int(out.sounds.size());done=out.action==shin::CutsceneFrame::resume_level;}assert(done&&voices==1);shin::ResourceArchive archive(argv[1]);shin::game::StageScene scene;scene.load(archive,3);shin::game::CollisionWorld world(scene.map,scene.collision_blocks);shin::BoosterMissiles m;m.spawn(100,-100,8,20);m.tick(0,0,0,a,f,world);assert(m.shots.size()==1&&m.shots[0].delay==19&&m.shots[0].e.at(0x50)==0);for(int i=0;i<22;++i)m.tick(0,0,0,a,f,world);}catch(const std::exception&e){std::cerr<<e.what()<<"\n";return 2;}}
