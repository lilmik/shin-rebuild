#pragma once
#include "later_bat.hpp"
#include "later_saur.hpp"
#include <variant>
#include <memory>
namespace shin::game {
class LaterAdditionalEnemies {
 using Actor=std::variant<LaterBat,LaterSaur>;struct Entry{int index;Actor actor;};std::vector<std::unique_ptr<Entry>>actors;
public:
 static bool supports(int t){return t==0x30||t==0x77;}
 static const char*resource(int t){return t==0x30?"sauraa.raw":"bat1.raw";}
 static CollisionRect rectangle(EntityPayload&e){return {e.at(0x28),e.at(0x2c),e.at(0x30),e.at(0x34)};}
 void spawn(int t,int x,int y,int index,AssetStore&a,bool one_way_left=false){if(t==0x30)actors.push_back(std::make_unique<Entry>(Entry{index,LaterSaur(x,y,a)}));else actors.push_back(std::make_unique<Entry>(Entry{index,LaterBat(x,y,a)}));if(t==0x77)std::get<LaterBat>(actors.back()->actor).one_way_left=one_way_left;}
 std::vector<EntityPayload*>targets(){std::vector<EntityPayload*>r;for(auto&entry:actors)std::visit([&](auto&a){if(a.payload.state!=2&&a.payload.at(0x58)!=3)r.push_back(&a.payload);},entry->actor);return r;}
 void trigger(EntityPayload&e,int direction){for(auto&entry:actors)std::visit([&](auto&a){if(&a.payload==&e)a.trigger(direction);},entry->actor);}
 bool hurt(EntityPayload&e){for(auto&entry:actors){bool found=false;std::visit([&](auto&a){if(&a.payload==&e&&e.state!=2&&e.at(0x58)!=3){a.hurt();found=true;}},entry->actor);if(found)return true;}return false;}
 void tick(int cx,int cy,PlayerPayload&p,AssetStore&a,IndexedSurface&f,const CollisionWorld&w,const std::function<void(const std::string&)>&sound){for(auto&entry:actors)std::visit([&](auto&actor){using T=std::decay_t<decltype(actor)>;if constexpr(std::is_same_v<T,LaterSaur>)actor.tick(cx,cy,p,a,f);else actor.tick(cx,cy,a,f);auto&e=actor.payload;if(e.state!=2){int material=w.body(rectangle(e),e.at(4),e.at(8));if(material)actor.terrain(w,material);}for(auto&file:actor.sound_requests)if(sound)sound(file);actor.sound_requests.clear();},entry->actor);}
};
}
