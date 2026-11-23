#pragma once
#include <vector>
namespace shin {
// MAP74 4142b0 ->434110/4341d0; scripted final-pair encounter, not an invented health win.
class FinalBossIntro {
 unsigned began_=0;int phase_=0;bool active_=false;
public:
 enum Target{player,robot,nurse,camera};struct Helper{Target target;unsigned address;};struct Spawn{unsigned constructor;int type,x,y;};
 struct Flag{Target target;unsigned offset;int value;};std::vector<Flag>flags;
 std::vector<Helper>helpers;std::vector<Spawn>spawns;std::vector<unsigned>sounds;unsigned music_request=0;
 bool finished=false;
 bool active()const{return active_;}
 bool begin(unsigned now){if(active_||finished)return false;began_=now;phase_=0;active_=true;helpers={{player,0x413200},{camera,0x427eb0}};sounds={0x77dc8c};return true;}
 void tick(unsigned now,int px,int py){if(!active_)return;unsigned t=now-began_;
 if(phase_==0&&t>3690){helpers.push_back({player,0x413260});spawns.push_back({0x442f10,0x3b,px+360,py-200});helpers.push_back({robot,0x443860});spawns.push_back({0x42a4c0,0x29,px+320,py-200});helpers.push_back({nurse,0x42af60});++phase_;}
 if(phase_==1&&t>4690){helpers.push_back({robot,0x4438c0});++phase_;}
 if(phase_==2&&t>5690){helpers.push_back({nurse,0x42afc0});++phase_;}
 if(phase_==3&&t>6690){flags.push_back({nurse,0x60,1});++phase_;} // original nurse facing flag60=1
 if(phase_==4&&t>11690){music_request=15;flags.push_back({nurse,0x60,1});helpers.push_back({robot,0x4438f0});++phase_;}
 if(phase_==5&&t>16690){helpers.push_back({player,0x413290});helpers.push_back({robot,0x443920});helpers.push_back({nurse,0x42afc0});++phase_;}
 if(phase_==6&&t>17690){helpers.push_back({player,0x4132c0});++phase_;}
 if(phase_==7&&t>27690){helpers.push_back({player,0x413290});spawns.push_back({0x41f150,0,px+50,py-500});helpers.push_back({nurse,0x42aff0});sounds.push_back(0x77dc94);sounds.push_back(0x77dc84);++phase_;}
 if(phase_==8&&t>37690){helpers.push_back({player,0x4132f0});helpers.push_back({robot,0x443950});helpers.push_back({nurse,0x42b020});active_=false;finished=true;++phase_;}
 }
};
}
