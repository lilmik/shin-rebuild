#include "../src/core/frame_gate.hpp"
#include <cassert>
#include <iostream>
int main(){shin::FrameGate f;int u=0,h=0,p=0;auto tick=[&]{f.tick([&]{++u;},[&]{++h;},[&]{++p;});};
tick();tick();assert(u==2&&h==2&&p==2);f.speed=3;for(int i=0;i<6;++i)tick();assert(p==4&&f.count==0);
f.delta=2;tick();assert(p==5);f.speed=1;f.delta=100;tick();assert(f.effective==1);f.speed=3;f.delta=5;tick();assert(f.effective==-2&&f.count==0);
f.count=INT32_MAX;f.delta=0;tick();assert(f.count==INT32_MIN);std::cout<<"frame gate cadence and signed wrap passed\n";}
