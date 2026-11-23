#include "../src/game/level_runtime.hpp"
#include <iostream>
int main(int argc,char**argv){
 if(argc!=2)return 1;shin::AssetStore a(argv[1]);shin::IndexedSurface f(640,480);shin::InputState keys;
 shin::LevelRuntime level(a,argv[1],0);level.original_sound=[](unsigned){return 1;};level.play_sound_file=[](const std::string&){};
 unsigned kinds[]={0x55,0x53,0x57,0x54,0x58},fields[]={0xc8,0xcc,0xd0,0xd8,0xdc};
 for(int i=0;i<5;++i){shin::game::EntityPayload item;item.kind=kinds[i];auto before=level.player.at(fields[i]);shin::game::respond_player_object(level.player,item,level);if(level.player.at(fields[i])!=before+1||item.at(0x3c)!=1||item.at(0x38)!=-15)return 6;}
 {shin::game::EntityPayload spring;spring.kind=0x40;spring.at(4)=0;spring.at(8)=65;spring.at(0x14)=56;spring.at(0x18)=10;level.player.at(0x58)=4;
  shin::game::respond_player_object(level.player,spring,level);if(level.player.at(0x58)!=-35||spring.at(0x50)!=1)return 7;}
 {shin::Projectile bomb;level.call(0x42b300,bomb,{0x42,0x42b3d0,0x42b460,8,100,100});if(level.explosion_count()!=1)return 8;
  for(int i=0;i<12;++i)level.tick(keys,f);if(level.explosion_count()!=0)return 9;}
 auto&p=level.player;p.at(0x8c)=2;p.at(8)=1000;p.at(12)=3000;
 p.at(0x5c)=4;p.at(0x1c)=1;p.at(0x20)=4;p.at(0xb4)=1;
 level.tick(keys,f);if(p.at(0x8c)!=1||p.at(8)!=0||p.at(12)!=0)return 2;
 level.tick(keys,f);if(level.camera_x()!=0||level.camera_y()!=0||p.at(0xb4)!=0)return 3;
 p.at(0x5c)=4;p.at(0x1c)=1;p.at(0x20)=4;
 level.tick(keys,f);if(!level.game_over()||p.at(0x8c)!=0)return 4;
 level.tick(keys,f);keys.message(0x100,32);level.tick(keys,f);keys.message(0x101,32);level.tick(keys,f);
 if(!level.wants_menu())return 5;
 std::cout<<"hazard respawn preserves remaining life, resets map/camera, and final life reaches game over/menu\n";
}
