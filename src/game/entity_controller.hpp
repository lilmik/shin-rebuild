#pragma once
#include <cstdint>
#include <array>
namespace shin {
struct EntityState {
 int map_type,x,y,previous_x,previous_y,vx=0,vy=0,state=1,prior_state=0,frame=0,frame_tick=0,frame_count=4,frame_delay=10,attack_tick=100,width=46,facing=0;
 std::int16_t map_tag=0;bool pending_delete=false,stationary=false;std::uint32_t animation_table=0;
};
// Context obligations are pure virtual: collision registration, respawn, rendering and projectiles cannot silently disappear.
struct EntityWorld {
 virtual ~EntityWorld()=default;
 virtual int camera_x()const=0;virtual int camera_y()const=0;virtual bool render_tick()const=0;
 virtual int player_x()const=0;virtual int player_width()const=0;
 virtual void register_collision(EntityState&,std::uint32_t terrain_callback,std::uint32_t object_callback)=0;
 virtual void respawn_record(const EntityState&,std::uint32_t source_width_global,std::uint32_t source_height_global)=0;
 virtual void render(const EntityState&,int screen_x,int screen_y,std::uint32_t left_atlas_global,std::uint32_t right_atlas_global)=0;
 virtual void spawn_projectile(int type,int x,int y,int vx,int vy,std::uint32_t callback)=0;
 virtual void defeated(const EntityState&)=0;
};
class EntityController {
 EntityState s_;
 void set_state(int state);
public:
 EntityController(int map_type,int x,int y,std::int16_t tag,EntityWorld&);
 EntityState& state(){return s_;}const EntityState& state()const{return s_;}
 void stand_still(){s_.stationary=true;s_.vx=0;set_state(0);}
 void platform(int direction){if(!s_.stationary&&!s_.pending_delete&&s_.state!=(s_.map_type==0?3:4)&&direction>=0&&direction<=2)set_state(direction);}
 void update(EntityWorld&);
};
}
