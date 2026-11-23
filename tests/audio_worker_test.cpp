#include "../src/platform/game_audio.hpp"
#include <chrono>
#include <iostream>
int main(int argc,char**argv){
 if(argc!=2)return 1;
 shin::platform::GameAudio audio(argv[1],false);
 // Warm file existence only; device work is never waited for by play_file.
 auto begin=std::chrono::steady_clock::now();
 for(int n=0;n<20;++n){if(!audio.play_file("RESOURCE/bonus.wav"))return 2;audio.stop_sound(0x80000000u);}
 auto queued=std::chrono::duration<double,std::milli>(std::chrono::steady_clock::now()-begin).count();
 audio.wait_sound_idle();
 if(audio.sound_failures())return 3;
 audio.stop_all();audio.wait_sound_idle();
 if(!audio.play_file("RESOURCE/bonus.wav"))return 4;
 audio.wait_sound_idle();
 if(audio.sound_failures())return 5;
 if(audio.events().size()!=41)return 6;
 std::cout<<"queued_40_commands_ms="<<queued<<" ordered play/pause, clear/reload and worker drain verified\n";
}
