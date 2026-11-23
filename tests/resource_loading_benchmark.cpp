#include "../src/game/level_runtime.hpp"
#include <chrono>
#include <iostream>
int main(int argc,char**argv){if(argc!=2)return 1;auto start=std::chrono::steady_clock::now();shin::AssetStore a(argv[1]);auto loaded=std::chrono::steady_clock::now();std::cout<<"archive_initial_ms="<<std::chrono::duration<double,std::milli>(loaded-start).count()<<"\n";for(int stage=0;stage<7;++stage){auto t=std::chrono::steady_clock::now();shin::LevelRuntime l(a,argv[1],stage);std::cout<<"stage="<<stage<<" construct_ms="<<std::chrono::duration<double,std::milli>(std::chrono::steady_clock::now()-t).count()<<"\n";}}
