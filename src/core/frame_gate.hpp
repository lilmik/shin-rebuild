#pragma once
#include <cstdint>
#include <cstring>
namespace shin {
// Reconstructed from 00433890. Defaults are neutral; caller loads initial globals.
struct FrameGate {
 std::int32_t speed=1,delta=0,effective=1,count=0;
 bool present_flag=false;
 static std::int32_t wrap(std::uint32_t x){std::int32_t out;std::memcpy(&out,&x,4);return out;}
 template<class Update,class HideCursor,class Present>
 void tick(Update update,HideCursor hide,Present present){
 effective=speed;if(speed!=1)effective=wrap(std::uint32_t(speed)-std::uint32_t(delta));
 update();present_flag=count==0;count=wrap(std::uint32_t(count)+1u);
 if(effective<=count)count=0;hide();if(present_flag)present();
 }
};
}
