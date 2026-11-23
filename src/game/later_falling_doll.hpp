#pragma once
#include "later_mechanisms.hpp"
namespace shin {
// MAP79 second dispatcher427610:404690/4047c0/4048d0.
class LaterFallingDoll {
 unsigned last_step_=0;bool clock_started_=false;
public:
 game::EntityPayload e;std::vector<unsigned>sounds;bool updated_this_tick=false,active_this_tick=false;
 LaterFallingDoll(int x,int y){e.kind=0x3c;e.state=1;e.at(4)=e.at(0x34)=x;e.at(8)=e.at(0x38)=y;e.at(0x3c)=-6;e.at(0x40)=-8;sounds.push_back(0x47391c);}
 static game::CollisionRect rectangle(){return {18,19,108,109};}
 void tick(int cx,int cy,AssetStore&a,IndexedSurface&f,unsigned now=~0u){updated_this_tick=active_this_tick=false;if(e.state==2)return;if(e.state==1){e.state=0;return;}if(std::abs(e.at(4)-cx-320)>=961){clock_started_=false;return;}
 active_this_tick=true;
 // Real-time playability cadence; source per-step velocities stay unchanged.
 if(now!=~0u){if(!clock_started_){clock_started_=true;last_step_=now;}else if(now-last_step_<32){f.blit(a.image("dol.raw"),e.at(4)-cx,e.at(8)-cy);return;}last_step_=now;}
 updated_this_tick=true;e.at(0x34)=e.at(4);e.at(0x38)=e.at(8);f.blit(a.image("dol.raw"),e.at(4)-cx,e.at(8)-cy);if(e.at(0x40)<11)++e.at(0x40);e.at(4)+=e.at(0x3c);e.at(8)+=e.at(0x40);}
 void terrain(const game::CollisionWorld&w,int material){if(material==5){e.state=2;return;}if(material!=1&&material!=2)return;auto clear=[&](int x,int y){return w.body(rectangle(),x,y)==0;};auto bouncey=[&](int extra){e.at(0x40)=-(e.at(0x40)+extra);++e.at(0x44);};for(int i=0;i<26;++i)if(clear(e.at(4),e.at(8)-i)){e.at(8)-=i;bouncey(material==2?2:0);return;}if(material==2){if(e.at(8)<=e.at(0x38))return;for(int y=e.at(8);y>=e.at(0x38);--y)if(clear(e.at(4),y)){e.at(8)=y;bouncey(0);return;}return;}int oldx=e.at(0x34),oldy=e.at(0x38);int x=e.at(4);if(x!=oldx){int step=x>oldx?-1:1;for(;;x+=step){if(clear(x,oldy)){e.at(4)=x;e.at(0x3c)=-e.at(0x3c);++e.at(0x44);break;}if(x==oldx)break;}}int y=e.at(8);if(y!=oldy){int step=y>oldy?-1:1;for(;;y+=step){if(clear(e.at(4),y)){e.at(8)=y;bouncey(0);return;}if(y==oldy)break;}}}
};
}
