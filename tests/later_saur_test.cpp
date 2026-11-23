#include "../src/game/later_saur.hpp"
#include "../src/game/player_animation.hpp"
#include <cassert>
int main(int argc,char**argv){assert(argc==2);shin::AssetStore a(argv[1]);shin::IndexedSurface f(640,480);shin::PlayerPayload p;shin::initialize_player_payload(p,4,4);shin::game::LaterSaur e(400,100,a);for(int i=0;i<145;++i)e.tick(0,0,p,a,f);assert(!e.sound_requests.empty());e.hurt();for(int i=0;i<12;++i)e.tick(0,0,p,a,f);assert(e.defeated);}
