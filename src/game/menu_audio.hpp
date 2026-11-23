#pragma once
#include "menu.hpp"
namespace shin {
// Both platform hosts must consume every menu command, not just draw calls.
template<class PlayMusic,class StopMusic,class PlaySound>
void dispatch_menu_audio(const MenuFrame&frame,PlayMusic play,StopMusic stop,PlaySound sound){
 for(const auto&command:frame.audio){
  if(command=="play_cdaudio_2_to_3")play(2,false);
  else if(command=="restart_cdaudio_2_to_3")play(2,true);
  else if(command=="stop_cdaudio")stop();
  else if(command.rfind("resource/",0)==0||command.rfind("RESOURCE/",0)==0)sound(command);
 }
}
template<class StopSound> void stop_menu_idle_audio(const MenuFrame&frame,StopSound stop){
 if(frame.action!=MenuAction::none)for(const auto*file:{"resource/_1.wav","resource/_2.wav","resource/_3.wav"})stop(file);
}
}
