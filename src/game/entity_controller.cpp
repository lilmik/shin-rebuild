#include "entity_controller.hpp"
#include <stdexcept>
#include <cstdlib>
namespace shin {
void EntityController::set_state(int n){
 s_.state=n;s_.frame=0;s_.frame_tick=0;s_.frame_count=n==0?1:n==3?3:4;s_.frame_delay=n==0?100:10;
 if(s_.map_type==0)s_.animation_table=n==0?0x45bf8c:0x45bf90;
 else{s_.animation_table=(s_.map_type==1?0x470ab8:0x46e5a0)+(n==0?0:n==4?40:n==3?24:8);}
 if(n==(s_.map_type==0?3:4)){s_.animation_table=s_.map_type==0?0x45bfa0:s_.map_type==1?0x470ae0:0x46e5c8;s_.frame_count=2;s_.frame_delay=s_.map_type==0?10:20;}
}
EntityController::EntityController(int type,int x,int y,std::int16_t tag,EntityWorld& w):s_{type,x,y,x,y}{
 if(type<0||type>2)throw std::invalid_argument("unsupported entity map type");s_.map_tag=tag;set_state(1);
 static constexpr std::uint32_t terrain[]={0x403c30,0x446140,0x439180},object[]={0x403c80,0x446190,0x4391d0};
 w.register_collision(s_,terrain[type],object[type]);
}
void EntityController::update(EntityWorld& w){
 auto&s=s_;int t=s.map_type;
 if(std::abs(s.x-w.camera_x()-320)>960){
  static constexpr std::uint32_t widths[]={0x473844,0x78f358,0x77ea48},heights[]={0x4738f8,0x78f478,0x77eb68};
  if(s.state!=(t==0?3:4))w.respawn_record(s,widths[t],heights[t]);s.pending_delete=true;
 }
 s.previous_x=s.x;s.previous_y=s.y;
 if(t!=0&&(s.state==1||s.state==2||(t==1&&s.state==0))&&s.attack_tick==200){s.prior_state=s.state;set_state(3);s.attack_tick=0;}
 if(s.state==1){s.vx=-2;s.facing=0;}else if(s.state==2){s.vx=2;s.facing=1;}
 else if(t!=0&&s.state==3){
  if(s.frame==0&&s.frame_tick==1)s.facing=s.x+s.width/2<=w.player_x()+w.player_width()/2;
  if(s.frame==2&&s.frame_tick==9){
   int px=s.x+(s.facing?s.width-10:0);
   int velocity=t==1?(w.player_x()-s.x)/32:(s.facing?4:-4);
   if(t==1){if(velocity>8)velocity=7;if(velocity< -8)velocity=-7;}
   w.spawn_projectile(t==1?0x5b:0x5f,px,s.y+30,velocity,t==1?-16:0,t==1?0x4366b0:0x42d380);
   int old=s.prior_state;if(old>=0&&old<=2)set_state(old);
  }
 }
 int death=t==0?3:4;
 if(s.state==death){
  if(s.frame==0&&s.frame_tick==0)s.vy=-10;
  if(s.frame==1&&s.frame_tick==0){s.frame_delay=10;s.vy=10;s.pending_delete=true;w.defeated(s);}
  if(s.frame==1&&s.frame_tick==9)s.pending_delete=true;
 }
 if(w.render_tick()){
  static constexpr std::uint32_t left[]={0x473848,0x78f47c,0x77ea44},right[]={0x473820,0x78f230,0x77eb6c};
  w.render(s,s.x-w.camera_x(),s.y-w.camera_y(),left[t],right[t]);
 }
 if(s.vx>0)--s.vx;if(s.vx<0)++s.vx;if(s.vy<11)++s.vy;s.x+=s.vx;s.y+=s.vy;
 if(++s.frame_tick>s.frame_delay){s.frame_tick=0;if(++s.frame>=s.frame_count)s.frame=0;}
 if(t!=0)++s.attack_tick;
}
}
