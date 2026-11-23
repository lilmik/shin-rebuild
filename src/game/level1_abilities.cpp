#include "game_rules.hpp"
#include "level1_abilities.hpp"
#include <cstdlib>
namespace shin {
#include "ability_assets.inc"
void Level1Abilities::change(int state,int count,int delay){state_=state;count_=count;delay_=delay;frame_=ticks_=0;}
bool Level1Abilities::activate(int choice,PlayerPayload&p,int cx,int cy){
 if(choice!=2&&choice!=3&&choice!=5&&choice!=6&&choice!=7)throw std::runtime_error("unsupported ability");
 if(active_)return false;
 const unsigned field=0xc0+4*choice;if(p.at(field)<=0)return false;
 double_=choice==2&&p.at(field)>=2;p.at(field)-=double_?2:1;
 player_=&p;choice_=choice;completed_=target_found_=false;screen_x_=screen_y_=0;
 x_=cx+(choice==3?0:choice==6?10:choice==7?400:100);y_=cy+(choice==3?-100:choice>=6?50:0);
 change(choice==2&&!double_?2:0,choice==7?4:1,choice==7?10:500);active_=true;
 const char*sound=choice==7?"RESOURCE/item-3.wav":choice==3?"RESOURCE/father.wav":choice==2||choice==5?"RESOURCE/item-1.wav":nullptr;
 if(sound)sounds.push_back(sound);return true;
}
void Level1Abilities::tick(int cx,int cy,IndexedSurface&target){
 // 413b60: the 0x5c objects are visual arcs without registered collision callbacks.
 for(auto it=bombs_.begin();it!=bombs_.end();){
  bool remove=std::abs(it->x-cx-320)>960||(it->ticks==4&&it->frame==3);
  target.blit(assets_.image(ability_names5[ability_indices5[it->frame]]),it->x-cx,it->y-cy);
  if(it->vy<11)++it->vy;it->x+=it->vx;it->y+=it->vy;
  if(++it->ticks>5){it->ticks=0;if(++it->frame>=4)it->frame=0;}
  if(remove)it=bombs_.erase(it);else ++it;
 }
 if(!active_)return;
 auto nearest=[&](){if(!nearest_target)throw std::runtime_error("ability nearest-target host is unbound");return nearest_target(x_,y_,target_x_,target_y_);};
 auto clear=[&](){if(!clear_enemies)throw std::runtime_error("ability clear-enemies host is unbound");clear_enemies();};
 int vx=0,vy=0,resource=0;const char**names=nullptr;
 if(choice_==6){
  if(state_==0){if(screen_x_>100)change(1,1,100);vx=6;}
  else if(state_==1){if(ticks_==4){change(2,20,1);sounds.push_back("RESOURCE/itemsho.wav");}}
  else if(state_==2&&ticks_==0&&frame_==19)active_=false;
  names=ability_names0;resource=ability_indices0[(state_==0?0:state_==1?1:2)+frame_];
 }else if(choice_==7){if(screen_x_<0)active_=false;vx=-6;names=ability_names1;resource=ability_indices1[frame_];
 }else{
  if(std::abs(x_-cx-320)>960)active_=false;
  if(choice_==2){
   if(state_==0||state_==2){
    if(!completed_&&screen_y_>100){if(double_)change(1,1,50);else{change(3,4,10);if(state_==3)target_found_=nearest();}}
    else if(completed_&&screen_y_>400)active_=false;vy=4;
   }else if(state_==1){if(ticks_==49){change(0,1,500);clear();completed_=true;}}
   else if(state_==3){if((double_&&ticks_==49)||(!double_&&ticks_==9&&frame_==3)){change(0,1,500);completed_=true;}
    if(!double_&&target_found_){x_+=(target_x_-x_)/2;y_+=(target_y_-y_)/2;}}
   names=ability_names2;resource=ability_indices2[(state_==0?0:state_==1?1:state_==2?2:3)+frame_];
  }else if(choice_==3){
   if(state_==0||state_==2){if(!completed_&&screen_y_>100)change(1,9,10);else if(completed_&&screen_y_>400)active_=false;vy=4;}
   else if(state_==1&&frame_==8&&ticks_==9){clear();change(2,1,500);completed_=true;}
   names=ability_names3;resource=ability_indices3[(state_==0?0:state_==1?1:10)+frame_];
  }else if(choice_==5){
   if(state_==0){if(!completed_&&screen_y_>100)change(1,3,10);else if(completed_&&screen_y_>400)active_=false;vy=4;}
   else if(state_==1&&ticks_==9){
    if(frame_==2)change(4,1,20);
    if(ticks_==9&&frame_==1){if(!nearest()){active_=false;return;}bombs_.push_back({x_,y_,(target_x_-x_)/20,-15});}
   }else if(state_==3){
    if(ticks_==9&&frame_==3){change(0,1,500);completed_=true;}
    if(nearest()){x_+=(target_x_-x_)/2;y_+=(target_y_-y_)/2;}
   }else if(state_==4&&ticks_==19)change(3,4,10);
   names=ability_names4;resource=ability_indices4[(state_==0?0:state_==1?2:state_==3?6:10)+frame_];
  }
 }
 screen_x_=x_-cx;screen_y_=y_-cy;
 target.blit(assets_.image(names[resource]),screen_x_,screen_y_);
 bool damaging=choice_==7||(choice_==6&&state_==2)||((choice_==2||choice_==5)&&state_==3);
 // The original collision pass follows integration, although rendering precedes it.
 if(choice_==3)x_+=(player_->at(8)-x_-50)/2;
 x_+=vx;y_+=vy;
 if(++ticks_>delay_){ticks_=0;if(++frame_>=count_)frame_=0;}
 if(damaging){if(!damage)throw std::runtime_error("ability damage host is unbound");
  int kills=damage(choice_==6?game::CollisionRect{-200,-200,800,400}:choice_==7?game::CollisionRect{0,0,300,300}:game::CollisionRect{33,62,100,217},x_,y_);
  for(int i=0;i<kills;++i){++player_->at(0x70);if(shin::score_awards_life(player_->at(0x70))){++player_->at(0x8c);sounds.push_back("RESOURCE/bonus.wav");}}
 }
}
}

