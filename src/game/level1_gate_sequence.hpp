#pragma once
#include "level1_gate_data.hpp"
#include "player_controller.hpp"
#include "../assets/asset_store.hpp"
#include <functional>
namespace shin {
// Recovered 40e660/40e270. Host suppresses normal player update while active.
class Level1GateSequence {
 bool active_=false,enter_=false;unsigned started_=0;
public:
 bool gate_removed=false,completed=false;
 bool active()const{return active_;}
 void start(PlayerPayload&p,unsigned now){active_=true;enter_=false;gate_removed=completed=false;started_=now;for(auto o:{0x1c,0x20,0x9c,0xa0,0xa4,0xa8,0xac})p.at(o)=0;p.at(0x108)=int(now);}
 // Mandatory sound callback receives actual PCM source filenames.
 void tick(PlayerPayload&p,unsigned now,bool enter,int cx,int cy,AssetStore&assets,IndexedSurface&target,const std::function<void(const std::string&)>&sound){
  if(!active_)return;int frame=p.at(0x1c);if(frame<0||frame>=51)throw std::runtime_error("gate frame out of range");
  p.at(0x44)=p.at(8)-cx;p.at(0x48)=p.at(12)-cy;target.blit_player(assets.image(gate_frames.at(gate_indices.at(frame))),p.at(0x44),p.at(0x48),true);
  if(frame==47){target.blit_player(assets.image("recall.app"),p.at(0x44),p.at(0x48)-180,true);auto m=gate_m_sequence.at(p.at(0x9c)),s=gate_s_sequence.at(p.at(0xa4));target.blit_player(assets.image(gate_recall_m.at(m)),p.at(0x44)+78,p.at(0x48)-182,true);target.blit_player(assets.image(gate_recall_s.at(s)),p.at(0x44)+16,p.at(0x48)-88,true);}
  if(p.at(0x54)>0)--p.at(0x54);if(p.at(0x54)<0)++p.at(0x54);if(p.at(0x58)<11)++p.at(0x58);p.at(8)+=p.at(0x54);p.at(12)+=p.at(0x58);
  if(enter)enter_=true;else if(enter_){enter_=false;active_=false;gate_removed=completed=true;p.at(0x1c)=p.at(0x20)=0;return;}
  if(p.at(0x20)==0){static const int phases[]={37,41,44,46,48,49};for(int i=0;i<6;++i)if(frame==phases[i]){if(!sound)throw std::runtime_error("unbound gate sound");sound("RESOURCE/"+std::to_string(21+i)+".wav");}if(frame==50)gate_removed=true;}
  ++p.at(0x20);p.at(0x10c)=int(now);
  if(gate_durations.at(frame)<unsigned(now-unsigned(p.at(0x108)))){p.at(0x108)=int(now);++p.at(0x1c);p.at(0x20)=0;if(p.at(0x1c)==51){p.at(0x1c)=0;active_=false;gate_removed=completed=true;return;}}
  // The legacy callback increments beyond the six-byte recall tables during
  // the 5000ms hold. Keep the terminal frame to avoid reading adjacent strings
  // as sprite indices in the native reconstruction.
  if(p.at(0x1c)==47){for(auto pair:{std::pair<int,int>{0xa8,0xa4},{0xa0,0x9c}}){if(++p.at(pair.first)==20){p.at(pair.first)=0;if(p.at(pair.second)<5)++p.at(pair.second);if(p.at(pair.second)==5)p.at(0xac)=1;}}}
 }
};
}
