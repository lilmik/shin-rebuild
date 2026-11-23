#pragma once
#include <array>
#include <cstdint>
#include <initializer_list>
#include <stdexcept>
namespace shin {
enum class ProjectileKind { ball,water_bomb };
struct Projectile {
 ProjectileKind kind=ProjectileKind::ball;
 std::array<std::int32_t,18> words{};
 std::uint8_t owner_state=1;bool explosion_queued=false;
 std::int32_t& at(unsigned o){if(o%4||o>=0x48)throw std::out_of_range("projectile field");return words[o/4];}
 const char* resource()const{return kind==ProjectileKind::ball?"ball.app":"wbomb.app";}
};
struct ProjectileContext {
 virtual ~ProjectileContext()=default;
 virtual int camera_x()const=0;virtual int camera_y()const=0;
 virtual bool present()const=0;
 virtual int register_collision(unsigned registry,unsigned callback,Projectile&)=0;
 virtual void unregister_collision(unsigned registry,int handle)=0;
 virtual void render(Projectile&)=0;
 // 431130 occupancy and42b300 water-bomb detonation are mandatory dependencies.
 virtual int call(unsigned function,Projectile&,std::initializer_list<std::int64_t>)=0;
 // Complete entity-specific response requires target payload (dozens of entity types).
 virtual void entity_collision(unsigned callback,Projectile&,unsigned target_kind,void* target)=0;
 virtual void release_allocation(int token)=0;
};
void initialize_projectile(Projectile&,ProjectileKind,int x,int y,int vx,int width,int height,ProjectileContext&);
void update_projectile(Projectile&,ProjectileContext&);
void projectile_terrain_collision(Projectile&,int side,ProjectileContext&);
void projectile_entity_collision(Projectile&,unsigned target_kind,void* target,ProjectileContext&);
void destroy_projectile(Projectile&,int owner_allocation_token,ProjectileContext&);
}
