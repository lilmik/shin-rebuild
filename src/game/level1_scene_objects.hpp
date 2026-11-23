#pragma once
#include "player_object_response.hpp"
#include "collision_world.hpp"
#include "../assets/asset_store.hpp"
#include <functional>
namespace shin {
// Four first-stage map objects: original constructors428750,414c70,44d980,414350.
class Level1SceneObjects {
public:
 struct Object {int type;game::EntityPayload e;bool removed=false;int placement=-1;};
 using Event=std::function<void(unsigned,unsigned,unsigned,unsigned)>;
 std::vector<Object> objects;
 std::function<void(int)> respawn;
 static bool supports(int t){return t==0x41||t==0x75||t==0x7b||t==0x7c||t==0x57||t==0x7d||t==0x7e;}
 // Caller supplies dispatcher426380's exact world coordinates, not tile-origin guesses.
 void spawn(int type,int x,int y,int placement=-1){if(!supports(type))throw std::invalid_argument("scene map type");Object o{type,{}};o.placement=placement;auto&e=o.e;e.kind=type==0x7b?0x2e:type==0x7c?0x2f:type==0x57?0x4c:type==0x7d?0x30:type==0x7e?0x31:0x45;e.state=1;e.at(4)=x;e.at(8)=y;e.at(0x2c)=x;e.at(0x30)=y;e.at(0x48)=type;if(type==0x7b||type==0x7c||type==0x57||type==0x7d||type==0x7e){e.at(0xc)=0;e.at(0x10)=0;e.at(0x14)=79;e.at(0x18)=91;e.at(0x4c)=type==0x7b?73:type==0x57?79:58;e.at(0x50)=type==0x7b?102:type==0x57?91:104;}objects.push_back(o);}
 void tick(int cx,int cy,AssetStore&assets,IndexedSurface&frame,PlayerPayload&player,game::ObjectResponseContext&ctx,const Event&event){
  game::CollisionRect pr={player.at(0x2c),player.at(0x30),player.at(0x34),player.at(0x38)};
  for(auto&o:objects){if(o.removed)continue;auto&e=o.e;if(e.state==2){o.removed=true;continue;}if(e.state==1){e.state=0;continue;}
   bool trigger=o.type==0x41||o.type==0x75;
   if(trigger){game::CollisionRect rect={o.type==0x41?-std::min(160,std::max(0,(frame.width-640)/2)):0,-320,32,320};if(game::intersects(pr,player.at(8),player.at(12),rect,e.at(4),e.at(8))){if(!event)throw std::runtime_error("unbound first-stage scene event");if(o.type==0x41)event(0x443cd0,0x443e30,0x444170,6);else event(0x436030,0x436130,0x4362c0,6);e.state=2;}continue;}
   e.at(0x2c)=e.at(4);e.at(0x30)=e.at(8);e.at(0x24)=e.at(4)-cx;e.at(0x28)=e.at(8)-cy;
   const auto&icon=assets.image(o.type==0x7b?"gamecar.app":o.type==0x57?"mole_m.app":o.type==0x7d?"gamepuzl.app":o.type==0x7e?"gamecard.app":"gamech_b.app");e.at(0x4c)=icon.width;e.at(0x50)=icon.height;frame.blit(icon,e.at(0x24),e.at(0x28),true);
   if(e.at(0x34)>0)--e.at(0x34);if(e.at(0x34)<0)++e.at(0x34);e.at(4)+=e.at(0x34);
   game::CollisionRect rect={0,0,79,91};if(game::intersects(pr,player.at(8),player.at(12),rect,e.at(4),e.at(8)))game::respond_player_object(player,e,ctx);
  }
 }
};
}
