#pragma once
#include "player_controller.hpp"
#include "../assets/asset_store.hpp"
#include <array>
#include <cstring>
#include <string_view>
namespace shin {
// Exact 41-name table45c3e0, load order4059c0; indices45c714..45c7bc.
inline constexpr std::array<const char*,41> player_resources={
"stop0.app","stop1.app","stop2.app","walk0.app","walk1.app","walk2.app","walk3.app","jump0.app","jump1.app","jump2.app","damage0.app","damage1.app","chi0.app","cw0.app","cw1.app","cw2.app","cw3.app","chi_f1.app","chi_f2.app","chi_f3.app","bore0.app","bore1.app","roa0.app","roa1.app","roa-w0.app","roa-w1.app","roa-w2.app","roa-w3.app","roa_f1.app","roa_f2.app","roa_f3.app","sss0.app","sss1.app","sss2.app","boost1.app","boost2.app","boost3.app","boost4.app","boost5.app","huk1.app","huk2.app"};
inline constexpr std::array<int,50> player_animation_indices={0,3,4,5,6,7,8,9,0,1,2,10,11,12,0,13,14,15,16,17,18,19,0,20,21,22,0,24,25,26,27,28,29,30,23,39,40,24,25,26,27,31,32,33,0,34,35,36,37,38};
inline int player_image_index(std::uint32_t table,int frame){
 if(table<0x45c714||table>0x45c7d8||(table-0x45c714)%4||frame<0)throw std::out_of_range("player animation table");
 auto offset=(table-0x45c714)/4+unsigned(frame);
 if(offset>=player_animation_indices.size())throw std::out_of_range("player frame");
 int i=player_animation_indices[offset];if(i<0||i>=41)throw std::out_of_range("player asset index");return i;
}
inline void render_player(PlayerPayload& p,AssetStore& assets,IndexedSurface& target){
 int i=player_image_index(std::uint32_t(p.at(0x18)),p.at(0x1c));
 // 405cf0: facing==1 uses4334a0 original, all other values use433540 mirror.
 target.blit_player(assets.image(player_resources[i]),p.at(0x44),p.at(0x48),true,0,0,-1,-1,p.at(0x88)!=1);
}
// Scalar writes from405b10 and40d730. Allocation/owner/registration handles
// (00,04,3c,40) remain untouched for host binding; no invented pointer values.
inline void initialize_player_payload(PlayerPayload& p,std::int32_t tilex,std::int32_t tiley){
 p.immunity_remaining_ms=0;p.immunity_clock_valid=false;
 auto bits=[](std::uint32_t v){std::int32_t n;std::memcpy(&n,&v,4);return n;};
 p.at(8)=bits(std::uint32_t(tilex)<<5);p.at(12)=bits(std::uint32_t(tiley)<<5);
 p.at(28)=0;
 p.at(32)=0;
 p.at(40)=10;
 p.at(48)=0xe;
 p.at(52)=0x32;
 p.at(56)=0x47;
 p.at(68)=0;
 p.at(72)=0;
 p.at(84)=0;
 p.at(88)=0;
 p.at(104)=0;
 p.at(100)=0;
 p.at(96)=0;
 p.at(112)=0;
 p.at(116)=0;
 p.at(136)=1;
 p.at(140)=5;
 p.at(144)=0;
 p.at(148)=0;
 p.at(152)=0;
 p.at(176)=3;
 p.at(180)=0;
 p.at(188)=0;
 p.at(192)=0;
 p.at(196)=0;
 p.at(200)=0;
 p.at(204)=0;
 p.at(208)=0;
 p.at(212)=0;
 p.at(216)=0;
 p.at(220)=0;
 p.at(224)=0;
 p.at(228)=0;
 p.at(232)=0;
 p.at(236)=0;
 p.at(240)=0;
 p.at(244)=0;
 p.at(252)=0;
 p.at(256)=0;
 p.at(260)=0;
 p.at(0x2c)=24;
 p.at(0x4c)=p.at(8);p.at(0x50)=p.at(12);
 p.at(0x10)=76;p.at(0x14)=72; // dimensions left by final loaded huk2.app
 p.at(0x5c)=0;p.at(0x1c)=0;p.at(0x18)=0x45c714;p.at(0x24)=1;p.at(0x28)=100;p.at(0x20)=0;
}
}
