#pragma once
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <mmsystem.h>
#include <chrono>
#include <thread>

namespace shin::platform {
// Per-process timer lifetime; high resolution waitable timers avoid the
// 15.6ms rounding that turned a 16ms frame into a roughly 31ms frame.
class FramePacer {
 HANDLE timer_=nullptr;bool period_=false;
public:
 using Clock=std::chrono::steady_clock;
 FramePacer(){
  timer_=CreateWaitableTimerExW(nullptr,nullptr,0x00000002,TIMER_ALL_ACCESS);
  if(!timer_){period_=timeBeginPeriod(1)==TIMERR_NOERROR;timer_=CreateWaitableTimerW(nullptr,FALSE,nullptr);}
 }
 ~FramePacer(){if(timer_)CloseHandle(timer_);if(period_)timeEndPeriod(1);}
 FramePacer(const FramePacer&)=delete;FramePacer&operator=(const FramePacer&)=delete;
 void wait_until(Clock::time_point deadline){
  auto remaining=std::chrono::duration_cast<std::chrono::nanoseconds>(deadline-Clock::now()).count();
  if(remaining<=0)return;
  if(timer_){LARGE_INTEGER due;due.QuadPart=-((remaining+99)/100);if(SetWaitableTimer(timer_,&due,0,nullptr,nullptr,FALSE)){WaitForSingleObject(timer_,INFINITE);return;}}
  std::this_thread::sleep_until(deadline);
 }
};
}
