#include "../src/game/level1_minigames.hpp"
#include <cassert>
#include <filesystem>
int main(int argc,char**argv){if(argc!=2)return 1;shin::AssetStore a(argv[1]);shin::IndexedSurface f(640,480);shin::Level1Minigames g;assert(g.start(0x7b));bool crash=false;for(unsigned now=1;now<110000&&g.active();now+=20){auto out=g.tick(now,{},a,f);for(auto&s:out.sounds){assert(std::filesystem::exists(std::filesystem::path("D:/ws_vs/lbxx/lbxx_asset")/s));if(s=="RESOURCE/crash.wav")crash=true;}if(out.finished)assert(std::string(out.palette)=="shin.pal");}assert(!g.active()&&crash);assert(g.start(0x7c));for(unsigned now=1;now<300000&&g.active();now+=20){auto out=g.tick(now,{},a,f);for(auto&s:out.sounds)assert(std::filesystem::exists(std::filesystem::path("D:/ws_vs/lbxx/lbxx_asset")/s));if(out.finished)assert(std::string(out.palette)=="shin.pal");}assert(!g.active());
assert(g.start(0x7c));for(unsigned now=1;now<300000&&g.active();now+=20){shin::Level1Minigames::Input in;if(g.flag_response_pending()){switch(g.flag_expected()){case 0:in.right=true;break;case 1:in.down=true;break;case 3:in.up=true;break;case 4:in.left=true;break;}}g.tick(now,in,a,f);}assert(!g.active()&&g.flag_stage()==3&&g.flag_lives()==5);

}
