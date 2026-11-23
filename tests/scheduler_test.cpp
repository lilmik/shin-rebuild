#include "../src/core/scheduler.hpp"
#include <cassert>
#include <iostream>
int main(){
 shin::Scheduler s; s.enable(0);s.enable(1);int runs=0,deaths=0;
 auto& a=s.create(0,[&](auto&){++runs;},[&](auto&){++deaths;});
 s.tick();assert(runs==0&&a.state==0);s.tick();assert(runs==1);
 a.state=2;s.tick();assert(deaths==1&&!s.first(0));
 std::vector<int> order;auto& b=s.create(0,[&](auto&){order.push_back(0);});
 auto& c=s.create(1,[&](auto&){order.push_back(1);});b.state=c.state=0;
 std::array<int,10> p{1,0,2,3,4,5,6,7,8,9};s.set_order(p);s.tick();assert((order==std::vector<int>{1,0}));
 s.enable(0,2);s.mark_group(0);s.tick();assert(!s.first(0)); // enabled must equal 1, cleanup independent
 int newborn=0;bool made=false;c.update=[&](auto&){if(!made){s.create(1,[&](auto&){++newborn;});made=true;}};
 s.tick();assert(newborn==0);s.tick();assert(newborn==1);
 std::cout<<"scheduler lifecycle/order/newborn/disabled cleanup passed\n";
}
