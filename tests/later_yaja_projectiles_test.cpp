#include "../src/game/later_yaja_projectiles.hpp"
#include <cassert>
int main(int argc,char**argv){assert(argc==2);shin::AssetStore a(argv[1]);shin::IndexedSurface f(640,480);shin::MapData m{32,32,std::vector<shin::MapCell>(1024)};for(auto&c:m.cells)c.foreground_tile=-1;shin::AppImage blocks{32,32,std::vector<unsigned char>(1024)};shin::game::CollisionWorld w(m,blocks);shin::game::LaterYajaProjectiles shots;shots.spawn(50,60,0,6);shots.tick(0,0,a,f,w);assert(shots.targets[0].at(8)==67);for(int i=0;i<10;++i)shots.tick(0,0,a,f,w);assert(shots.targets[0].at(0x40)==11);}
