#pragma once
#include <array>
#include <cstdint>
#include <initializer_list>
#include <stdexcept>
namespace shin {
struct PlayerPayload {
 std::array<std::int32_t,68> words{};
 std::uint8_t owner_state=0;
 // Preserve the original 100-step counter at the rebuild's nominal 60 Hz,
 // while measuring expiry with real elapsed milliseconds (not update calls).
 static constexpr unsigned damage_immunity_ms=1667;
 unsigned immunity_remaining_ms=0,immunity_clock_ms=0;bool immunity_clock_valid=false;
 void grant_damage_immunity(){immunity_remaining_ms=damage_immunity_ms;at(0xb8)=100;}
 void sync_damage_immunity(unsigned now,bool freeze=false){if(immunity_clock_valid&&!freeze){unsigned dt=now-immunity_clock_ms;immunity_remaining_ms=dt>=immunity_remaining_ms?0:immunity_remaining_ms-dt;}immunity_clock_ms=now;immunity_clock_valid=true;at(0xb8)=int((immunity_remaining_ms*100+damage_immunity_ms-1)/damage_immunity_ms);}

 std::int32_t& at(unsigned offset){if(offset%4||offset>=0x110)throw std::out_of_range("player field");return words[offset/4];}
};
// Required host contract. address IDs are recovered dependencies, not callable native pointers.
// call gets state by reference; argument 0 means player base and 0x2c its collision descriptor
// only for functions whose original signature uses that pointer. Other args retain source values.
struct PlayerContext {
 virtual ~PlayerContext()=default;
 virtual std::int32_t& global(std::uint32_t address)=0;
 virtual std::int32_t camera_x()const=0;
 virtual std::int32_t camera_y()const=0;
 virtual std::int32_t call(std::uint32_t address,PlayerPayload&,std::initializer_list<std::int64_t>)=0;
 virtual void render(PlayerPayload&)=0;
};
void update_player(PlayerPayload&,PlayerContext&);
}
