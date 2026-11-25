#include "../src/core/interface_music_cache.hpp"
#include <iostream>
struct Music{int id;};
struct Delete{int*count;void operator()(Music*p)const{++*count;delete p;}};
int main(){
 int freed=0;
 {
  shin::InterfaceMusicCache<Music,Delete>cache;
  auto*menu=cache.retain(2,{new Music{2},Delete{&freed}});
  auto*select=cache.retain(3,{new Music{3},Delete{&freed}});
  for(int i=0;i<100;++i)if(cache.find(2)!=menu||cache.find(3)!=select||!cache.owns(menu))return 1;
  if(freed||cache.find(4))return 2;
  bool rejected=false;try{cache.retain(4,{new Music{4},Delete{&freed}});}catch(const std::invalid_argument&){rejected=true;}
  if(!rejected||freed!=1)return 3;
 }
 if(freed!=3)return 4;
 std::cout<<"Interface tracks reused across 100 returns; gameplay pinning rejected; all owned music released\n";
}
