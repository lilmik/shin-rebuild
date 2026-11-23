#include "../src/game/companion_hud.hpp"
#include <iostream>
int main(int argc,char**argv){if(argc!=2)return 1;shin::AssetStore a(argv[1]);shin::PlayerPayload p;shin::initialize_player_payload(p,0,0);shin::IndexedSurface f(640,480);shin::CompanionHud hud;int hits=0;
 std::vector<shin::CompanionHud::Target> targets={{0,0,{0,0,74,71},[&]{++hits;}}};
 hud.activate();for(int i=0;i<210;++i)hud.tick(p,0,0,a,f,targets);
 if(hud.active()||hits==0)return 2;
 shin::IndexedSurface wide(853,480);wide.origin_x=106;hud.tick(p,0,0,a,wide,{});if(hud.screen_x()!=-96||hud.screen_y()!=400)return 3;shin::IndexedSurface tall(640,853);tall.origin_y=186;hud.tick(p,0,0,a,tall,{});if(hud.screen_x()!=10||hud.screen_y()!=587)return 4;
 std::cout<<"Shiro activation, target contact, return to HUD, life assets verified\n";
}
