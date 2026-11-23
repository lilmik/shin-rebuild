#pragma once
#include <array>
#include <cstdint>
#include <functional>
#include <memory>
#include <stdexcept>
#include <vector>
namespace shin {
// Semantic reconstruction of 0044d290/300/370/3a0/440; native pointer layout intentionally differs.
class Scheduler {
public:
 struct Node {
  std::uint8_t kind=0xff, state=1;
  std::function<void(Node&)> update, destroy;
  void* data=nullptr;
  Node *previous=nullptr,*next=nullptr;
  bool linked=false;
 };
 static constexpr std::size_t group_count=10;
private:
 std::array<Node,group_count> heads_{};
 std::array<int,group_count> order_{};
 std::array<int,group_count> enabled_{};
 // Retain detached storage: original destructor can recycle pool memory, but the
 // source loop reads node->next AFTER destructor. This avoids a C++ use-after-free.
 std::vector<std::unique_ptr<Node>> storage_;
 bool ticking_=false;
public:
 Scheduler(){for(std::size_t i=0;i<group_count;++i){order_[i]=int(i);enabled_[i]=1;heads_[i].state=0;}order_[6]=7;order_[7]=6;}
 Scheduler(const Scheduler&)=delete; Scheduler& operator=(const Scheduler&)=delete;
 void set_order(const std::array<int,group_count>& order){for(auto x:order)if(x<0||x>=int(group_count))throw std::out_of_range("group order");order_=order;}
 void enable(std::size_t group,int value=1){enabled_.at(group)=value;}
 Node& create(std::size_t group,std::function<void(Node&)> update={},std::function<void(Node&)> destroy={}){
  Node* tail=&heads_.at(group);while(tail->next)tail=tail->next;
  auto owned=std::make_unique<Node>();auto* node=owned.get();node->update=std::move(update);node->destroy=std::move(destroy);
  node->previous=tail;node->linked=true;tail->next=node;storage_.push_back(std::move(owned));return *node;
 }
 void remove(Node& n){if(!n.linked)return;n.previous->next=n.next;if(n.next)n.next->previous=n.previous;n.linked=false;if(n.destroy)n.destroy(n);}
 void mark_group(std::size_t group){for(Node* n=heads_.at(group).next;n;n=n->next)n->state=2;}
 Node* first(std::size_t group){return heads_.at(group).next;}
 void tick(){
  if(ticking_)throw std::logic_error("recursive scheduler tick");
  ticking_=true;
  try {
   for(auto g:order_)if(enabled_[g]==1)for(Node* n=heads_[g].next;n;n=n->next)if(n->state==0&&n->update)n->update(*n);
   // Cleanup runs for disabled groups too. State 1 becomes runnable only AFTER updates.
   for(auto g:order_)for(Node* n=heads_[g].next;n;n=n->next){if(n->state==1)n->state=0;else if(n->state==2)remove(*n);}
  }catch(...){ticking_=false;throw;}
  ticking_=false;
 }
};
}
