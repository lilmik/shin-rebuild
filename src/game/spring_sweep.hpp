#pragma once
#include "player_object_response.hpp"
#include "collision_world.hpp"
#include <algorithm>
namespace shin {
// A downward crossing of the spring's top surface, evaluated at time of impact.
// x/y are player top-left positions; rect carries the player's collision offsets.
inline bool spring_sweep_landing(const game::CollisionRect&r,int oldx,int oldy,int x,int y,int sx,int sy,int width=56,int oldsx=0,int oldsy=0,bool moving=false){
 if(!moving){oldsx=sx;oldsy=sy;}
 const double before=double(oldy)+r[3]-oldsy,after=double(y)+r[3]-sy;
 if(after<=before||before>0||after<0)return false;
 const double t=-before/(after-before);
 const double px=oldx+(x-oldx)*t, springx=oldsx+(sx-oldsx)*t;
 return px+r[2]>springx&&px+r[0]<springx+width;
}
inline bool spring_swept_contact(PlayerPayload&p,game::EntityPayload&e,game::ObjectResponseContext&ctx,int oldx,int oldy,int oldsx,int oldsy){
 if(p.at(0x58)<=0)return false;
 game::CollisionRect r={p.at(0x2c),p.at(0x30),p.at(0x34),p.at(0x38)};
 if(!spring_sweep_landing(r,oldx,oldy,p.at(8),p.at(12),e.at(4),e.at(8),56,oldsx,oldsy,true))return false;
 // Match 4098f0 case17's first overlapping row, then invoke its actual bounce path.
 p.at(12)=e.at(8)-r[3]+1;
 game::respond_player_object(p,e,ctx);
 return p.at(0x58)<0;
}
}
