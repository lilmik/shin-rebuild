#include "../src/game/menu_audio.hpp"
#include <iostream>
// Virtual timestamps test the full 210-second schedule without waiting in real time.
int main(){
 shin::MainMenu menu(0);unsigned triggers=0,last=0;
 for(unsigned now=0;now<=210016;now+=16){
  auto frame=menu.update(now,{bool(now%320==0),false,false});
  unsigned clips=0;for(const auto&event:frame.audio)if(event.rfind("resource/",0)==0)++clips;
  if(clips){if(clips!=3||now<40000||now>200016)return 1;
   if(triggers&&now-last!=10000)return 2;last=now;++triggers;}
 }
 if(triggers!=17)return 3;
 shin::MainMenu reentered(300000);reentered.update(300000,{});
 for(unsigned now:{310001u,320001u,330001u})reentered.update(now,{});
 auto before=reentered.update(340000,{});for(const auto&e:before.audio)if(e.rfind("resource/",0)==0)return 4;
 auto next=reentered.update(340001,{});if(next.audio.size()!=3)return 5;
 std::cout<<"Menu idle: 17 three-clip triggers, 40..200 seconds; navigation does not reset timer\n";
}
