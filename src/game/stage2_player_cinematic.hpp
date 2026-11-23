#pragma once
#include "player_animation.hpp"
#include "player_world_response.hpp"
namespace shin {
class Stage2PlayerCinematic {
 bool active_=false,ending_=false;
public:
 bool freeze_groups_requested=false;std::vector<unsigned>spawn_requests;std::vector<std::string>sounds;
 bool active()const{return active_;}
static inline const char*names0[]={"2#411.raw","2#412.raw","2#323.raw"};
static inline const int indices0[]={0,1,2};
static inline const char*names1[]={"1#247.raw","1#248.raw","2#621.raw","2#622.raw","2#623.raw","2#631.raw","2#632.raw","2#633.raw","2#634.raw","2#635.raw","2#636.raw","2#641.raw","1#115.raw","1#116.raw","1#117.raw"};
static inline const int indices1[]={0,1,2,3,4,3,5,6,7,8,9,10,11,0,12,13,14,13};
void pose(PlayerPayload&p,int state,int table,int count,int delay=10){p.at(0x5c)=state;p.at(0x18)=table;p.at(0x24)=count;p.at(0x28)=delay;p.at(0x1c)=p.at(0x20)=0;}
 bool helpercall(unsigned helper,PlayerPayload&p){switch(helper){
 case 0x40f750:active_=true;ending_=false;pose(p,0x78,0x45d820,1);return true;
 case 0x40f780:pose(p,0x2a,0x45d818,2);return true;
 case 0x40fc20:active_=true;ending_=true;pose(p,0x2d,0x45da08,2);return true;
 case 0x40fc80:pose(p,0x2e,0x45da10,4);return true;
 case 0x40fcb0:pose(p,0x2f,0x45da20,6);return true;
 case 0x40fce0:pose(p,0x30,0x45da38,1);return true;
 case 0x40fd10:pose(p,0x31,0x45da40,4);return true;
 case 0x40fd40:freeze_groups_requested=true;spawn_requests.push_back(0x43e610);[[fallthrough]];
 case 0x40f7e0:active_=false;pose(p,0,0x45c714,1,100);return true;
 default:return false;}}
 void tick(PlayerPayload&p,const game::CollisionWorld&world,int cx,int cy,AssetStore&assets,IndexedSurface&frame,bool terrain=true){if(!active_)return;
 int state=p.at(0x5c);if((ending_&&state>=0x2d&&state<=0x31)||(!ending_&&(state==0x2a||state==0x78)))p.at(0x54)=0;
 p.at(0x44)=p.at(8)-cx;p.at(0x48)=p.at(12)-cy;int table=p.at(0x18),f=p.at(0x1c);
 if(ending_){unsigned i=unsigned((table-0x45da08)/4+f);if(i>=sizeof(indices1)/sizeof(int))throw std::runtime_error("ending animation table");frame.blit_player(assets.image(names1[indices1[i]]),p.at(0x44),p.at(0x48)-9);}
 else{unsigned i=unsigned((table-0x45d818)/4+f);if(i>=sizeof(indices0)/sizeof(int))throw std::runtime_error("intro animation table");frame.blit_player(assets.image(names0[indices0[i]]),p.at(0x44),p.at(0x48)-9);}
 if(p.at(0x54)>0)--p.at(0x54);if(p.at(0x54)<0)++p.at(0x54);if(p.at(0x58)<11)++p.at(0x58);p.at(8)+=p.at(0x54);p.at(12)+=p.at(0x58);
 if(++p.at(0x20)>p.at(0x28)){p.at(0x20)=0;if(++p.at(0x1c)>=p.at(0x24))p.at(0x1c)=0;}
 if(terrain){int material=world.body({p.at(0x2c),p.at(0x30),p.at(0x34),p.at(0x38)},p.at(8),p.at(12));if(material){auto r=game::respond_player_world(p,world,material);if(r.safety_limit_reached)throw std::runtime_error("cinematic terrain budget");if(r.hurt_sound_requested)sounds.push_back("RESOURCE/dem-shin.wav");}}
 }
};
}
