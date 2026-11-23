#pragma once
#include <algorithm>
#include <cstdint>
namespace shin {
// Display and UI always run at 60Hz. Only gameplay update cadence changes.
// Deadlines accumulate rather than adding rendering/sleep overhead each frame.
class UpdateClock {
 std::int64_t next_=0,interval_=0;bool started_=false;
public:
 static constexpr std::int64_t display_ns=1000000000LL/60;
 unsigned due(std::int64_t now,std::int64_t interval){
  interval=std::max<std::int64_t>(1,interval);
  if(!started_||interval_!=interval){started_=true;interval_=interval;next_=now;}
  unsigned count=0;
  while(now>=next_&&count<8){next_+=interval_;++count;}
  if(now>=next_)next_=now+interval_; // Never replay an unbounded stall backlog.
  return count;
 }
 void reset(){started_=false;}
};
}
