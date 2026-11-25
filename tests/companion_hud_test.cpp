#include "../src/game/companion_hud.hpp"
#include <iostream>
int main(int argc,char**argv){if(argc!=2)return 1;shin::AssetStore a(argv[1]);shin::PlayerPayload p;shin::initialize_player_payload(p,0,0);shin::IndexedSurface f(640,480);shin::CompanionHud hud;int hits=0;
 std::vector<shin::CompanionHud::Target> targets={{0,0,{0,0,74,71},[&]{++hits;}}};
 hud.activate();for(int i=0;i<210;++i)hud.tick(p,0,0,a,f,targets);
 if(hud.active()||hits==0)return 2;
 shin::IndexedSurface wide(853,480);wide.origin_x=106;hud.tick(p,0,0,a,wide,{});if(hud.screen_x()!=-96||hud.screen_y()!=400)return 3;shin::IndexedSurface tall(640,853);tall.origin_y=186;hud.tick(p,0,0,a,tall,{});if(hud.screen_x()!=10||hud.screen_y()!=587)return 4;
 int visible_hits=0,offscreen_hits=0;shin::CompanionHud attack;
 std::vector<shin::CompanionHud::Target> choices={{-5000,0,{0,0,74,71},[&]{++offscreen_hits;}},{100,100,{0,0,74,71},[&]{++visible_hits;}}};
 attack.activate();for(int i=0;i<210;++i)attack.tick(p,0,0,a,f,choices);
 if(attack.active()||visible_hits==0||offscreen_hits!=0)return 5;
 int first_hits=0,near_hits=0;shin::CompanionHud ordered;
 std::vector<shin::CompanionHud::Target> ordered_targets={{350,100,{0,0,74,71},[&]{++first_hits;}},{100,100,{0,0,74,71},[&]{++near_hits;}}};
 ordered.activate();for(int i=0;i<210;++i)ordered.tick(p,0,0,a,f,ordered_targets);
 if(!first_hits||near_hits)return 6;
 std::cout<<"Shiro activation, target contact, return to HUD, life assets verified\n";
}
