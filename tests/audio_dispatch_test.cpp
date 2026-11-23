#include "../src/game/menu_audio.hpp"
#include "../src/platform/game_audio.hpp"
#include <iostream>
int main(int argc,char**argv){if(argc!=2)return 1;shin::MainMenu menu(0);menu.update(0,{});shin::MenuFrame frame;
 for(unsigned step=1;step<=4;++step)frame=menu.update(step*10000+1,{});
 if(frame.audio.size()!=3)return 2;
 shin::platform::GameAudio audio(argv[1],false);shin::dispatch_menu_audio(frame,[](unsigned,bool){},[]{},[&](const std::string&name){audio.play_wav(name);});audio.wait_sound_idle();if(audio.sound_failures()||audio.journal.size()!=3)return 3;audio.stop_all();audio.wait_sound_idle();
 shin::platform::GameAudio silent(argv[1],true);if(!silent.play_file("RESOURCE/MENU.WAV",0,true))return 4;unsigned original=silent.events().back().id;
 for(int i=0;i<4200;++i)if(!silent.play_file("RESOURCE/bonus.wav"))return 5;
 silent.service_music();if(silent.events().size()>4096)return 6;
 silent.stop("resource\\menu.flac");if(silent.events().back().action!="stop_sound"||silent.events().back().id!=original)return 7;
 unsigned track=0;bool stopped=false;shin::MenuFrame commands;commands.audio={"play_cdaudio_2_to_3","stop_cdaudio"};shin::dispatch_menu_audio(commands,[&](unsigned t,bool){track=t;},[&]{stopped=true;},[](const std::string&){});if(track!=2||!stopped)return 8;
 auto replay=menu.update(165001,{});bool restarted=false;shin::dispatch_menu_audio(replay,[&](unsigned t,bool restart){restarted=t==2&&restart;},[]{},[](const std::string&){});if(!restarted)return 9;
 int stopped_clips=0;frame.action=shin::MenuAction::start_game;shin::stop_menu_idle_audio(frame,[&](const std::string&){++stopped_clips;});if(stopped_clips!=3)return 10;
 std::cout<<"Menu idle clips decoded and submitted; menu music commands and history-independent case-insensitive stop passed\n";
}
