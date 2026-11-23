#pragma once
#include <algorithm>
#include <cstdlib>
#include "player_animation.hpp"
#include "player_object_response.hpp"
#include "collision_world.hpp"
namespace shin {
class BoosterMissiles {
public:
 struct Shot{game::EntityPayload e;int delay,frame=0,ticks=0;bool right;};std::vector<Shot>shots;
 void spawn(int x,int y,int vx,int delay){Shot s{};s.e.kind=0x46;s.e.at(4)=x;s.e.at(8)=y;s.e.at(0x50)=vx;s.delay=delay;s.right=vx>=0;shots.push_back(s);}
 void tick(int cx,int cy,int py,AssetStore&a,IndexedSurface&f,const game::CollisionWorld&w){static const char*names[]={"high5ba.raw","high5ca.raw","high5da.raw"};for(auto&s:shots){auto&e=s.e;if(e.state==2)continue;if((e.at(4)<cx+std::min(-640,-f.origin_x-128)||e.at(4)>cx+std::max(1280,f.width-f.origin_x+128))||e.at(8)>cy-f.origin_y+f.height){e.state=2;continue;}e.at(0x48)=e.at(4);e.at(0x4c)=e.at(8);f.blit(a.image(names[s.frame]),e.at(4)-cx,e.at(8)-cy,true,0,0,-1,-1,s.right);e.at(4)+=e.at(0x50);e.at(8)+=e.at(0x54);if(++s.ticks>4){s.ticks=0;s.frame=(s.frame+1)%3;}if(s.delay>0){--s.delay;e.at(0x54)=e.at(8)<=py+20?4:-4;e.at(0x50)=0;}else{e.at(0x50)+=s.right?1:-1;e.at(0x54)=0;}if(w.body({0,0,10,10},e.at(4),e.at(8)))e.state=2;}shots.erase(std::remove_if(shots.begin(),shots.end(),[](auto&s){return s.e.state==2;}),shots.end());}
};
}

