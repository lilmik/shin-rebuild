#include "../src/game/level1_gate_sequence.hpp"
#include <iostream>
int main(int argc,char**argv){if(argc!=2)return 1;shin::AssetStore assets(argv[1]);for(auto name:shin::gate_frames)assets.image(name);for(auto name:shin::gate_recall_m)assets.image(name);for(auto name:shin::gate_recall_s)assets.image(name);shin::PlayerPayload p;shin::IndexedSurface frame(640,480);shin::Level1GateSequence seq;seq.start(p,0);int sounds=0;auto sound=[&](const std::string&){++sounds;};seq.tick(p,150,false,0,0,assets,frame,sound);if(p.at(0x1c)!=0)return 2;seq.tick(p,151,false,0,0,assets,frame,sound);if(p.at(0x1c)!=1)return 3;seq.tick(p,152,true,0,0,assets,frame,sound);seq.tick(p,153,false,0,0,assets,frame,sound);if(seq.active()||p.at(0x1c)!=0)return 4;
 seq.start(p,0);for(unsigned now=16;now<60000&&seq.active();now+=16)seq.tick(p,now,false,0,0,assets,frame,sound);
 if(seq.active()||!seq.gate_removed||sounds!=6)return 5;
 std::cout<<"complete gate timeline, six sounds, terminal recall frame, and Enter release verified\n";}
