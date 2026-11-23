#pragma once
#include "../assets/asset_store.hpp"
#include <array>
#include <algorithm>
namespace shin {
// 43d120/43d400/43d9c0, MAP57: nine-hole mole game.
class Stage2Minigame {
public:
 struct Input{bool up=false,down=false,left=false,right=false,space=false,exit=false;};
 struct Output{bool finished=false;const char*palette=nullptr;int cd=-1;std::vector<std::string>sounds;};
private:
 struct Mole{int x,y,tx,ty,remaining=0,hit=0,hit_tick=0;};
 std::array<Mole,9> moles_{};bool active_=false,first_=true;Input previous_{};
 int column_=1,row_=1,hx_=240,hy_=70,hammer_=0,latch_=0,score_=0,seconds_=60,spawn_ticks_=0,interval_=15;unsigned music_=0,random_=1,step_clock_=0,game_start_=0;bool step_started_=false;
 static void draw(AssetStore&a,IndexedSurface&f,const std::string&n,int x,int y,bool key=true){f.blit(a.image(n),x,y,key);}
 Output finish(Output o){active_=false;o.finished=true;o.palette="shin.pal";return o;}
public:
 bool active()const{return active_;}int score()const{return score_;}int seconds()const{return seconds_;}int selected()const{return row_*3+column_;}int remaining(int i)const{return moles_.at(i).remaining;}
 bool start(int type=0x57){if(active_||type!=0x57)return false;*this=Stage2Minigame{};active_=true;
  constexpr int xs[]={64,240,412,54,240,422,44,240,438};constexpr int ys[]={50,50,50,150,150,150,278,278,278};
  for(int i=0;i<9;++i)moles_[i]={xs[i],ys[i],xs[i],ys[i]};return true;}
 Output tick(unsigned now,Input in,AssetStore&a,IndexedSurface&f){Output o;if(!active_)return o;
  if(first_){first_=false;music_=game_start_=now;o.palette="mole.pal";o.cd=12;}if(now-music_>58000){music_=now;o.cd=12;}
  if(in.exit)return finish(o);
  unsigned elapsed=now-game_start_;seconds_=elapsed>=60000?0:60-int(elapsed/1000);
  if(seconds_==0)return finish(o);
  draw(a,f,"board1.app",0,0,false);
  auto number=[&](int value,const char*prefix,int x,int y){int divisors[]={100,10,1};for(int k=0;k<3;++k){auto n=std::string(prefix)+std::to_string(value/divisors[k]%10)+".app";draw(a,f,n,x,y);x+=a.image(n).width;}};
  number(seconds_,"n1-",70,20);number(score_,"n2-",310,25);
  for(int row=0;row<3;++row){for(int col=0;col<3;++col){auto&m=moles_[row*3+col];auto&im=a.image(m.hit?"mole_dam.app":"mole.app");f.blit(im,m.x,m.y,true,0,0,im.width,im.height);}draw(a,f,"board"+std::to_string(row+2)+".app",0,row==0?174:row==1?281:405);}
  draw(a,f,"hammer"+std::to_string(hammer_+1)+".app",hx_,hy_);
  // Faster mole/hammer motion; the countdown is independent wall-clock time.
  if(step_started_&&now-step_clock_<33)return o;
  step_started_=true;step_clock_=now;
  if(in.left&&!previous_.left&&column_>0)--column_;if(in.right&&!previous_.right&&column_<2)++column_;if(in.up&&!previous_.up&&row_>0)--row_;if(in.down&&!previous_.down&&row_<2)++row_;
  if(in.space&&!latch_)latch_=1;else if(!in.space&&latch_&&hammer_==2)latch_=0;
  if(latch_&&hammer_<2){hy_+=80;++hammer_;auto&m=moles_[selected()];if(hammer_==2&&!m.hit&&m.remaining>0){++score_;m.hit=1;o.sounds.push_back("RESOURCE/punch2.wav");}}
  else if(!latch_&&hammer_>0){hy_-=80;--hammer_;}
  hx_+=(40+column_*200-hx_)/2;hy_+=(-80+row_*150-hy_)/2;
  if(interval_<spawn_ticks_){spawn_ticks_=0;bool available=false;for(auto&m:moles_)available|=!m.hit&&m.remaining==0;if(available){int index;do{random_=random_*214013u+2531011u;index=int((random_>>16)&32767)%9;}while(moles_[index].hit||moles_[index].remaining>0);if(moles_[index].remaining==0){moles_[index].remaining=interval_+8;moles_[index].ty-=50;}}}
  for(auto&m:moles_){if(m.remaining>0&&--m.remaining==0)m.ty+=50;if(m.hit&&++m.hit_tick==10){m.hit=0;m.hit_tick=0;}m.x+=(m.tx-m.x)/2;m.y+=(m.ty-m.y)/2;}
  ++spawn_ticks_;interval_=std::max(14,seconds_/4);previous_=in;return o;
 }
};
}
