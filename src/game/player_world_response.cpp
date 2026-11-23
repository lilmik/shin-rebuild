#include "player_world_response.hpp"
namespace shin::game {
WorldResponse respond_player_world(PlayerPayload&p,const CollisionWorld&w,int kind,int budget){
 WorldResponse out;auto&x=p.at(8);auto&y=p.at(12);CollisionRect r{p.at(0x2c),p.at(0x30),p.at(0x34),p.at(0x38)};
 auto body=[&](int xx,int yy){return w.body(r,xx,yy);};
 auto spend=[&](){if(budget<=0){out.safety_limit_reached=true;return false;}--budget;return true;};
 if(kind==5){p.at(0x1c)=0;p.at(0x20)=0;p.at(0x5c)=4;p.at(0x18)=0x45c740;p.at(0x24)=2;p.at(0x28)=5;p.at(0xb4)=1;out.hurt_sound_requested=true;return out;}
 if(kind==3||kind==4){for(int i=0;i<16;++i){int side=kind==3?w.right(r,x,y):w.left(r,x,y);if(side||w.ground(r,x,y))x+=kind==3?-1:1;}if(w.ground(r,x,y)){do{if(!spend())return out;--y;}while(w.ground(r,x,y));}return out;}
 if(kind!=1&&kind!=2)return out;
 for(int i=0;i<26;++i){if(!body(x,y-i)){y-=i;if(kind==2)p.at(100)=1;return out;}}
 int oldx=p.at(0x4c),oldy=p.at(0x50);
 if(kind==2){if(y<=oldy)return out;for(int yy=y;yy>=oldy;--yy){if(!spend())return out;if(!body(x,yy)){y=yy;return out;}}return out;}
 if(x!=oldx){int step=x>oldx?-1:1;for(int xx=x;step<0?xx>=oldx:xx<=oldx;xx+=step){if(!spend())return out;if(!body(xx,oldy)){x=xx;break;}}}
 if(y!=oldy){int step=y>oldy?-1:1;for(int yy=y;step<0?yy>=oldy:yy<=oldy;yy+=step){if(!spend())return out;if(!body(x,yy)){y=yy;return out;}}}
 return out;
}
}
