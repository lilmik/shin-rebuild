#pragma once
#include "player_object_response.hpp"
#include "../assets/asset_store.hpp"
#include <set>
#include <algorithm>
namespace shin::game {
// Original constructors415d80/449820/43f570 and callbacks415e90/449930/43f680.
// Sprite indices are read from the original PE tables45ed78/4713f8/46f480.
class FlyingEnemyManager {
 struct Actor {EntityPayload e;int type,index;bool fresh=true;};
 std::vector<Actor> actors_;
 std::set<int> spawned_;
 static void pose(EntityPayload&e,int state){e.at(0x50)=state;e.at(0x10)=e.at(0x14)=0;e.at(0x18)=state==3?(e.kind==0x1f?6:e.kind==0x20?2:1):1;e.at(0x1c)=state==3&&e.kind==0x1f?2:10;}
 static const char* image(const EntityPayload&e,int state,int f){
  static constexpr const char*a[]={"shutba.raw","shutbb.raw","shutca.raw","shutcb.raw","shutcc.raw","shutcd.raw","shutce.raw","shutcf.raw"};
  static constexpr const char*b[]={"shutbc.raw","shutbd.raw","shutea.raw","shuteb.raw"};
  static constexpr const char*c[]={"shutbe.raw","shutbf.raw"};
  int i=state==4?1:state==3?(e.kind==0x1f?2+f:e.kind==0x20?2+f:0):0;
  return e.kind==0x1f?a[std::clamp(i,0,7)]:e.kind==0x20?b[std::clamp(i,0,3)]:c[std::clamp(i,0,1)];
 }
 static void movement(Actor&a){auto&e=a.e;auto&vx=e.at(0x48);auto&vy=e.at(0x4c);int t=e.at(0x6c);if(e.kind!=0x1f){vx=e.at(0x50)==2?2:-2;if(e.at(0x50)!=3)e.at(0x60)=e.at(0x50)==2;return;}
 switch(a.type){case 0x3c:vx=-2;if(t<100)vy=2;break;case 0x3d:vx=-2;if(t<100)vy=-2;break;case 0x3e:vx=-2;vy=((e.at(0x58)/100)&1)?2:-2;break;case 0x3f:if(t<50){vx=-2;vy=2;}else if(t<500){vx=2;vy=2;}break;case 0x40:vx=t<20?-2:-8;vy=0;break;default:vx=-2;break;}
 if(e.at(0x50)!=3)e.at(0x60)=0;
 }
public:
 struct ProjectileEvent {int x,y,vx,vy;unsigned callback;int kind;};
 std::vector<ProjectileEvent> projectile_events;
 std::vector<std::string> sounds;
 static bool supports(int type){return type==0x37||type==0x38||type==0x39||(type>=0x3c&&type<=0x40);}
 void clear(){actors_.clear();spawned_.clear();projectile_events.clear();sounds.clear();}
 void spawn(int type,int x,int y,int index){if(!supports(type)||!spawned_.insert(index).second)return;Actor a{};a.type=type;a.index=index;auto&e=a.e;e.kind=type==0x38?0x20:type==0x39?0x21:0x1f;e.at(4)=e.at(0x40)=x;e.at(8)=e.at(0x44)=y;e.at(0x54)=type;e.at(0x20)=28;e.at(0x24)=21;e.at(0x28)=74;e.at(0x2c)=89;pose(e,1);actors_.push_back(a);}
 std::vector<EntityPayload*> targets(){std::vector<EntityPayload*>r;for(auto&a:actors_)if(a.e.state!=2&&a.e.at(0x50)!=4)r.push_back(&a.e);return r;}
 bool hurt(EntityPayload&e){if(e.state==2||e.at(0x50)==4)return false;pose(e,4);sounds.push_back("RESOURCE/punch2.wav");return true;}
 void tick(int cx,int cy,PlayerPayload&player,AssetStore&assets,IndexedSurface&frame){
 for(auto&a:actors_){auto&e=a.e;if(e.state==2)continue;if(a.fresh){a.fresh=false;continue;}if(std::abs(e.at(4)-cx-320)>960){e.state=2;continue;}e.at(0x40)=e.at(4);e.at(0x44)=e.at(8);e.at(0x38)=e.at(4)-cx;e.at(0x3c)=e.at(8)-cy;
 const auto&im=assets.image(e.kind==0x1f?"shutcf.raw":e.kind==0x20?"shuteb.raw":"shutbf.raw");e.at(0x64)=im.width;e.at(0x68)=im.height;
 int state=e.at(0x50),f=e.at(0x10),t=e.at(0x14);
 if(state<=2){int interval=e.kind==0x1f?40:e.kind==0x20?50:200;if(e.at(0x58)==interval){e.at(0x5c)=state;pose(e,3);e.at(0x58)=0;}if(state==1||state==2)movement(a);}
 else if(state==3){if(e.kind==0x1f){movement(a);e.at(0x60)=0;}if(f==0&&t==1)e.at(0x60)=e.at(4)+e.at(0x64)/2<=player.at(8)+player.at(0x10)/2;
 bool fire=e.kind==0x1f?f==5&&t==1:e.kind==0x20?f==1&&t==9:f==0&&t==9;
 if(fire&&e.at(4)+e.at(0x64)>=cx&&e.at(4)<cx+640){if(e.kind==0x1f){int x=e.at(4)+(e.at(0x60)?e.at(0x64)-10:0);projectile_events.push_back({x,e.at(8)+30,e.at(0x60)?8:-8,0,0x42cc00,0x24});}else if(e.kind==0x20){int x=e.at(4),y=e.at(8)+30;projectile_events.push_back({x,y,(player.at(8)-x)/40,(player.at(12)-y)/40,0x41e610,0x25});}pose(e,e.at(0x5c));}}
 else if(state==4){if(f==0&&t==0)e.at(0x4c)=-10;if(f==0&&t==9){e.at(0x1c)=10;e.at(0x4c)=10;e.state=2;}}
 frame.blit(assets.image(image(e,e.at(0x50),e.at(0x10))),e.at(4)-cx,e.at(8)-cy,true,0,0,-1,-1,e.at(0x60)!=0);
 if(e.at(0x48)>0)--e.at(0x48);if(e.at(0x48)<0)++e.at(0x48);e.at(4)+=e.at(0x48);e.at(8)+=e.at(0x4c);
 if(++e.at(0x14)>e.at(0x1c)){e.at(0x14)=0;if(++e.at(0x10)>=e.at(0x18))e.at(0x10)=0;}++e.at(0x58);if(e.kind==0x1f)++e.at(0x6c);
 }
 }
};
}



