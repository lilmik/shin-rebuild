#include "../src/game/level_runtime.hpp"
#include <iostream>
int main(int argc,char**argv){if(argc!=2)return 1;shin::AssetStore assets(argv[1]);shin::LevelRuntime level(assets,argv[1],0);shin::IndexedSurface frame(640,480);shin::InputState input;for(int i=0;i<20;++i)level.tick(input,frame);if(level.player.at(12)!=199)return 2;std::cout<<"real map/player/terrain/render integrated for 20 updates\n";}
