#include "platform/touch_controls.hpp"
#include <cassert>
int main(){shin::TouchControls t;t.finger(1,.05f,.75f,true);t.finger(2,.9f,.8f,true);assert(t.held(37)&&t.held(32));t.finger(1,.2f,.75f,true);assert(!t.held(37)&&t.held(39)&&t.held(32));t.finger(2,0,0,false);assert(!t.held(32)&&t.held(39));t.clear();assert(!t.held(39));t.finger(1,.12f,.6f,true);t.finger(2,.78f,.6f,true);assert(t.held(38));t.finger(1,0,0,false);assert(t.held(38));auto p=t.pixels();assert(p.size()==960*540);assert(p.front()==0);t.clear();assert(!t.held(38));}
