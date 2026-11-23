#include "../src/game/stage2_exit.hpp"
#include "../src/assets/asset_store.hpp"
#include <cassert>
#include <filesystem>
int main(){shin::AssetStore a("D:\\ws_vs\\lbxx\\lbxx_asset\\RESOURCE.RES");for(auto n:shin::stage2_exit_assets)a.image(n);for(auto n:{"73.wav","74.wav","75.wav"})assert(std::filesystem::exists(std::filesystem::path("D:\\ws_vs\\lbxx\\lbxx_asset\\resource")/n));shin::Stage2ExitCutscene c;unsigned sounds=0;bool done=false;for(unsigned now=0;now<20000&&!done;now+=16){auto f=c.update(now,false);sounds+=unsigned(f.sounds.size());done=f.action==shin::CutsceneFrame::resume_level;}assert(done&&sounds==3);assert(c.update(30000,false).draws.empty());shin::Stage2ExitCutscene s;assert(s.update(0,true).action==shin::CutsceneFrame::resume_level);shin::Stage2ExitCutscene t;assert(t.update(0,false).sounds.empty());assert(t.update(300,false).sounds[0]==0x77fc44);assert(t.phase()==0);t.update(301,false);assert(t.phase()==1);}
