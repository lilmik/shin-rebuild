#pragma once
#include <future>
#include <chrono>
#include <utility>
namespace shin {
// Only the worker touches resources while the owner pumps platform events.
// Joining before return (including exceptions) keeps referenced owners alive.
template<class Work,class Pump> auto loading_task(Work work,Pump pump){
 auto pending=std::async(std::launch::async,std::move(work));
 while(pending.wait_for(std::chrono::milliseconds(5))!=std::future_status::ready)pump();
 return pending.get();
}
}
