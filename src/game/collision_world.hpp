#pragma once
#include <array>
#include <vector>
#include <functional>
#include <list>
#include "../assets/resource_archive.hpp"
namespace shin::game {
// Original rectangle offsets [left,top,right,bottom].
using CollisionRect=std::array<int,4>;
class CollisionWorld {
 const MapData* map_;const AppImage* blocks_;
public:
 CollisionWorld(const MapData&map,const AppImage&blocks):map_(&map),blocks_(&blocks){}
 int raw_sample(int world_x,int world_y)const;
 int sample(int world_x,int world_y)const;
 int body(const CollisionRect&,int x,int y)const; // 0x431130
 int left(const CollisionRect&,int x,int y)const; // 0x431f90
 int right(const CollisionRect&,int x,int y)const; // 0x4320c0
 int ground(const CollisionRect&,int x,int y)const; // 0x431d60
};
bool intersects(const CollisionRect&,int,int,const CollisionRect&,int,int);
struct CollisionBody {
 void* owner=nullptr; const unsigned char* state=nullptr;const CollisionRect* rectangle=nullptr;const int*x=nullptr;const int*y=nullptr;
 std::function<void(int)> on_world;
 std::function<void(void*)> on_object;
};
class CollisionDispatcher {
 std::list<CollisionBody> bodies_;
public:
 CollisionBody* add(CollisionBody b){bodies_.push_back(std::move(b));return &bodies_.back();}
 void remove(CollisionBody*p){bodies_.remove_if([&](const auto&b){return &b==p;});}
 void world(const CollisionWorld&);
 void objects();
};
// Two separate registries exist in original (0x431050 / 0x434b40).
// Fields retain positional semantics pending consumers' complete recovery.
struct CollisionRegistration {int field8,field10,field14,field18,fieldC;};
class CollisionRegistry {
 std::vector<CollisionRegistration> nodes_;
public:
 void add(int p1,int p2,int p3,int p4,int p5){nodes_.push_back({p1,p2,p3,p4,p5});}
 void clear(){nodes_.clear();}const auto& entries()const{return nodes_;}
};
}
