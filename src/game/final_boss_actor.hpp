#pragma once
#include "booster_boss.hpp"
namespace shin {
// Final pair spawned by4341d0: MAP3b/29,443020/42a5e0. Each starts10HP.
class FinalBossActor{
 std::vector<const char*>names;std::vector<int>frames;int state=1,delay=10,frame=0,ticks=0,elapsed=0;bool robot=false;
 void pose(unsigned h){switch(h){
#include "final_boss_poses.inc"
 default:return;}frame=ticks=0;payload.at(robot?0x58:0x50)=state;}
 void walk(int px){pose(robot?(px<payload.at(4)?0x4435b0:0x4435e0):(px<payload.at(4)?0x42ac70:0x42aca0));}
public:
 struct Shot{unsigned constructor;int kind,x,y,vx;};std::vector<Shot>projectiles;std::vector<std::string>sounds;game::EntityPayload payload;bool defeated=false;
 FinalBossActor(int type,int x,int y,AssetStore&a){robot=type==0x3b;payload.kind=robot?0x7c:0x77;switch(payload.kind){
#include "final_boss_names.inc"
 }auto&im=a.image(names.back());payload.at(4)=payload.at(robot?0x48:0x40)=x;payload.at(8)=payload.at(robot?0x4c:0x44)=y;payload.at(robot?0x50:0x48)=0;payload.at(robot?0x54:0x4c)=0;payload.at(robot?0x74:0x6c)=10;auto rect=rectangle();for(int i=0;i<4;++i)payload.at((robot?0x28:0x20)+4*i)=rect[i];payload.at(robot?0xc:0x64)=im.width;payload.at(robot?0x10:0x68)=im.height;pose(robot?0x4435b0:0x42ac70);}
 game::CollisionRect rectangle()const{return robot?game::CollisionRect{83,28,157,239}:game::CollisionRect{105,26,178,148};}
 void resume_combat(int px){payload.at(robot?0x50:0x48)=0;walk(px);}
 int hp(){return payload.at(robot?0x74:0x6c);}
 bool dying()const{return state==(robot?3:4);}
 bool hurt(){if(defeated||dying())return false;pose(robot?0x443610:0x42ad00);sounds.push_back("RESOURCE/punch2.wav");return true;}
 void terrain(const game::CollisionWorld&w,int material){auto&e=payload;if(material!=1&&material!=2)return;auto body=[&](int x,int y){return w.body(rectangle(),x,y);};for(int n=0;n<26;++n)if(!body(e.at(4),e.at(8)-n)){e.at(8)-=n;if(material==2)e.at(robot?0x54:0x4c)+=2;return;}if(material==1){int x=e.at(4),old=e.at(robot?0x48:0x40),step=x>old?-1:1;if(x!=old)for(int v=x;(step<0&&v>=old)||(step>0&&v<=old);v+=step)if(!body(v,e.at(robot?0x4c:0x44))){e.at(4)=v;break;}}int y=e.at(8),old=e.at(robot?0x4c:0x44);if(y==old||(material==2&&y<old))return;int step=y>old?-1:1;for(int v=y;(step<0&&v>=old)||(step>0&&v<=old);v+=step)if(!body(e.at(4),v)){e.at(8)=v;return;}}
 void tick(int cx,int cy,int px,int py,int pw,AssetStore&a,IndexedSurface&f){auto&e=payload;if(defeated)return;int vxoff=robot?0x50:0x48,vyoff=robot?0x54:0x4c;int width=e.at(robot?0xc:0x64);e.at(robot?0x48:0x40)=e.at(4);e.at(robot?0x4c:0x44)=e.at(8);
 if(state<=2){if(elapsed>200){pose(robot?(py<e.at(8)+e.at(0x10)/2?0x443640:0x443670):(py<e.at(8)+e.at(0x68)/2?0x42acd0:0x42ad70));elapsed=0;}if(state==1||state==2){e.at(vxoff)=state==1?-2:2;e.at(0x60)=state==2;}}
 else if(dying()){if(frame==0&&ticks==0)e.at(vyoff)=-10;if(frame==1&&ticks==0){if(--e.at(robot?0x74:0x6c)<1){e.state=2;defeated=true;return;}walk(px);}}
 else if(!robot){if((state==3&&frame==0&&ticks==0)||(state==5&&frame==0&&ticks==1))e.at(0x60)=e.at(4)+width/2<=px+pw/2;if(state==3&&frame==1&&ticks==9)projectiles.push_back({0x448110,0x63,e.at(4)+(e.at(0x60)?width-10:0),e.at(8)+30,e.at(0x60)?8:-8});if(state==5&&frame==2&&ticks==9)projectiles.push_back({0x446480,0x43,e.at(4)+(e.at(0x60)?width-10:0),e.at(8)+50,0});if(frame==3&&ticks==9)walk(px);}
 else if((state==4||state==5)&&frame==1&&ticks==9)walk(px);
 f.blit(a.image(names.at(frames.at(frame))),e.at(4)-cx,e.at(8)-cy,true,0,0,-1,-1,e.at(0x60)!=0);f.blit(a.image(robot?"bsfc71.raw":"bsfc72.raw"),10,robot?50:90);for(int n=0;n<hp();++n)f.blit(a.image("rlife.raw"),50+20*n,robot?50:90);
 if(e.at(vxoff)>0)--e.at(vxoff);if(e.at(vxoff)<0)++e.at(vxoff);if(e.at(vyoff)<11)++e.at(vyoff);e.at(4)+=e.at(vxoff);e.at(8)+=e.at(vyoff);if(++ticks>delay){ticks=0;frame=(frame+1)%frames.size();}++elapsed;
 }
};
}


