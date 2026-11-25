#pragma once
#include "stage2_minigame.hpp"
namespace shin {
namespace extra_minigame_data {
#include "extra_minigame_data.inc"
}
// Card:41c810/41c920; sliding puzzle:4245a0/4246a0/424320.
class ExtraMinigames {
public:
 using Input=Stage2Minigame::Input;using Output=Stage2Minigame::Output;
private:
 int type_=0,selected_=0,first_card_=-1,second_card_=-1,lives_=4,empty_=8;bool active_=false,first_=true,feedback_=false,solved_=false;unsigned started_=0,reveal_=0,finished_at_=0,random_=1;Input previous_{};bool action_ready_=false;
 std::array<int,18> cards_{};std::array<bool,18> present_{};std::array<int,9> board_{};
 unsigned next_random(){random_=random_*214013u+2531011u;return (random_>>16)&32767;}
 void move(int d){int n=empty_;if(d==0&&empty_>=3)n-=3;if(d==1&&empty_%3<2)++n;if(d==2&&empty_<6)n+=3;if(d==3&&empty_%3>0)--n;std::swap(board_[empty_],board_[n]);empty_=n;}
 Output finish(Output o){active_=false;o.finished=true;o.palette="shin.pal";return o;}
public:
 bool active()const{return active_;}int selected()const{return selected_;}int lives()const{return lives_;}int face(int n)const{return cards_.at(n);}bool present(int n)const{return present_.at(n);}const auto&board()const{return board_;}
 bool start(int type){if(active_||(type!=1&&type!=4))return false;*this=ExtraMinigames{};type_=type;active_=true;present_.fill(true);for(int i=0;i<18;++i)cards_[i]=i/2;for(int i=0;i<18;++i)std::swap(cards_[i],cards_[next_random()%18]);for(int i=0;i<9;++i)board_[i]=extra_minigame_data::puzzle_goal[i];return true;}
 Output tick(unsigned now,Input in,AssetStore&a,IndexedSurface&f){Output o;if(!active_)return o;if(first_){first_=false;started_=now;o.palette="shin.pal";o.cd=0;}if(in.exit)return finish(o);if(!in.space)action_ready_=true;if(!action_ready_)in.space=false;
 auto draw=[&](const std::string&n,int x,int y,bool key=true){f.blit(a.image(n),x,y,key);};
 if(type_==1){
  if(feedback_&&now-reveal_>800){if(cards_[first_card_]==cards_[second_card_])present_[first_card_]=present_[second_card_]=false;else --lives_;first_card_=second_card_=-1;feedback_=false;}
  draw("cardbg.app",0,0,false);bool preview=now-started_<2300;
  for(int i=0;i<18;++i)if(present_[i]){bool open=preview||i==first_card_||i==second_card_;auto n=open?"card0"+std::to_string(cards_[i]+1)+".app":"card00.app";draw(n,extra_minigame_data::card_xy[i*2],extra_minigame_data::card_xy[i*2+1]);}
  for(int i=0;i<lives_;++i)draw("cardlife.app",extra_minigame_data::card_lives_xy[i*2],extra_minigame_data::card_lives_xy[i*2+1]);draw("cardarw.app",extra_minigame_data::card_xy[selected_*2]+32,extra_minigame_data::card_xy[selected_*2+1]+45);
  bool remaining=false;for(bool b:present_)remaining|=b;if(!remaining||lives_==0)return finish(o);
  if(!feedback_){if(in.left&&!previous_.left&&selected_>0)--selected_;if(in.right&&!previous_.right&&selected_<17)++selected_;if(in.up&&!previous_.up&&selected_>=6)selected_-=6;if(in.down&&!previous_.down&&selected_<12)selected_+=6;
   if(in.space&&!previous_.space&&present_[selected_]){if(first_card_<0)first_card_=selected_;else if(selected_!=first_card_){second_card_=selected_;feedback_=true;reveal_=now;o.sounds.push_back(cards_[first_card_]==cards_[second_card_]?"RESOURCE/eat.wav":"RESOURCE/fail.wav");}}}
 }else{
  if(now-started_<5000)move(int(next_random()%4));
  else if(!solved_){int d=-1;if(in.up&&!previous_.up)d=2;else if(in.down&&!previous_.down)d=0;else if(in.left&&!previous_.left)d=1;else if(in.right&&!previous_.right)d=3;if(d>=0){move(d);o.sounds.push_back("RESOURCE/puzmove.wav");}
   solved_=true;for(int i=0;i<9;++i)solved_&=board_[i]==extra_minigame_data::puzzle_goal[i];if(solved_){finished_at_=now;o.sounds.push_back("RESOURCE/puzopen.wav");}}
  draw("pbg.app",0,0,false);draw("p9.app",solved_?319:459,308);for(int i=0;i<9;++i)if(board_[i]>=0)draw("p"+std::to_string(board_[i]+1)+".app",extra_minigame_data::puzzle_xy[i*2],extra_minigame_data::puzzle_xy[i*2+1]);
  if(solved_&&now-finished_at_>2500)return finish(o);
 }
 previous_=in;return o;
 }
};
}
