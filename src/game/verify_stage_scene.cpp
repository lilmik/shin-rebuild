#include "stage_scene.hpp"
#include <iostream>
int main(int argc,char**argv){if(argc!=2)return 2;shin::ResourceArchive a(argv[1]);shin::game::StageScene s;for(int i=0;i<7;++i){s.load(a,i);std::cout<<i<<" "<<s.map.width<<"x"<<s.map.height<<" "<<s.placements.size()<<" objects\n";if(s.tiles.width!=640||s.background.width<32||s.camera.pixel_x!=0)return 3;}return 0;}
