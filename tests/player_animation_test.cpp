#include "../src/game/player_animation.hpp"
#include <stdexcept>
#include <iostream>
void check(bool b){if(!b)throw std::runtime_error("player animation failed");}
int main(int argc,char**argv){
 shin::PlayerPayload p;p.words.fill(777);shin::initialize_player_payload(p,3,4);
 check(p.at(8)==96&&p.at(12)==128&&p.at(0x4c)==96&&p.at(0x50)==128);
 check(p.at(0x2c)==24&&p.at(0x30)==14&&p.at(0x34)==50&&p.at(0x38)==71);
 check(p.at(0x18)==0x45c714&&p.at(0x28)==100&&p.at(0x88)==1&&p.at(0x8c)==5&&p.at(0xb0)==3);
 check(p.at(0)==777&&p.at(4)==777&&p.at(0x3c)==777&&p.at(0x40)==777&&p.at(0x78)==777);
 check(p.at(0x10)==76&&p.at(0x14)==72&&p.at(0x54)==0&&p.at(0x100)==0);
 check(shin::player_image_index(0x45c718,3)==6&&shin::player_image_index(0x45c7a8,3)==27&&shin::player_image_index(0x45c7a0,1)==40);
 if(argc<2)throw std::runtime_error("resource path required");shin::AssetStore assets(argv[1]);for(auto n:shin::player_resources)assets.image(n);
 p.at(0x44)=0;p.at(0x48)=0;p.at(0x18)=0x45c714;p.at(0x1c)=0;
 auto& original=assets.image("stop0.app");shin::IndexedSurface right(original.width,original.height,0xfe),left(original.width,original.height,0xfe);
 p.at(0x88)=1;shin::render_player(p,assets,right);p.at(0x88)=0;shin::render_player(p,assets,left);
 check(right.pixels==original.pixels);
 for(int y=0;y<original.height;++y)for(int x=0;x<original.width;++x)check(left.pixels[y*original.width+x]==original.pixels[y*original.width+original.width-1-x]);
 std::cout<<"player41 assets/animation indices/explicit initialization/pixel mirrored rendering passed\n";
}

