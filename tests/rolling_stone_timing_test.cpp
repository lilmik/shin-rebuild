#include "../src/game/later_falling_doll.hpp"
#include <iostream>
int main(int argc,char**argv){if(argc!=2)return 1;shin::AssetStore assets(argv[1]);shin::IndexedSurface frame(853,480);frame.origin_x=106;shin::LaterFallingDoll fast(400,100),normal(400,100);
 for(unsigned now=0;now<=1000;++now)fast.tick(0,0,assets,frame,now);
 for(unsigned now=0;now<=1000;now+=16)normal.tick(0,0,assets,frame,now);
 if(std::abs(fast.e.at(4)-normal.e.at(4))>6)return 2;
 int x=fast.e.at(4);fast.tick(0,0,assets,frame,1001);if(fast.updated_this_tick||!fast.active_this_tick||fast.e.at(4)!=x)return 3;
 std::cout<<"Stone cadence independent of update frequency; contact stays active between steps\n";
}
