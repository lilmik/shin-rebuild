#pragma once
#include <map>
#include <memory>
#include <stdexcept>
namespace shin {
// Keep only the two frequently revisited interface tracks decoded. Gameplay
// music stays outside this cache and is released on transition as before.
template<class Music,class Deleter> class InterfaceMusicCache {
 std::map<unsigned,std::unique_ptr<Music,Deleter>> tracks_;
public:
 Music*find(unsigned track)const{auto it=tracks_.find(track);return it==tracks_.end()?nullptr:it->second.get();}
 bool owns(const Music*music)const{for(const auto&entry:tracks_)if(entry.second.get()==music)return true;return false;}
 Music*retain(unsigned track,std::unique_ptr<Music,Deleter>music){
  if(track!=2&&track!=3)throw std::invalid_argument("Only interface tracks 2 and 3 may be pinned");
  auto result=tracks_.emplace(track,std::move(music));return result.first->second.get();
 }
};
}
