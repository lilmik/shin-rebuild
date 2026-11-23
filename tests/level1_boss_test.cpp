#include "../src/game/level1_boss.hpp"
int main(){shin::game::FirstBoss boss(100,100,130,180);if(boss.payload.at(0x74)!=10)return 1;for(int i=0;i<9;++i){boss.hurt();for(int j=0;j<10;++j)boss.update(0,76);}if(boss.payload.at(0x74)!=1)return 2;boss.hurt();for(int i=0;i<9;++i)if(boss.update(0,76).completed)return 3;if(!boss.update(0,76).completed)return 4;}
