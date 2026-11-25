#include "../src/game/stage2_minigame.hpp"
#include <iostream>
int main(int argc,char**argv){if(argc<2)return 1;shin::AssetStore assets(argv[1]);shin::IndexedSurface frame(640,480);shin::Stage2Minigame game;game.start();auto first=game.tick(0,{},assets,frame);if(first.cd!=12||!first.palette)return 2;
 // A press and release entirely between two motion steps must still swing.
 game.tick(5,{false,false,false,false,true},assets,frame);game.tick(10,{},assets,frame);
 game.tick(34,{},assets,frame);if(game.hammer_pose()!=1)return 3;
 game.tick(68,{},assets,frame);if(game.hammer_pose()!=2)return 4;
 game.tick(102,{},assets,frame);game.tick(136,{},assets,frame);if(game.hammer_pose()!=0)return 5;
 game.start(); // An already active game must not reset its timer/state.
 unsigned now=136;int target=-1;
 for(int step=0;step<60&&target<0;++step){game.tick(now+=34,{},assets,frame);for(int i=0;i<9;++i)if(game.remaining(i)>0)target=i;}
 if(target<0)return 6;int row=target/3,col=target%3;
 if(row!=1){game.tick(now+=34,{row==0,row==2},assets,frame);game.tick(now+=34,{},assets,frame);}
 if(col!=1){game.tick(now+=34,{false,false,col==0,col==2},assets,frame);game.tick(now+=34,{},assets,frame);}
 if(game.selected()!=target)return 7;
 game.tick(now+5,{false,false,false,false,true},assets,frame);game.tick(now+10,{},assets,frame);
 game.tick(now+=34,{},assets,frame);auto hit=game.tick(now+=34,{},assets,frame);
 if(game.score()!=1||hit.sounds.size()!=1)return 8;
 auto finished=game.tick(60000,{},assets,frame);if(!finished.finished||game.active()||game.seconds()!=0)return 9;
 shin::Stage2Minigame nav;nav.start();nav.tick(0,{},assets,frame);nav.tick(5,{false,false,false,true},assets,frame);nav.tick(10,{},assets,frame);nav.tick(34,{},assets,frame);if(nav.selected()!=5)return 10;
 std::cout<<"Short strike retained across timing gate, hammer swing/hit and real-time countdown passed\n";
}
