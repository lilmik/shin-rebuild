#pragma once
#include "booster_boss.hpp"
namespace shin {
// Fifth-selection pair: map27/28 ->404d70/444540; shared counter45c0a8.
class JungleBoss {
 std::vector<const char*>names;std::vector<int>frames;int state=1,delay=10,frame=0,ticks=0,elapsed=0;bool second=false;
 void pose(unsigned h){switch(h){
#include "jungle_boss_poses.inc"
 default:return;}frame=ticks=0;payload.at(0x50)=state;if(h==0x444a90)payload.at(0x4c)=-20;}
 void walk(int px){pose(second?(px<payload.at(4)?0x444a30:0x444a60):(px<payload.at(4)?0x4052d0:0x405300));}
public:
 struct Shot{int x,y,vx;};std::vector<Shot>projectiles;std::vector<std::string>sounds;game::EntityPayload payload;bool defeated=false,cinematic=true;
 JungleBoss(int type,int x,int y,AssetStore&a){second=type==0x28;payload.kind=second?0x76:0x75;switch(type){
#include "jungle_boss_names.inc"
 }auto&im=a.image(names.back());payload.at(4)=payload.at(0x40)=x;payload.at(8)=payload.at(0x44)=y;payload.at(0x64)=im.width;payload.at(0x68)=im.height;payload.at(0x6c)=10;auto r=rectangle();for(int i=0;i<4;++i)payload.at(0x20+4*i)=r[i];pose(second?0x444a30:0x4052d0);}
 game::CollisionRect rectangle()const{return second?game::CollisionRect{36,9,88,191}:game::CollisionRect{33,28,79,201};}
 int hp(){return payload.at(0x6c);}bool dying()const{return state==4;}
 void begin_combat(){cinematic=false;}
 bool hurt(){if(defeated||cinematic||dying())return false;pose(second?0x444ad0:0x405360);sounds.push_back("RESOURCE/punch2.wav");return true;}
 void terrain(const game::CollisionWorld&w,int material){auto&e=payload;if(material!=1&&material!=2)return;auto body=[&](int x,int y){return w.body(rectangle(),x,y);};for(int n=0;n<26;++n)if(!body(e.at(4),e.at(8)-n)){e.at(8)-=n;if(material==2)e.at(0x4c)+=2;return;}if(material==1){int x=e.at(4),old=e.at(0x40),step=x>old?-1:1;if(x!=old)for(int v=x;(step<0&&v>=old)||(step>0&&v<=old);v+=step)if(!body(v,e.at(0x44))){e.at(4)=v;break;}}int y=e.at(8),old=e.at(0x44);if(y==old||(material==2&&y<old))return;int step=y>old?-1:1;for(int v=y;(step<0&&v>=old)||(step>0&&v<=old);v+=step)if(!body(e.at(4),v)){e.at(8)=v;return;}}
 void tick(int cx,int cy,int px,int pw,AssetStore&a,IndexedSurface&f){auto&e=payload;if(defeated)return;e.at(0x40)=e.at(4);e.at(0x44)=e.at(8);
 if(!cinematic){if(state<=2){if(elapsed>200){pose(second?0x444a90:0x405330);elapsed=0;}if(state<=2){e.at(0x48)=(state==1?-1:1)*(second?4:2);e.at(0x60)=state==2;}}
 else if(state==3){if(second){e.at(0x48)=e.at(0x60)?8:-8;if(frame==1&&ticks==9)walk(px);}else{if(frame==0&&ticks==1)e.at(0x60)=e.at(4)+e.at(0x64)/2<=px+pw/2;if(frame==2&&ticks==9){projectiles.push_back({e.at(4)+(e.at(0x60)?e.at(0x64)-10:0),e.at(8)+30,e.at(0x60)?8:-8});walk(px);}}}
 else if(state==4){if(frame==0&&ticks==0)e.at(0x4c)=-10;if(frame==1&&ticks==0){if(--e.at(0x6c)<1){e.state=2;defeated=true;return;}walk(px);}}}
 f.blit(a.image(names.at(frames.at(frame))),e.at(4)-cx,e.at(8)-cy,true,0,0,-1,-1,e.at(0x60)!=0);f.blit(a.image(second?"bsfc52.raw":"bsfc51.raw"),10,second?90:50);for(int n=0;n<hp();++n)f.blit(a.image("rlife.raw"),50+20*n,second?90:50);
 if(e.at(0x48)>0)--e.at(0x48);if(e.at(0x48)<0)++e.at(0x48);if(e.at(0x4c)<11)++e.at(0x4c);e.at(4)+=e.at(0x48);e.at(8)+=e.at(0x4c);if(++ticks>delay){ticks=0;frame=(frame+1)%frames.size();}++elapsed;
 }
};
}
