#pragma once
#include "player_controller.hpp"
#include <vector>
namespace shin::game {
struct EntityPayload {int kind=0;unsigned char state=0;std::array<std::int32_t,128> words{};std::int32_t& at(unsigned o){if(o%4||o>=512)throw std::out_of_range("entity field");return words[o/4];}};
struct ObjectResponseContext {
 virtual ~ObjectResponseContext()=default;
 virtual std::int32_t& global(std::uint32_t)=0;
 // Logical IDs: player payload 0x10000, other payload 0x20000, other owner 0x30000.
 // Implementations must interpret these as references, never native addresses.
 virtual std::int32_t call(std::uint32_t,PlayerPayload&,EntityPayload&,std::initializer_list<std::int64_t>)=0;
 virtual std::int32_t sound(std::uint32_t original_sound_global)=0;
};
std::int32_t respond_player_object(PlayerPayload&,EntityPayload&,ObjectResponseContext&,unsigned loop_budget=65536);
}
