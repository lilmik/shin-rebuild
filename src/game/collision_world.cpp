#include "collision_world.hpp"
#include <stdexcept>
#include <algorithm>
namespace shin::game {
int CollisionWorld::raw_sample(int x,int y)const{
 // C++ / and % preserve original signed truncation toward zero.
 if(x<0||y<0)return 0;int tx=x/32,ty=y/32;if(tx<0||ty<0||tx>=int(map_->width)||ty>=int(map_->height))return 0;
 int tile=map_->cells[std::size_t(ty)*map_->width+tx].foreground_tile;if(tile<0)return 0;
 int bx=(tile%20)*32+x%32,by=(tile/20)*32+y%32;
 if(bx<0||by<0||bx>=int(blocks_->width)||by>=int(blocks_->height))return 0;
 int v=blocks_->indices[std::size_t(by)*blocks_->width+bx];return v;
}
int CollisionWorld::sample(int x,int y)const{int v=raw_sample(x,y);return v>=1&&v<=4?v:0;}
int CollisionWorld::ground(const CollisionRect&r,int x,int y)const{int left=r[0]+x,right=r[2]+x,bottom=r[3]+y;for(int p=left;p<right;p+=8){if(p/32<0||bottom/32<0)break;int v=sample(p,bottom);if(v)return v;}return sample(right,bottom);}

int CollisionWorld::left(const CollisionRect&r,int x,int y)const{for(int py=r[1]+y;py<r[3]+y;py+=8){if((r[0]+x)/32<0||py/32<0)return 0;int v=sample(r[0]+x,py);if(v)return v;}return 0;}
int CollisionWorld::right(const CollisionRect&r,int x,int y)const{for(int py=r[1]+y;py<r[3]+y;py+=8){if((r[2]+x)/32<0||py/32<0)return 0;int v=sample(r[2]+x,py);if(v)return v;}return 0;}
int CollisionWorld::body(const CollisionRect&r,int x,int y)const{
 const int l=r[0]+x,t=r[1]+y,rr=r[2]+x,b=r[3]+y;
 auto horizontal=[&](int yy,int type){for(int xx=l;xx<rr;xx+=8){if(xx/32<0||yy/32<0)break;if(raw_sample(xx,yy)==type)return true;}return raw_sample(rr,yy)==type;};
 auto vertical=[&](int xx,int type){for(int yy=t;yy<b;yy+=8){if(xx/32<0||yy/32<0)break;if(raw_sample(xx,yy)==type)return true;}return false;};
 for(int type=1;type<=2;++type)if(horizontal(t,type)||horizontal(b,type)||vertical(l,type)||vertical(rr,type))return type;
 for(int type=3;type<=5;++type)if(horizontal(b,type))return type;return 0;
}
bool intersects(const CollisionRect&a,int ax,int ay,const CollisionRect&b,int bx,int by){return std::max(a[0]+ax,b[0]+bx)<std::min(a[2]+ax,b[2]+bx)&&std::max(a[1]+ay,b[1]+by)<std::min(a[3]+ay,b[3]+by);}
void CollisionDispatcher::world(const CollisionWorld&w){for(auto&b:bodies_)if(b.state&&*b.state==0&&b.rectangle&&b.x&&b.y){int kind=w.body(*b.rectangle,*b.x,*b.y);if(kind&&b.on_world)b.on_world(kind);}}
void CollisionDispatcher::objects(){for(auto a=bodies_.begin();a!=bodies_.end();++a){if(!a->state||*a->state!=0)continue;auto b=a;for(++b;b!=bodies_.end();++b){if(!b->state||*b->state!=0)continue;if(a->rectangle&&a->x&&a->y&&b->rectangle&&b->x&&b->y&&intersects(*a->rectangle,*a->x,*a->y,*b->rectangle,*b->x,*b->y)){if(a->on_object)a->on_object(b->owner);if(b->on_object)b->on_object(a->owner);}}}}
}

