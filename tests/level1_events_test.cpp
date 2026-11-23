#include "../src/game/level1_events.hpp"
int main(){shin::PlayerPayload p;p.at(0xbc)=7;p.at(0xb4)=1;shin::game::Level1Events e;if(e.boss_transition(4,0,8,0,p))return 1;if(!e.boss_transition(4,0,9,0,p))return 2;if(!e.completed||e.current_stage!=1||e.previous_stage!=0||p.at(0xbc)||p.at(0xb4))return 3;if(e.boss_transition(4,0,9,0,p))return 4;}
