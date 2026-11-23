#pragma once
#include <array>
#include <cstdint>
// Rewritten from 0x433900; portable state storage, never accesses original addresses.
// Addresses are provenance IDs. Gameplay action names remain unknown.
namespace shin {
struct InputState {
    std::array<std::uint32_t,17> values{}; // globals 0x46d4b0..0x46d4f0
    std::uint32_t active=0;
    bool cursor_hide_requested=false, quit_requested=false;
    static constexpr std::uint32_t key_address(std::uint32_t vk) {
        switch(vk) {
        case 13: return 0x46d4ecu;
        case 27: return 0x46d4f0u;
        case 32: return 0x46d4e8u;
        case 37: return 0x46d4b8u;
        case 38: return 0x46d4b0u;
        case 39: return 0x46d4bcu;
        case 40: return 0x46d4b4u;
        case 112: return 0x46d4c8u;
        case 113: return 0x46d4ccu;
        case 114: return 0x46d4d0u;
        case 115: return 0x46d4d4u;
        case 116: return 0x46d4d8u;
        case 117: return 0x46d4dcu;
        case 118: return 0x46d4e0u;
        case 120: return 0x46d4e4u;
        default:return 0;
        }
    }
    std::uint32_t read(std::uint32_t original_address) const {
        if(original_address==0x46d594u)return active;
        if(original_address<0x46d4b0u || original_address>0x46d4f0u || original_address%4)return 0;
        return values[(original_address-0x46d4b0u)/4];
    }
    // true means original WndProc consumes message and returns 1.
    // false delegates to the host's default window handler.
    bool message(std::uint32_t msg,std::uint32_t wparam) {
        std::uint32_t address=0,value=0;
        switch(msg) {
        case 2:quit_requested=true;return true;
        case 0x1c:active=wparam;if(wparam==1)cursor_hide_requested=true;return true;
        case 0x100:case 0x101:address=key_address(wparam);value=(msg==0x100);break;
        case 0x201:case 0x202:address=0x46d4c0u;value=(msg==0x201);break;
        case 0x204:case 0x205:address=0x46d4c4u;value=(msg==0x204);break;
        default:return false;
        }
        if(address)values[(address-0x46d4b0u)/4]=value;
        return true; // Unsupported VKs are consumed without mutation, as original.
    }
};
}
