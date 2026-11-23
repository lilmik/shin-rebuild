#include "level1_enemies.hpp"
#include <algorithm>
namespace shin {
#include "level1_enemy_assets.inc"
const char* Level1Enemies::image_name(const EntityState&s){
 unsigned base=s.map_type==0?0x45bf8c:s.map_type==1?0x470ab8:0x46e5a0;
 unsigned i=(s.animation_table-base)/4+unsigned(s.frame);if(i>=unsigned(s.map_type==0?7:12))throw std::runtime_error("enemy animation index");
 int n=(s.map_type==0?indices0:s.map_type==1?indices1:indices2)[i];
 if(n<0||n>=(s.map_type==0?7:10))throw std::runtime_error("enemy resource index");
 return(s.map_type==0?names0:s.map_type==1?names1:names2)[n];
}
void Level1Enemies::render(const EntityState&s,int x,int y,unsigned,unsigned){if(frame_)frame_->blit(assets_.image(image_name(s)),x,y,true,0,0,-1,-1,s.facing!=0);}
void Level1Enemies::tick(int x,int y,IndexedSurface&f){cx_=x;cy_=y;frame_=&f;game::CollisionWorld world(map_,blocks_);
 for(auto&e:enemies_){auto&s=e->state();if(s.pending_delete)continue;e->update(*this);if(s.map_tag>=0&&s.state==(s.map_type==0?3:4))dead_placements.insert(s.map_tag);game::CollisionRect r={28,14,46,71};int kind=world.body(r,s.x,s.y);
  if(kind==1||kind==2){bool fixed=false;for(int n=0;n<26;++n)if(world.body(r,s.x,s.y-n)==0){s.y-=n;if(kind==2)s.vy+=2;fixed=true;break;}
   if(!fixed&&kind==1){int step=s.previous_x<s.x?-1:1;for(int xx=s.x;step<0?xx>=s.previous_x:xx<=s.previous_x;xx+=step)if(world.body(r,xx,s.previous_y)==0){s.x=xx;break;}}
   if(!fixed){int step=s.previous_y<s.y?-1:1;if(kind==1||s.y>s.previous_y)for(int yy=s.y;step<0?yy>=s.previous_y:yy<=s.previous_y;yy+=step)if(world.body(r,s.x,yy)==0){s.y=yy;break;}}
  }
 }
 frame_=nullptr;
}
void Level1Enemies::defeat(EntityState&s){
 if(s.pending_delete||s.state==(s.map_type==0?3:4))return;
 if(s.map_tag>=0)dead_placements.insert(s.map_tag);
 s.state=s.map_type==0?3:4;s.frame=s.frame_tick=0;s.frame_count=2;s.frame_delay=s.map_type==0?10:20;
 s.animation_table=s.map_type==0?0x45bfa0:s.map_type==1?0x470ae0:0x46e5c8;
 sound_requests.push_back(s.map_type==0?"RESOURCE/dem-boy.wav":s.map_type==1?"RESOURCE/dem-h2.wav":"RESOURCE/punch2.wav");
}
bool Level1Enemies::hit(const game::CollisionRect&r,int x,int y){bool result=false;for(auto&e:enemies_){auto&s=e->state();if(s.pending_delete||s.state==(s.map_type==0?3:4))continue;if(game::intersects(r,x,y,{28,14,46,71},s.x,s.y)){defeat(s);result=true;}}return result;}
}
