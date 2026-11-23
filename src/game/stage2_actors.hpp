#pragma once
#include "stage2_mouse.hpp"
#include "stage2_guard.hpp"
#include "stage2_boss.hpp"
#include "stage2_extra_enemies.hpp"
#include "stage2_remaining_entities.hpp"
#include "player_animation.hpp"
#include <variant>
#include <memory>
#include <set>
namespace shin::game {
class Stage2Actors {
 using Actor=std::variant<Stage2Mouse,Stage2Guard,Stage2Boss,Stage2ExtraEnemy,Stage2RemainingEnemy>;
 struct Entry {std::size_t placement;Actor actor;};
 AssetStore&assets_;CollisionWorld world_;PlayerPayload&player_;std::vector<std::unique_ptr<Entry>>actors_;
 template<class T>void collect(T&a){for(auto&f:a.sound_requests)sounds.push_back(f);a.sound_requests.clear();}
 template<class T>void collect_events(T&a){for(auto&p:a.projectiles)projectiles.push_back({p.x,p.y,p.vx,p.vy,p.callback});a.projectiles.clear();if constexpr(!std::is_same_v<T,Stage2ExtraEnemy>&&!std::is_same_v<T,Stage2RemainingEnemy>){for(int kind:a.spawn_requests)spawn_requests.push_back(kind);a.spawn_requests.clear();}collect(a);}
public:
 struct ProjectileEvent {int x,y,vx,vy;unsigned callback;};
 struct RespawnRecord{std::size_t placement;int x,y;};std::vector<RespawnRecord>respawn_records;std::set<std::size_t> dead_placements;
 std::vector<ProjectileEvent>projectiles;std::vector<int>spawn_requests;std::vector<std::size_t>respawn_indices;std::vector<std::string>sounds;bool boss_defeated=false;unsigned defeated_count=0;
 Stage2Actors(AssetStore&a,const MapData&m,const AppImage&b,PlayerPayload&p):assets_(a),world_(m,b),player_(p){}
 Stage2Boss* boss(){for(auto&n:actors_)if(auto*b=std::get_if<Stage2Boss>(&n->actor))return b;return nullptr;}
 bool spawn(int maptype,int x,int y,std::size_t index){
  if(dead_placements.count(index))return false;for(auto&n:actors_)if(n->placement==index)return false;
  if(maptype==31)actors_.push_back(std::make_unique<Entry>(Entry{index,Stage2Mouse(x,y,assets_)}));
  else if(maptype==36)actors_.push_back(std::make_unique<Entry>(Entry{index,Stage2Guard(x,y,assets_)}));
  else if(maptype==32)actors_.push_back(std::make_unique<Entry>(Entry{index,Stage2Boss(x,y,assets_)}));
  else if(Stage2RemainingEnemy::supports(maptype))actors_.push_back(std::make_unique<Entry>(Entry{index,Stage2RemainingEnemy(maptype,x,y,assets_)}));
  else if(Stage2ExtraEnemy::supports(maptype))actors_.push_back(std::make_unique<Entry>(Entry{index,Stage2ExtraEnemy(maptype,x,y,assets_)}));else return false;
  std::visit([&](auto&a){using T=std::decay_t<decltype(a)>;if constexpr(!std::is_same_v<T,Stage2Mouse>)collect_events(a);},actors_.back()->actor);return true;
 }
 static CollisionRect rectangle(EntityPayload&e){unsigned r=(e.kind==0x70||e.kind==0x71||e.kind==0x68||e.kind==0x69)?0x28:0x20;return {e.at(r),e.at(r+4),e.at(r+8),e.at(r+12)};}
 static int death_state(EntityPayload&e){return (e.kind==0x70||e.kind==0x71)?3:e.kind==0x72||e.kind==0x68||e.kind==0x69?4:7;}
 static bool dying(EntityPayload&e){return e.at((e.kind==0x70||e.kind==0x71||e.kind==0x68||e.kind==0x69)?0x58:0x50)==death_state(e);}
 std::vector<EntityPayload*>targets(){std::vector<EntityPayload*>r;for(auto&n:actors_)std::visit([&](auto&a){if(a.payload.state==0)r.push_back(&a.payload);},n->actor);return r;}
 // Exact helper dispatch for recovered player callbacks and unconditional ability clears.
 bool call_helper(unsigned address,EntityPayload&e){bool found=false;for(auto&n:actors_)std::visit([&](auto&a){using T=std::decay_t<decltype(a)>;if(&a.payload!=&e)return;if constexpr(std::is_same_v<T,Stage2Mouse>){if(address!=0x442a20)return;}else if constexpr(std::is_same_v<T,Stage2Guard>){if(address!=0x44d220)return;}else if constexpr(std::is_same_v<T,Stage2RemainingEnemy>){if(address!=0x41b4b0)return;}else if constexpr(std::is_same_v<T,Stage2ExtraEnemy>){if(address!=(e.kind==0x68?0x41a4e0u:0x44c610u))return;}else{if(address!=0x438670)return;}a.hurt();collect(a);found=true;},n->actor);return found;}
 bool hurt(EntityPayload&e){for(auto&n:actors_){bool found=false;std::visit([&](auto&a){if(&a.payload==&e&&e.state==0&&!dying(e)){a.hurt();collect(a);found=true;}},n->actor);if(found)return true;}return false;}
 bool hit(const CollisionRect&r,int x,int y){bool result=false;for(auto*e:targets())if(!dying(*e)&&intersects(r,x,y,rectangle(*e),e->at(4),e->at(8)))result=hurt(*e)||result;return result;}
 void platform(EntityPayload&e,int direction){for(auto&n:actors_)std::visit([&](auto&a){if(&a.payload==&e)a.platform(direction);},n->actor);}
 void tick(int cx,int cy,IndexedSurface&frame){
  for(auto&n:actors_)std::visit([&](auto&a){using T=std::decay_t<decltype(a)>;
   if constexpr(std::is_same_v<T,Stage2Mouse>||std::is_same_v<T,Stage2ExtraEnemy>||std::is_same_v<T,Stage2RemainingEnemy>)a.tick(cx,cy,assets_,frame);else a.tick(cx,cy,player_.at(8),player_.at(0x10),assets_,frame);
   if(a.payload.state==0){auto&e=a.payload;int material=world_.body(rectangle(e),e.at(4),e.at(8));if(material)a.terrain(world_,material);}
   if constexpr(std::is_same_v<T,Stage2Mouse>)collect(a);else collect_events(a);
   if(dying(a.payload)||a.defeated)dead_placements.insert(n->placement);
   if(a.respawn_requested&&!dead_placements.count(n->placement)){respawn_indices.push_back(n->placement);respawn_records.push_back({n->placement,a.payload.at(4),a.payload.at(8)});}
   if(a.defeated){++defeated_count;if constexpr(std::is_same_v<T,Stage2Boss>)boss_defeated=true;}
  },n->actor);
  actors_.erase(std::remove_if(actors_.begin(),actors_.end(),[](const auto&n){return std::visit([](const auto&a){return a.payload.state==2;},n->actor);}),actors_.end());
 }
};
}
