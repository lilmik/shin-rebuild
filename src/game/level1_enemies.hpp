#pragma once
#include "entity_controller.hpp"
#include "player_animation.hpp"
#include "collision_world.hpp"
#include <memory>
#include <functional>
#include <vector>
#include <set>
namespace shin {
struct EnemyProjectileEvent{int type,x,y,vx,vy;unsigned callback;};
class Level1Enemies:private EntityWorld{
 AssetStore&assets_;const MapData&map_;const AppImage&blocks_;PlayerPayload&player_;int cx_=0,cy_=0;IndexedSurface*frame_=nullptr;
 std::vector<std::unique_ptr<EntityController>> enemies_;
 struct Registration{EntityState*state;unsigned object,terrain;};std::vector<Registration>registrations_;
 int camera_x()const override{return cx_;}int camera_y()const override{return cy_;}bool render_tick()const override{return frame_!=nullptr;}
 int player_x()const override{return player_.at(8);}int player_width()const override{return player_.at(0x10);}
 void register_collision(EntityState&s,unsigned object,unsigned terrain)override{registrations_.push_back({&s,object,terrain});}
 void respawn_record(const EntityState&s,unsigned,unsigned)override{if(s.map_tag<0||!dead_placements.count(s.map_tag))respawns.push_back(s);}
 void render(const EntityState&,int,int,unsigned,unsigned)override;
 void spawn_projectile(int t,int x,int y,int vx,int vy,unsigned c)override{projectiles.push_back({t,x,y,vx,vy,c});}
 void defeated(const EntityState&s)override{defeated_types.push_back(s.map_type);}
public:
 std::set<short> dead_placements;
 std::vector<std::string> sound_requests;
 std::vector<EnemyProjectileEvent>projectiles;std::vector<EntityState>respawns;std::vector<int>defeated_types;
 Level1Enemies(AssetStore&a,const MapData&m,const AppImage&b,PlayerPayload&p):assets_(a),map_(m),blocks_(b),player_(p){}
 void spawn(int type,int x,int y,short tag=-1){if(tag>=0){if(dead_placements.count(tag))return;for(auto&e:enemies_)if(e->state().map_tag==tag&&!e->state().pending_delete)return;}enemies_.push_back(std::make_unique<EntityController>(type,x,y,tag,static_cast<EntityWorld&>(*this)));auto& s=enemies_.back()->state();s.width=assets_.image(type==0?"c1_f.app":type==1?"c2_f.app":"c3_f.app").width;}
 void spawn_stationary(int x,int y){spawn(0,x,y,-1);enemies_.back()->stand_still();}
 void tick(int camera_x,int camera_y,IndexedSurface&);
 // Projectile hits mark source death state; scoring/sound provided explicitly by caller.
 void defeat(EntityState&);
 void platform(EntityState& state,int direction){for(auto&e:enemies_)if(&e->state()==&state){e->platform(direction);return;}}
 bool hit(const game::CollisionRect&,int x,int y);
 std::vector<EntityState*> targets(){std::vector<EntityState*>out;for(auto&e:enemies_)if(!e->state().pending_delete)out.push_back(&e->state());return out;}
 static const char* image_name(const EntityState&);
};
}

