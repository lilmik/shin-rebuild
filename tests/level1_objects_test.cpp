#include "../src/game/level1_objects.hpp"
#include <iostream>
int main(int argc,char**argv){if(argc!=2)return 1;shin::AssetStore assets(argv[1]);shin::ResourceArchive archive(argv[1]);shin::game::StageScene scene;scene.load(archive,0);shin::Level1Objects objects(assets,scene.placements);if(objects.available_count()!=477)return 2;objects.spawn_visible(13000,1300);for(auto&o:objects.objects())if(!shin::Level1Objects::supports(o.type))return 3;std::cout<<"477 real placement bindings and visible spawn validated\n";}
