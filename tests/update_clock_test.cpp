#include "../src/core/update_clock.hpp"
#include <cassert>
int main(){
 for(int ms:{8,16,32,100,209}){
  shin::UpdateClock clock;unsigned total=0;
  for(int frame=0;frame<=600;++frame)total+=clock.due(frame*shin::UpdateClock::display_ns,ms*1000000LL);
  auto expected=unsigned((600*shin::UpdateClock::display_ns)/(ms*1000000LL))+1;
  assert(total==expected);
 }
 shin::UpdateClock clock;assert(clock.due(0,16000000)==1);assert(clock.due(1000000000,16000000)==8);
 assert(clock.due(1000000001,16000000)==0);
 assert(clock.due(1000000001,32000000)==1); // Settings apply immediately.
 clock.reset();assert(clock.due(0,16000000)==1);
}
