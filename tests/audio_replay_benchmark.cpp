#include "../src/platform/native_backend.hpp"
#include <algorithm>
#include <chrono>
#include <filesystem>
#include <iostream>
int main(int argc,char**argv){
 if(argc!=2)return 1;
 shin::platform::WavSound sound;if(!sound.load(std::filesystem::path(argv[1]).wstring())||!sound.play())return 2;
 sound.pause();std::vector<double> times;
 for(int i=0;i<50;++i){auto start=std::chrono::steady_clock::now();if(!sound.play(i%2!=0))return 3;sound.pause();times.push_back(std::chrono::duration<double,std::milli>(std::chrono::steady_clock::now()-start).count());}
 std::sort(times.begin(),times.end());std::cout<<"replay_and_pause_count=50 p50_ms="<<times[25]<<" p95_ms="<<times[47]<<" max_ms="<<times.back()<<"\n";
}
