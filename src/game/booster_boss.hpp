#pragma once
#include "player_animation.hpp"
#include "collision_world.hpp"
#include "player_object_response.hpp"
namespace shin {
// MAP7a dispatched by427610 (not426380):414f80/415090/415ab0.
class BoosterBoss {
#include "booster_boss_data.inc"
 int volley_cooldown_=0;
 void pose(int state){auto&e=payload;e.at(0x50)=state;e.at(0x10)=e.at(0x14)=0;e.at(0x18)=state==5?1:4;e.at(0x1c)=state==5?20:10;}
 void volley(int px){if(volley_cooldown_>0)return;volley_cooldown_=90;auto&e=payload;bool right=px>=e.at(4)+e.at(0x6c)/2;e.at(0x68)=right;for(int delay:{100,80,60,40,20})projectiles.push_back({e.at(4)+(right?e.at(0x6c)-58:58),e.at(8)+86,right?8:-8,delay});sounds.push_back("RESOURCE/missile.wav");}
public:
 struct Missile{int x,y,vx,delay;};std::vector<Missile>projectiles;std::vector<std::string>sounds;game::EntityPayload payload;bool ending_requested=false;
 BoosterBoss(int x,int y,AssetStore&a){auto&e=payload;e.kind=0x32;e.at(4)=e.at(0x40)=x;e.at(8)=e.at(0x44)=y;e.at(0x20)=23;e.at(0x24)=52;e.at(0x28)=204;e.at(0x2c)=342;const auto&im=a.image(names[12]);e.at(0x6c)=im.width;e.at(0x70)=im.height;e.at(0x74)=30;pose(1);}
 static game::CollisionRect rectangle(){return {23,52,204,342};}
 bool hurt(){auto&e=payload;if(e.state==2||e.at(0x50)==5)return false;pose(5);--e.at(0x74);sounds.push_back("RESOURCE/punch2.wav");return true;}
 void terrain(const game::CollisionWorld&w,int material){auto&e=payload;auto body=[&](int x,int y){return w.body(rectangle(),x,y);};if(material!=1&&material!=2)return;for(int n=0;n<26;++n)if(!body(e.at(4),e.at(8)-n)){e.at(8)-=n;if(material==2)e.at(0x4c)+=2;return;}
 if(material==1){int x=e.at(4),old=e.at(0x40);int step=x>old?-1:1;if(x!=old)for(int v=x;(step<0&&v>=old)||(step>0&&v<=old);v+=step)if(!body(v,e.at(0x44))){e.at(4)=v;break;}}
 int y=e.at(8),old=e.at(0x44);if(y==old||(material==2&&y<old))return;int step=y>old?-1:1;for(int v=y;(step<0&&v>=old)||(step>0&&v<=old);v+=step)if(!body(e.at(4),v)){e.at(8)=v;return;}}
 void tick(int cx,int cy,int px,int pw,AssetStore&a,IndexedSurface&f){auto&e=payload;if(e.state==2)return;if(volley_cooldown_>0)--volley_cooldown_;e.at(0x40)=e.at(4);e.at(0x44)=e.at(8);int state=e.at(0x50),frame=e.at(0x10),t=e.at(0x14);
 if(state==1||state==2){if(e.at(0x60)>50){e.at(0x64)=state;pose(3);e.at(0x60)=0;}e.at(0x48)=state==1?-2:2;e.at(0x68)=state==2;}
 else if(state==3){if(frame==0&&t==0)e.at(0x68)=e.at(4)+e.at(0x6c)/2<=px+pw/2;if(frame==1&&t==1)volley(px);if(frame==3&&t==9)pose(px+pw/2<e.at(4)+e.at(0x6c)/2?1:2);}
 else if(state==5){if(frame==0&&t==0&&e.at(0x74)==0)e.at(0x4c)=-10;if(frame==0&&t==9){if(e.at(0x74)==0){e.state=2;ending_requested=true;return;}e.at(0x64)=5;pose(3);volley(px);}}
 int index=e.at(0x50)==5?hurtframe:e.at(0x50)==3?attacking[e.at(0x10)]:walking[e.at(0x10)];f.blit(a.image(names[index]),e.at(4)-cx,e.at(8)-cy,true,0,0,-1,-1,e.at(0x68)!=0);f.blit(a.image("bsfc41.raw"),10,50);for(int n=0;n<e.at(0x74);++n)f.blit(a.image("rlife.raw"),50+20*n,50);
 if(e.at(0x48)>0)--e.at(0x48);if(e.at(0x48)<0)++e.at(0x48);if(e.at(0x4c)<11)++e.at(0x4c);e.at(4)+=e.at(0x48);e.at(8)+=e.at(0x4c);if(++e.at(0x14)>e.at(0x1c)){e.at(0x14)=0;e.at(0x10)=(e.at(0x10)+1)%e.at(0x18);}++e.at(0x60);
 }
};
}

