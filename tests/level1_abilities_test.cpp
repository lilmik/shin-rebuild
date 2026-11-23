#include "../src/game/level1_abilities.hpp"
#include <iostream>
int main(int argc,char**argv){
 if(argc!=2)return 1;
 shin::AssetStore a(argv[1]);shin::Level1Abilities abilities(a);shin::PlayerPayload p;shin::initialize_player_payload(p,0,0);
 int damage=0,clears=0,nearest=0;bool saw2=false,saw5=false;
 abilities.damage=[&](const shin::game::CollisionRect&r,int,int){++damage;if(abilities.choice()==2){saw2=true;if(r[0]!=33||r[2]!=100)throw std::runtime_error("wrong mother collider");}if(abilities.choice()==5)saw5=true;return damage==1?1:0;};
 abilities.clear_enemies=[&](){++clears;};
 abilities.nearest_target=[&](int,int,int&x,int&y){++nearest;x=250;y=100;return true;};
 shin::IndexedSurface f(640,480);
 for(int choice:{6,7,2,2,3,5}){
  p.at(0xc0+4*choice)=choice==2&&clears==0?2:1;
  const int before=p.at(0xc0+4*choice),oldclears=clears;
  if(!abilities.activate(choice,p,0,0))return 2;
  const int expected=choice==2&&before==2?2:1;
  if(p.at(0xc0+4*choice)!=before-expected)return 3;
  if(abilities.activate(choice,p,0,0))return 4;
  int ticks=0;while(abilities.active()&&ticks++<1000)abilities.tick(0,0,f);
  if(abilities.active())return 5;
  if((choice==3||(choice==2&&before==2))&&clears!=oldclears+1)return 6;
 }
 for(int i=0;i<30;++i)abilities.tick(0,0,f);
 if(!saw2||!saw5||nearest==0||clears!=2||p.at(0x70)!=1)return 7;
 std::cout<<"ability 2(single/double), 3, 5, 6, 7 assets, inventory, lifetimes, contact and clear callbacks passed\n";
 return 0;
}

