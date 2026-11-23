#include "../src/game/stage_music.hpp"
#include <cassert>
int main(){for(unsigned i=0;i<7;++i){shin::StageMusic music(i);assert(music.track()==i+4);assert(music.flying_stage()==(i==2));assert(music.update(100));assert(!music.update(100+music.period()));assert(music.update(101+music.period()));}bool bad=false;try{shin::StageMusic invalid(7);}catch(const std::out_of_range&){bad=true;}assert(bad);}
