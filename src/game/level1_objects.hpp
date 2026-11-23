#pragma once
#include "stage_scene.hpp"
#include "map_placement_assets.hpp"
#include "player_object_response.hpp"
#include "../assets/asset_store.hpp"
#include "collision_world.hpp"
#include <functional>
namespace shin {
class Level1Objects {
public:
 struct Object {std::size_t placement;int type;game::EntityPayload payload;bool removed=false;bool activated=false;};
private:
 AssetStore& assets_;std::vector<game::MapPlacement> placements_;std::vector<bool> spawned_;std::vector<Object> objects_;
 static const char* image(int type){switch(type){case 0x78:return "banana.raw";case 0x34:return "mug_jun.raw";case 0x35:return "mug_sin1.raw";case 0x2b:return "ball.app";case 0x2c:return "wbomb.app";case 0xb:return "ichi.app";case 0xc:return "iroa.app";case 0x18:return "item_m.app";case 0x19:return "item_f.app";case 0x1a:return "item_si.app";case 0x1b:return "item_ym.app";case 0x2a:return "life.app";case 0x1c:return "item_fr.app";case 0x26:return "item_w.app";default:break;}return ((type>=3&&type<=5)||(type==0x31||type==0x32))?"mb.app":type>=13&&type<=17?"sp_stop.app":type==10?"chocobi.app":type==0x76?"cho.app":"can.app";}
public:
 Level1Objects(AssetStore&a,const std::vector<game::MapPlacement>&p):assets_(a),placements_(p),spawned_(p.size()){spawn_visible(0,0,0,0,true);}
 static bool supports(int type){return type==0x78||type==0x34||type==0x35||(type>=3&&type<=5)||(type==0x31||type==0x32)||(type>=0xa&&type<=0xc)||type==0x18||type==0x19||type==0x1a||type==0x1b||type==0x2a||type==0x2b||type==0x2c||type==0x1c||type==0x26||type==0x2d||type==0x76||(type>=13&&type<=17);}
 void rearm(std::size_t i,const game::MapPlacement&m){placements_.at(i)=m;spawned_.at(i)=false;for(auto&o:objects_)if(o.placement==i)o.removed=true;}
 std::function<void(PlayerPayload&)> gate_contact;
 std::function<void(PlayerPayload&,game::EntityPayload&)> contact_override;
 std::vector<Object>& objects(){return objects_;}
 std::size_t available_count()const{std::size_t n=0;for(auto&p:placements_)n+=supports(p.type);return n;}
 void spawn_visible(int cx,int cy,int vx=0,int vy=0,bool preload_static=false){
  for(std::size_t i=0;i<placements_.size();++i){auto&m=placements_[i];bool trigger=m.type>=13&&m.type<=17;int ex=(trigger?20:0)+(vx+31)/32,ey=(trigger?15:0)+(vy+31)/32;bool dynamic=(m.type>=3&&m.type<=5)||m.type==0x31||m.type==0x32;bool outside=m.cell_x<cx/32-ex||m.cell_x>=cx/32+21+ex||m.cell_y<cy/32-ey||m.cell_y>=cy/32+16+ey;if(spawned_[i]||!supports(m.type)||(!preload_static&&outside))continue;
   auto&sprite=assets_.image(map_placement_asset(m.type));Object o{i,m.type,{}};auto&e=o.payload;bool platform=(m.type>=3&&m.type<=5)||(m.type==0x31||m.type==0x32);e.kind=platform?0x3e:trigger?0x3f:m.type==10?0x51:m.type==0x76?0x45:0x49;e.state=platform?1:0;e.at(4)=m.pixel_x(sprite.width);e.at(8)=m.pixel_y(sprite.height);
   switch(m.type){case 0x78:e.kind=0x50;break;case 0x34:e.kind=0x4a;break;case 0x35:e.kind=0x4b;break;case 0xb:e.kind=0x52;break;case 0xc:e.kind=0x56;break;case 0x18:e.kind=0x55;break;case 0x19:e.kind=0x53;break;case 0x1a:e.kind=0x57;break;case 0x1c:e.kind=0x54;break;case 0x26:e.kind=0x58;break;case 0x1b:e.kind=0x59;break;case 0x2a:e.kind=0x5a;break;case 0x2b:e.kind=0x47;break;case 0x2c:e.kind=0x48;break;}
   e.at(0xc)=5;e.at(0x10)=15;e.at(0x14)=26;e.at(0x18)=50;
   if(m.type==0x18||m.type==0x19||m.type==0x1a||m.type==0x1b||m.type==0x1c||m.type==0x26){e.at(0xc)=e.at(0x10)=0;e.at(0x14)=e.at(0x18)=31;}
   e.at(0x2c)=e.at(4);e.at(0x30)=e.at(8);e.at(0x48)=m.type;e.at(0x4c)=sprite.width;e.at(0x50)=sprite.height;if(platform){e.at(0xc)=0;e.at(0x10)=0;e.at(0x14)=62;e.at(0x18)=22;e.at(0x3c)=m.type==3?1:m.type==4?2:m.type==5?0:m.type==0x31?3:4;}if(trigger){e.at(0xc)=0;e.at(0x10)=0;e.at(0x14)=15;e.at(0x18)=15;e.at(0x40)=(m.type<<16)|(m.type-13);}objects_.push_back(o);spawned_[i]=true;
  }
 }
 void tick(int cx,int cy,IndexedSurface&target,PlayerPayload&player,game::ObjectResponseContext&context){
  spawn_visible(cx,cy,target.origin_x,target.origin_y);
  for(auto&o:objects_){if(o.removed)continue;auto&e=o.payload;
   const bool platform=(o.type>=3&&o.type<=5)||o.type==0x31||o.type==0x32;
   if(platform){
    // Allocate with the map, but start only when its original position enters view.
    const bool near=e.at(4)>=cx-target.origin_x-64&&e.at(4)<cx-target.origin_x+target.width+64&&e.at(8)>=cy-target.origin_y-64&&e.at(8)<cy-target.origin_y+target.height+64;
    if(!o.activated){if(!near)continue;o.activated=true;}
    if(!near){e.at(0x2c)=e.at(4);e.at(0x30)=e.at(8);continue;}
   }
   if(e.state==1){e.state=0;continue;}if(e.state==2){o.removed=true;continue;}
   if((o.type>=3&&o.type<=5)||(o.type==0x31||o.type==0x32)){
    e.at(0x2c)=e.at(4);e.at(0x30)=e.at(8);int state=e.at(0x3c);e.at(0x34)=state==1?-2:state==2?2:0;e.at(0x38)=state==3?-2:state==4?2:0;
    target.blit(assets_.image("mb.app"),e.at(4)-cx,e.at(8)-cy,true);
    if(e.at(0x34)>0)--e.at(0x34);if(e.at(0x34)<0)++e.at(0x34);if(e.at(0x38)>0)--e.at(0x38);if(e.at(0x38)<0)++e.at(0x38);e.at(4)+=e.at(0x34);e.at(8)+=e.at(0x38);
    game::CollisionRect rect={0,0,62,22},pr={player.at(0x2c),player.at(0x30),player.at(0x34),player.at(0x38)};
    for(auto&trigger:objects_)if(trigger.payload.kind==63&&!trigger.removed){auto&v=trigger.payload;game::CollisionRect vr={0,0,15,15};if(game::intersects(rect,e.at(4),e.at(8),vr,v.at(4),v.at(8)))e.at(0x3c)=v.at(0x40)&255;}
    if(game::intersects(pr,player.at(8),player.at(12),rect,e.at(4),e.at(8))){if(contact_override)contact_override(player,e);else game::respond_player_object(player,e,context);}
    continue;
   }
   if(o.type>=13&&o.type<=17)continue;
   if(o.type!=0x76&&e.at(0x3c)==1){
    struct ScreenOrigin{IndexedSurface&f;int x,y;~ScreenOrigin(){f.origin_x=x;f.origin_y=y;}} screen_origin{target,target.origin_x,target.origin_y};target.origin_x=target.origin_y=0;
    // 43be80: the collected chocobi moves before drawing, including on
    // the frame that crosses the bottom edge. Its screen path no longer
    // follows the camera and cannot collide with the player again.
    if(o.type==10){e.at(0x24)+=e.at(0x34);e.at(0x28)+=e.at(0x38);if(e.at(0x38)<11)++e.at(0x38);if(e.at(0x28)>target.height-target.origin_y){e.state=2;o.removed=true;}target.blit(assets_.image(image(o.type)),e.at(0x24),e.at(0x28),true);}
    else{target.blit(assets_.image(image(o.type)),e.at(0x24),e.at(0x28),true);e.at(0x24)+=e.at(0x34);e.at(0x28)+=e.at(0x38);if(e.at(0x38)<11)++e.at(0x38);if(e.at(0x28)>target.height-target.origin_y){e.state=2;o.removed=true;}}
    continue;
   }
   e.at(0x2c)=e.at(4);e.at(0x30)=e.at(8);e.at(0x24)=e.at(4)-cx;e.at(0x28)=e.at(8)-cy;
   target.blit(assets_.image(image(o.type)),e.at(0x24),e.at(0x28),true);
   if(e.at(0x34)>0)--e.at(0x34);if(e.at(0x34)<0)++e.at(0x34);e.at(4)+=e.at(0x34);
   game::CollisionRect a={player.at(0x2c),player.at(0x30),player.at(0x34),player.at(0x38)},b={e.at(0xc),e.at(0x10),e.at(0x14),e.at(0x18)};
   if(o.type==0x76){b={-50,-320,32,320};if(game::intersects(a,player.at(8),player.at(12),b,e.at(4),e.at(8))&&e.at(0x70)==0){if(!gate_contact)throw std::runtime_error("unbound gate40e660 contact");gate_contact(player);e.at(0x70)=1;}}
   else if(game::intersects(a,player.at(8),player.at(12),b,e.at(4),e.at(8))){if(contact_override)contact_override(player,e);else game::respond_player_object(player,e,context);}
  }
 }
};
}

