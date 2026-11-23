#include "game_rules.hpp"
#include "player_object_response.hpp"
namespace shin::game {
std::int32_t respond_player_object(PlayerPayload&p,EntityPayload&e,ObjectResponseContext&ctx,unsigned loop_budget){
 auto guard=[&](){if(loop_budget==0)throw std::runtime_error("object response scan budget exhausted");--loop_budget;};
 auto mem=[&](int address)->std::int32_t&{if(address>=0x10000&&address<0x10110)return p.at(address-0x10000);if(address>=0x20000&&address<0x20200)return e.at(address-0x20000);throw std::out_of_range("invalid logical entity address");};
 auto invoke=[&](std::uint32_t id,std::initializer_list<std::int64_t>args)->std::int32_t{
 if(id==0x40d7f0){p.at(0x1c)=0;p.at(0x20)=0;p.at(0x5c)=4;p.at(0x18)=0x45c740;p.at(0x24)=2;p.at(0x28)=5;return ctx.sound(0x4752e8);}
 if(id==0x40d890){p.at(0x1c)=0;p.at(0x20)=0;p.at(0x5c)=7;p.at(0x18)=0x45c748;p.at(0x24)=1;p.at(0x28)=5;return 0;}
 return ctx.call(id,p,e,args);};
#define FUN_004012e0(...) invoke(0x004012e0,{__VA_ARGS__})
#define FUN_00401d10(...) invoke(0x00401d10,{__VA_ARGS__})
#define FUN_00402fd0(...) invoke(0x00402fd0,{__VA_ARGS__})
#define FUN_00403720(...) invoke(0x00403720,{__VA_ARGS__})
#define FUN_00403e50(...) invoke(0x00403e50,{__VA_ARGS__})
#define FUN_00405360(...) invoke(0x00405360,{__VA_ARGS__})
#define FUN_0040d7f0(...) invoke(0x0040d7f0,{__VA_ARGS__})
#define FUN_0040d890(...) invoke(0x0040d890,{__VA_ARGS__})
#define FUN_0040d950(...) invoke(0x0040d950,{__VA_ARGS__})
#define FUN_00414820(...) invoke(0x00414820,{__VA_ARGS__})
#define FUN_00415ab0(...) invoke(0x00415ab0,{__VA_ARGS__})
#define FUN_0041a4e0(...) invoke(0x0041a4e0,{__VA_ARGS__})
#define FUN_0041b4b0(...) invoke(0x0041b4b0,{__VA_ARGS__})
#define FUN_0041ba30(...) invoke(0x0041ba30,{__VA_ARGS__})
#define FUN_0041c810(...) invoke(0x0041c810,{__VA_ARGS__})
#define FUN_0041dab0(...) invoke(0x0041dab0,{__VA_ARGS__})
#define FUN_0041e230(...) invoke(0x0041e230,{__VA_ARGS__})
#define FUN_0041f7f0(...) invoke(0x0041f7f0,{__VA_ARGS__})
#define FUN_00420660(...) invoke(0x00420660,{__VA_ARGS__})
#define FUN_00420ea0(...) invoke(0x00420ea0,{__VA_ARGS__})
#define FUN_00422680(...) invoke(0x00422680,{__VA_ARGS__})
#define FUN_004242a0(...) invoke(0x004242a0,{__VA_ARGS__})
#define FUN_004245a0(...) invoke(0x004245a0,{__VA_ARGS__})
#define FUN_004286e0(...) invoke(0x004286e0,{__VA_ARGS__})
#define FUN_00428b30(...) invoke(0x00428b30,{__VA_ARGS__})
#define FUN_0042ad00(...) invoke(0x0042ad00,{__VA_ARGS__})
#define FUN_0042c4c0(...) invoke(0x0042c4c0,{__VA_ARGS__})
#define FUN_0042ca50(...) invoke(0x0042ca50,{__VA_ARGS__})
#define FUN_0042ebb0(...) invoke(0x0042ebb0,{__VA_ARGS__})
#define FUN_00432f90(...) invoke(0x00432f90,{__VA_ARGS__})
#define FUN_00434ad0(...) invoke(0x00434ad0,{__VA_ARGS__})
#define FUN_00434f90(...) invoke(0x00434f90,{__VA_ARGS__})
#define FUN_00437860(...) invoke(0x00437860,{__VA_ARGS__})
#define FUN_00438670(...) invoke(0x00438670,{__VA_ARGS__})
#define FUN_004393d0(...) invoke(0x004393d0,{__VA_ARGS__})
#define FUN_0043aa70(...) invoke(0x0043aa70,{__VA_ARGS__})
#define FUN_0043b5e0(...) invoke(0x0043b5e0,{__VA_ARGS__})
#define FUN_0043c0a0(...) invoke(0x0043c0a0,{__VA_ARGS__})
#define FUN_0043cd20(...) invoke(0x0043cd20,{__VA_ARGS__})
#define FUN_0043d120(...) invoke(0x0043d120,{__VA_ARGS__})
#define FUN_0043e450(...) invoke(0x0043e450,{__VA_ARGS__})
#define FUN_00440a10(...) invoke(0x00440a10,{__VA_ARGS__})
#define FUN_00441290(...) invoke(0x00441290,{__VA_ARGS__})
#define FUN_00442a20(...) invoke(0x00442a20,{__VA_ARGS__})
#define FUN_00443610(...) invoke(0x00443610,{__VA_ARGS__})
#define FUN_00444ad0(...) invoke(0x00444ad0,{__VA_ARGS__})
#define FUN_004463b0(...) invoke(0x004463b0,{__VA_ARGS__})
#define FUN_00447f70(...) invoke(0x00447f70,{__VA_ARGS__})
#define FUN_0044c610(...) invoke(0x0044c610,{__VA_ARGS__})
#define FUN_0044d220(...) invoke(0x0044d220,{__VA_ARGS__})
#define FUN_0044de50(...) invoke(0x0044de50,{__VA_ARGS__})
{
  int iVar1=0;
  int iVar2;
  int iVar3;
  int iVar4;
  
                    /* 0x98f0  54  0x004098f0 */
  iVar4 = 0x10000;
  iVar1 = static_cast<signed char>(e.kind) + -0x29;
  switch(iVar1) {
  case 0:
    iVar1 = 0x20000;
    if ((mem(iVar1 + 0x3c) != 1) && (mem(iVar4 + 0xc0) == 0)) {
      ctx.global(0x004762a4) = 0x30000;
      mem(iVar4 + 0xc0) = 1;
      return iVar1;
    }
    break;
  case 5:
    ctx.global(0x00471928) = 0;
    ctx.global(0x00471938) = 0;
    ctx.global(0x0047193c) = 0;
    ctx.global(0x00471940) = 0;
    iVar4 = FUN_00401d10(0x2b,0x00401de0,0x004021e0,5);
    e.state = '\x02';
    return iVar4;
  case 6:
    ctx.global(0x00471928) = 0;
    ctx.global(0x00471938) = 0;
    ctx.global(0x0047193c) = 0;
    ctx.global(0x00471940) = 0;
    iVar4 = FUN_0041ba30(0x2b,0x0041bb00,0x0041c240,5);
    e.state = '\x02';
    return iVar4;
  case 7:
    ctx.global(0x00471928) = 0;
    ctx.global(0x00471938) = 0;
    ctx.global(0x0047193c) = 0;
    ctx.global(0x00471940) = 0;
    iVar4 = FUN_004245a0(0x2b,0x004246a0,0x004249e0,5);
    e.state = '\x02';
    return iVar4;
  case 8:
    ctx.global(0x00471928) = 0;
    ctx.global(0x00471938) = 0;
    ctx.global(0x0047193c) = 0;
    ctx.global(0x00471940) = 0;
    iVar4 = FUN_0041c810(0x2b,0x0041c920,0x0041ced0,5);
    e.state = '\x02';
    return iVar4;
  case 9:
    iVar3 = 0x20000;
    iVar1 = mem(iVar4 + 0x5c);
    if ((iVar1 != 4) && (mem(iVar3 + 0x50) != 5)) {
      if (iVar1 == 0x11) {
        FUN_00415ab0(iVar3);
        iVar3 = mem(iVar4 + 0x70) + 1;
        mem(iVar4 + 0x70) = iVar3;
        iVar1 = iVar3 / 100;
        if (shin::score_awards_life(iVar3)) {
          mem(iVar4 + 0x8c) = mem(iVar4 + 0x8c) + 1;
          iVar4 = ctx.sound(0x475200);
          return iVar4;
        }
      }
      else if ((((iVar1 == 2) || (iVar1 == 10)) || (iVar1 == 0xe)) && (0 < mem(iVar4 + 0x58)))
      {
        iVar1 = mem(iVar4 + 0x30) + mem(iVar4 + 0xc);
        if (iVar1 < mem(iVar3 + 0x24) + mem(iVar3 + 8) + -0x14) {
          mem(iVar4 + 0x58) = 0xfffffff1;
          FUN_00415ab0(iVar3);
          iVar3 = mem(iVar4 + 0x70) + 1;
          mem(iVar4 + 0x70) = iVar3;
          iVar1 = iVar3 / 100;
          if (shin::score_awards_life(iVar3)) {
            mem(iVar4 + 0x8c) = mem(iVar4 + 0x8c) + 1;
            iVar4 = ctx.sound(0x475200);
            return iVar4;
          }
        }
        else if (mem(iVar4 + 0xb8) == 0) {
          FUN_0040d7f0(iVar4);
          iVar1 = mem(iVar4 + 0x10) / 2;
          if (mem(iVar4 + 8) + iVar1 < mem(iVar3 + 4) + mem(iVar3 + 0x6c) / 2) {
            mem(iVar4 + 0x54) = 0xfffffff0;
            mem(iVar4 + 0x88) = 1;
          }
          else {
            mem(iVar4 + 0x54) = 0x10;
            mem(iVar4 + 0x88) = 0;
          }
          mem(iVar4 + 0x58) = 0xfffffffb;
          return iVar1;
        }
      }
      else if (mem(iVar4 + 0xb8) == 0) {
        FUN_0040d7f0(iVar4);
        iVar1 = mem(iVar4 + 0x10) / 2;
        if (mem(iVar4 + 8) + iVar1 < mem(iVar3 + 4) + mem(iVar3 + 0x6c) / 2) {
          mem(iVar4 + 0x54) = 0xfffffff0;
          mem(iVar4 + 0x88) = 1;
        }
        else {
          mem(iVar4 + 0x54) = 0x10;
          mem(iVar4 + 0x88) = 0;
        }
        mem(iVar4 + 0x58) = 0xfffffffb;
        return iVar1;
      }
    }
    break;
  case 0x13:
    iVar3 = 0x20000;
    if ((mem(iVar4 + 0x5c) != 4) && (mem(iVar4 + 0xb8) == 0)) {
      FUN_0040d7f0(iVar4);
      iVar1 = mem(iVar3 + 0x3c);
      if (iVar1 < 0) {
        mem(iVar4 + 0x54) = 0xfffffff0;
        mem(iVar4 + 0x88) = 1;
      }
      else if ((iVar1 < 1) &&
              (iVar1 = mem(iVar4 + 0x10) / 2,
              mem(iVar4 + 8) + iVar1 < mem(iVar3 + 4) + mem(iVar3 + 0xc) / 2)) {
        mem(iVar4 + 0x54) = 0xfffffff0;
        mem(iVar4 + 0x88) = 1;
      }
      else {
        mem(iVar4 + 0x54) = 0x10;
        mem(iVar4 + 0x88) = 0;
      }
      mem(iVar4 + 0x58) = 0xfffffffb;
      return iVar1;
    }
    break;
  case 0x15:
    iVar3 = 0x20000;
    iVar1 = FUN_00434ad0(0x00476288,mem(iVar4 + 8),mem(iVar4 + 0xc),
                         iVar3 + 0xc,mem(iVar3 + 4),mem(iVar3 + 8));
    while (iVar1 != 0) { guard();
      mem(iVar4 + 0xc) = mem(iVar4 + 0xc) + -1;
      mem(iVar4 + 100) = 1;
      iVar1 = FUN_00434ad0(0x00476288,mem(iVar4 + 8),mem(iVar4 + 0xc),
                           iVar3 + 0xc,mem(iVar3 + 4),mem(iVar3 + 8));
    }
    mem(iVar4 + 0xc) = mem(iVar4 + 0xc) + 1;
    iVar1 = 0;
    if (mem(iVar4 + 100) == 1) {
      iVar1 = mem(iVar3 + 4) - mem(iVar3 + 0x2c);
      mem(iVar4 + 8) = mem(iVar4 + 8) + iVar1;
      return iVar1;
    }
    break;
  case 0x17:
    iVar3 = 0x20000;
    if (0 < mem(iVar4 + 0x58)) {
      iVar2 = FUN_00434ad0(0x00476288,mem(iVar4 + 8),mem(iVar4 + 0xc),
                           iVar3 + 0xc,mem(iVar3 + 4),mem(iVar3 + 8));
      while (iVar1 = 0, iVar2 != 0) {
        mem(iVar4 + 0xc) = mem(iVar4 + 0xc) + -1;
        mem(iVar4 + 0x58) = 0xffffffdd;
        mem(iVar4 + 100) = 1;
        iVar2 = FUN_00434ad0(0x00476288,mem(iVar4 + 8),mem(iVar4 + 0xc),
                             iVar3 + 0xc,mem(iVar3 + 4),mem(iVar3 + 8));
      }
    }
    if (mem(iVar4 + 100) != 0) {
      iVar4 = FUN_00434f90(iVar3);
      return iVar4;
    }
    break;
  case 0x1a:
    if (mem(iVar4 + 0x5c) != 4) {
      iVar4 = FUN_0040d7f0(iVar4);
      return iVar4;
    }
    break;
  case 0x1b:
    iVar3 = 0x20000;
    if ((mem(iVar4 + 0x5c) != 4) && (mem(iVar4 + 0xb8) == 0)) {
      FUN_0040d7f0(iVar4);
      iVar1 = mem(iVar3 + 0x50);
      if (iVar1 < 0) {
        mem(iVar4 + 0x54) = 0xfffffff0;
        mem(iVar4 + 0x88) = 1;
      }
      else if ((iVar1 < 1) &&
              (iVar1 = mem(iVar4 + 0x10) / 2,
              mem(iVar4 + 8) + iVar1 < mem(iVar3 + 4) + mem(iVar3 + 0xc) / 2)) {
        mem(iVar4 + 0x54) = 0xfffffff0;
        mem(iVar4 + 0x88) = 1;
      }
      else {
        mem(iVar4 + 0x54) = 0x10;
        mem(iVar4 + 0x88) = 0;
      }
      mem(iVar4 + 0x58) = 0xfffffffb;
      e.state = '\x02';
      return iVar1;
    }
    break;
  case 0x1d:
    iVar3 = 0x20000;
    if ((mem(iVar4 + 0x5c) != 4) && (mem(iVar4 + 0xb8) == 0)) {
      FUN_0040d7f0(iVar4);
      iVar1 = mem(iVar3 + 0x50);
      if (iVar1 < 0) {
        mem(iVar4 + 0x54) = 0xfffffff0;
        mem(iVar4 + 0x88) = 1;
      }
      else if ((iVar1 < 1) &&
              (iVar1 = mem(iVar4 + 0x10) / 2,
              mem(iVar4 + 8) + iVar1 < mem(iVar3 + 4) + mem(iVar3 + 0xc) / 2)) {
        mem(iVar4 + 0x54) = 0xfffffff0;
        mem(iVar4 + 0x88) = 1;
      }
      else {
        mem(iVar4 + 0x54) = 0x10;
        mem(iVar4 + 0x88) = 0;
      }
      mem(iVar4 + 0x58) = 0xfffffffb;
      e.state = '\x02';
      return iVar1;
    }
    break;
  case 0x1e:
    iVar1 = 0x20000;
    if (mem(iVar1 + 0x3c) != 1) {
      FUN_0044de50(0x30000);
      iVar1 = mem(iVar4 + 0x70) + 1;
      mem(iVar4 + 0x70) = iVar1;
      iVar3 = iVar1 / 100;
      if (shin::score_awards_life(iVar1)) {
        mem(iVar4 + 0x8c) = mem(iVar4 + 0x8c) + 1;
        iVar3 = ctx.sound(0x475200);
      }
      mem(iVar4 + 0xe0) = mem(iVar4 + 0xe0) + 1;
      return iVar3;
    }
    break;
  case 0x1f:
    iVar1 = 0x20000;
    if (mem(iVar1 + 0x3c) != 1) {
      FUN_004242a0(0x30000);
      iVar1 = mem(iVar4 + 0x70) + 1;
      mem(iVar4 + 0x70) = iVar1;
      iVar3 = iVar1 / 100;
      if (shin::score_awards_life(iVar1)) {
        mem(iVar4 + 0x8c) = mem(iVar4 + 0x8c) + 1;
        iVar3 = ctx.sound(0x475200);
      }
      mem(iVar4 + 0xe4) = mem(iVar4 + 0xe4) + 1;
      return iVar3;
    }
    break;
  case 0x20:
    iVar1 = 0x20000;
    if (mem(iVar1 + 0x3c) != 1) {
      FUN_00414820(0x30000);
      iVar1 = mem(iVar4 + 0x70) + 1;
      mem(iVar4 + 0x70) = iVar1;
      iVar3 = iVar1 / 100;
      if (shin::score_awards_life(iVar1)) {
        mem(iVar4 + 0x8c) = mem(iVar4 + 0x8c) + 1;
        iVar3 = ctx.sound(0x475200);
      }
      mem(iVar4 + 0xec) = mem(iVar4 + 0xec) + 1;
      ctx.global(0x0045c330) = ctx.global(0x0045c330) + 1;
      return iVar3;
    }
    break;
  case 0x21:
    iVar1 = 0x20000;
    if (mem(iVar1 + 0x3c) != 1) {
      FUN_00447f70(0x30000);
      iVar1 = mem(iVar4 + 0x70) + 1;
      mem(iVar4 + 0x70) = iVar1;
      iVar3 = iVar1 / 100;
      if (shin::score_awards_life(iVar1)) {
        mem(iVar4 + 0x8c) = mem(iVar4 + 0x8c) + 1;
        iVar3 = ctx.sound(0x475200);
      }
      mem(iVar4 + 0xf0) = mem(iVar4 + 0xf0) + 1;
      ctx.global(0x0045c330) = ctx.global(0x0045c330) + 1;
      return iVar3;
    }
    break;
  case 0x22:
    iVar1 = 0x20000;
    if (mem(iVar1 + 0x3c) != 1) {
      FUN_0043e450(0x30000);
      iVar1 = mem(iVar4 + 0x70) + 1;
      mem(iVar4 + 0x70) = iVar1;
      iVar3 = iVar1 / 100;
      if (shin::score_awards_life(iVar1)) {
        mem(iVar4 + 0x8c) = mem(iVar4 + 0x8c) + 1;
        iVar3 = ctx.sound(0x475200);
      }
      mem(iVar4 + 0xf4) = mem(iVar4 + 0xf4) + 1;
      ctx.global(0x0045c330) = ctx.global(0x0045c330) + 1;
      return iVar3;
    }
    break;
  case 0x23:
    ctx.global(0x00471928) = 0;
    ctx.global(0x00471938) = 0;
    ctx.global(0x0047193c) = 0;
    ctx.global(0x00471940) = 0;
    iVar4 = FUN_0043d120(0x2b,0x0043d400,0x0043d9c0,5);
    e.state = '\x02';
    return iVar4;
  case 0x24:
    iVar3 = 0x20000;
    iVar1 = FUN_00434ad0(0x00476288,mem(iVar4 + 8),mem(iVar4 + 0xc),
                         iVar3 + 0xc,mem(iVar3 + 4),mem(iVar3 + 8));
    while (iVar1 != 0) { guard();
      mem(iVar4 + 0xc) = mem(iVar4 + 0xc) + -1;
      mem(iVar4 + 100) = 1;
      iVar1 = FUN_00434ad0(0x00476288,mem(iVar4 + 8),mem(iVar4 + 0xc),
                           iVar3 + 0xc,mem(iVar3 + 4),mem(iVar3 + 8));
    }
    mem(iVar4 + 0xc) = mem(iVar4 + 0xc) + 1;
    iVar1 = 0;
    if (mem(iVar4 + 100) == 1) {
      iVar1 = mem(iVar3 + 4) - mem(iVar3 + 0x2c);
      mem(iVar4 + 8) = mem(iVar4 + 8) + iVar1;
      return iVar1;
    }
    break;
  case 0x25:
    iVar3 = 0x20000;
    iVar1 = FUN_00434ad0(0x00476288,mem(iVar4 + 8),mem(iVar4 + 0xc),
                         iVar3 + 0xc,mem(iVar3 + 4),mem(iVar3 + 8));
    while (iVar1 != 0) { guard();
      mem(iVar4 + 0xc) = mem(iVar4 + 0xc) + -1;
      mem(iVar4 + 100) = 1;
      iVar1 = FUN_00434ad0(0x00476288,mem(iVar4 + 8),mem(iVar4 + 0xc),
                           iVar3 + 0xc,mem(iVar3 + 4),mem(iVar3 + 8));
    }
    mem(iVar4 + 0xc) = mem(iVar4 + 0xc) + 1;
    iVar1 = 0;
    if (mem(iVar4 + 100) == 1) {
      iVar1 = mem(iVar3 + 4) - mem(iVar3 + 0x2c);
      mem(iVar4 + 8) = mem(iVar4 + 8) + iVar1;
      return iVar1;
    }
    break;
  case 0x26:
    iVar3 = 0x20000;
    iVar1 = FUN_00434ad0(0x00476288,mem(iVar4 + 8),mem(iVar4 + 0xc),
                         iVar3 + 0xc,mem(iVar3 + 4),mem(iVar3 + 8));
    while (iVar1 != 0) { guard();
      mem(iVar4 + 0xc) = mem(iVar4 + 0xc) + -1;
      mem(iVar4 + 100) = 1;
      iVar1 = FUN_00434ad0(0x00476288,mem(iVar4 + 8),mem(iVar4 + 0xc),
                           iVar3 + 0xc,mem(iVar3 + 4),mem(iVar3 + 8));
    }
    mem(iVar4 + 0xc) = mem(iVar4 + 0xc) + 1;
    iVar1 = 0;
    if (mem(iVar4 + 100) == 1) {
      iVar1 = mem(iVar3 + 4) - mem(iVar3 + 0x2c);
      mem(iVar4 + 8) = mem(iVar4 + 8) + iVar1;
      return iVar1;
    }
    break;
  case 0x27:
    iVar1 = 0x20000;
    if (mem(iVar1 + 0x3c) != 1) {
      FUN_0042ebb0(0x30000);
      iVar1 = mem(iVar4 + 0x70) + 1;
      mem(iVar4 + 0x70) = iVar1;
      iVar3 = iVar1 / 100;
      if (shin::score_awards_life(iVar1)) {
        mem(iVar4 + 0x8c) = mem(iVar4 + 0x8c) + 1;
        iVar3 = ctx.sound(0x475200);
      }
      mem(iVar4 + 0xf8) = mem(iVar4 + 0xf8) + 1;
      ctx.global(0x0045c330) = ctx.global(0x0045c330) + 1;
      return iVar3;
    }
    break;
  case 0x28:
    iVar1 = 0x20000;
    if (mem(iVar1 + 0x3c) != 1) {
      FUN_0043c0a0(0x30000);
      iVar1 = mem(iVar4 + 0x70) + 1;
      mem(iVar4 + 0x70) = iVar1;
      iVar3 = iVar1 / 100;
      if (shin::score_awards_life(iVar1)) {
        mem(iVar4 + 0x8c) = mem(iVar4 + 0x8c) + 1;
        iVar3 = ctx.sound(0x475200);
      }
      mem(iVar4 + 0x74) = mem(iVar4 + 0x74) + 1;
      ctx.global(0x0045c330) = ctx.global(0x0045c330) + 1;
      return iVar3;
    }
    break;
  case 0x29:
    iVar4 = FUN_0040d890(iVar4);
    e.state = '\x02';
    return iVar4;
  case 0x2a:
    iVar1 = 0x20000;
    if (mem(iVar1 + 0x3c) != 1) {
      iVar1 = FUN_00428b30(0x30000);
      mem(iVar4 + 0xcc) = mem(iVar4 + 0xcc) + 1;
      return iVar1;
    }
    break;
  case 0x2b:
    iVar1 = 0x20000;
    if (mem(iVar1 + 0x3c) != 1) {
      iVar1 = FUN_004012e0(0x30000);
      mem(iVar4 + 0xd8) = mem(iVar4 + 0xd8) + 1;
      return iVar1;
    }
    break;
  case 0x2c:
    iVar1 = 0x20000;
    if (mem(iVar1 + 0x3c) != 1) {
      iVar1 = FUN_0043aa70(0x30000);
      mem(iVar4 + 200) = mem(iVar4 + 200) + 1;
      return iVar1;
    }
    break;
  case 0x2d:
    iVar4 = FUN_0040d950(iVar4);
    e.state = '\x02';
    return iVar4;
  case 0x2e:
    iVar1 = 0x20000;
    if (mem(iVar1 + 0x3c) != 1) {
      iVar1 = FUN_0041f7f0(0x30000);
      mem(iVar4 + 0xd0) = mem(iVar4 + 0xd0) + 1;
      return iVar1;
    }
    break;
  case 0x2f:
    iVar1 = 0x20000;
    if (mem(iVar1 + 0x3c) != 1) {
      iVar1 = FUN_00441290(0x30000);
      mem(iVar4 + 0xdc) = mem(iVar4 + 0xdc) + 1;
      return iVar1;
    }
    break;
  case 0x30:
    iVar1 = 0x20000;
    if (mem(iVar1 + 0x3c) != 1) {
      iVar1 = FUN_00420660(0x30000);
      mem(iVar4 + 0xd4) = mem(iVar4 + 0xd4) + 1;
      return iVar1;
    }
    break;
  case 0x31:
    iVar1 = 0x20000;
    if (mem(iVar1 + 0x3c) != 1) {
      FUN_00440a10(0x30000);
      iVar1 = mem(iVar4 + 0x70) + 1;
      mem(iVar4 + 0x70) = iVar1;
      iVar3 = iVar1 / 100;
      if (shin::score_awards_life(iVar1)) {
        mem(iVar4 + 0x8c) = mem(iVar4 + 0x8c) + 1;
        iVar3 = ctx.sound(0x475200);
      }
      mem(iVar4 + 0x8c) = mem(iVar4 + 0x8c) + 1;
      return iVar3;
    }
    break;
  case 0x32:
    iVar3 = 0x20000;
    if ((mem(iVar4 + 0x5c) != 4) && (mem(iVar4 + 0xb8) == 0)) {
      FUN_0040d7f0(iVar4);
      iVar1 = mem(iVar3 + 0x3c);
      if (iVar1 < 0) {
        mem(iVar4 + 0x54) = 0xfffffff0;
        mem(iVar4 + 0x88) = 1;
      }
      else if ((iVar1 < 1) &&
              (iVar1 = mem(iVar4 + 0x10) / 2,
              mem(iVar4 + 8) + iVar1 < mem(iVar3 + 4) + mem(iVar3 + 0xc) / 2)) {
        mem(iVar4 + 0x54) = 0xfffffff0;
        mem(iVar4 + 0x88) = 1;
      }
      else {
        mem(iVar4 + 0x54) = 0x10;
        mem(iVar4 + 0x88) = 0;
      }
      mem(iVar4 + 0x58) = 0xfffffffb;
      e.state = '\x02';
      return iVar1;
    }
    break;
  case 0x34:
    iVar3 = 0x20000;
    if ((mem(iVar4 + 0x5c) != 4) && (mem(iVar4 + 0xb8) == 0)) {
      FUN_0040d7f0(iVar4);
      iVar1 = mem(iVar3 + 0x50);
      if (iVar1 < 0) {
        mem(iVar4 + 0x54) = 0xfffffff0;
        mem(iVar4 + 0x88) = 1;
      }
      else if ((iVar1 < 1) &&
              (iVar1 = mem(iVar4 + 0x10) / 2,
              mem(iVar4 + 8) + iVar1 < mem(iVar3 + 4) + mem(iVar3 + 0xc) / 2)) {
        mem(iVar4 + 0x54) = 0xfffffff0;
        mem(iVar4 + 0x88) = 1;
      }
      else {
        mem(iVar4 + 0x54) = 0x10;
        mem(iVar4 + 0x88) = 0;
      }
      mem(iVar4 + 0x58) = 0xfffffffb;
      e.state = '\x02';
      return iVar1;
    }
    break;
  case 0x35:
    iVar3 = 0x20000;
    if ((mem(iVar4 + 0x5c) != 4) && (mem(iVar4 + 0xb8) == 0)) {
      FUN_0040d7f0(iVar4);
      iVar1 = mem(iVar3 + 0x3c);
      if (iVar1 < 0) {
        mem(iVar4 + 0x54) = 0xfffffff0;
        mem(iVar4 + 0x88) = 1;
      }
      else if ((iVar1 < 1) &&
              (iVar1 = mem(iVar3 + 0xc) / 2,
              mem(iVar4 + 8) + mem(iVar4 + 0x10) / 2 < mem(iVar3 + 4) + iVar1)) {
        mem(iVar4 + 0x54) = 0xfffffff0;
        mem(iVar4 + 0x88) = 1;
      }
      else {
        mem(iVar4 + 0x54) = 0x10;
        mem(iVar4 + 0x88) = 0;
      }
      mem(iVar4 + 0x58) = 0xfffffffb;
      e.state = '\x02';
      return iVar1;
    }
    break;
  case 0x36:
    iVar3 = 0x20000;
    if ((mem(iVar4 + 0x5c) != 4) && (mem(iVar4 + 0xb8) == 0)) {
      FUN_0040d7f0(iVar4);
      iVar1 = mem(iVar3 + 0x3c);
      if (iVar1 < 0) {
        mem(iVar4 + 0x54) = 0xfffffff0;
        mem(iVar4 + 0x88) = 1;
      }
      else if ((iVar1 < 1) &&
              (iVar1 = mem(iVar3 + 0xc) / 2,
              mem(iVar4 + 8) + mem(iVar4 + 0x10) / 2 < mem(iVar3 + 4) + iVar1)) {
        mem(iVar4 + 0x54) = 0xfffffff0;
        mem(iVar4 + 0x88) = 1;
      }
      else {
        mem(iVar4 + 0x54) = 0x10;
        mem(iVar4 + 0x88) = 0;
      }
      mem(iVar4 + 0x58) = 0xfffffffb;
      e.state = '\x02';
      return iVar1;
    }
    break;
  case 0x37:
    iVar3 = 0x20000;
    if ((mem(iVar4 + 0x5c) != 4) && (mem(iVar4 + 0xb8) == 0)) {
      FUN_0040d7f0(iVar4);
      iVar1 = mem(iVar3 + 0x50);
      if (iVar1 < 0) {
        mem(iVar4 + 0x54) = 0xfffffff0;
        mem(iVar4 + 0x88) = 1;
      }
      else if ((iVar1 < 1) &&
              (iVar1 = mem(iVar4 + 0x10) / 2,
              mem(iVar4 + 8) + iVar1 < mem(iVar3 + 4) + mem(iVar3 + 0xc) / 2)) {
        mem(iVar4 + 0x54) = 0xfffffff0;
        mem(iVar4 + 0x88) = 1;
      }
      else {
        mem(iVar4 + 0x54) = 0x10;
        mem(iVar4 + 0x88) = 0;
      }
      mem(iVar4 + 0x58) = 0xfffffffb;
      e.state = '\x02';
      return iVar1;
    }
    break;
  case 0x38:
    iVar3 = 0x20000;
    if ((mem(iVar4 + 0x5c) != 4) && (mem(iVar4 + 0xb8) == 0)) {
      FUN_0040d7f0(iVar4);
      iVar1 = mem(iVar3 + 0x3c);
      if (iVar1 < 0) {
        mem(iVar4 + 0x54) = 0xfffffff0;
        mem(iVar4 + 0x88) = 1;
      }
      else if ((iVar1 < 1) &&
              (iVar1 = mem(iVar4 + 0x10) / 2,
              mem(iVar4 + 8) + iVar1 < mem(iVar3 + 4) + mem(iVar3 + 0xc) / 2)) {
        mem(iVar4 + 0x54) = 0xfffffff0;
        mem(iVar4 + 0x88) = 1;
      }
      else {
        mem(iVar4 + 0x54) = 0x10;
        mem(iVar4 + 0x88) = 0;
      }
      mem(iVar4 + 0x58) = 0xfffffffb;
      e.state = '\x02';
      return iVar1;
    }
    break;
  case 0x39:
    iVar3 = 0x20000;
    if ((mem(iVar4 + 0x5c) != 4) && (mem(iVar4 + 0xb8) == 0)) {
      FUN_0040d7f0(iVar4);
      iVar1 = mem(iVar3 + 0x50);
      if (iVar1 < 0) {
        mem(iVar4 + 0x54) = 0xfffffff0;
        mem(iVar4 + 0x88) = 1;
      }
      else if ((iVar1 < 1) &&
              (iVar1 = mem(iVar4 + 0x10) / 2,
              mem(iVar4 + 8) + iVar1 < mem(iVar3 + 4) + mem(iVar3 + 0xc) / 2)) {
        mem(iVar4 + 0x54) = 0xfffffff0;
        mem(iVar4 + 0x88) = 1;
      }
      else {
        mem(iVar4 + 0x54) = 0x10;
        mem(iVar4 + 0x88) = 0;
      }
      mem(iVar4 + 0x58) = 0xfffffffb;
      e.state = '\x02';
      return iVar1;
    }
    break;
  case 0x3a:
    iVar3 = 0x20000;
    if ((mem(iVar4 + 0x5c) != 4) && (mem(iVar4 + 0xb8) == 0)) {
      FUN_0040d7f0(iVar4);
      iVar1 = mem(iVar3 + 0x50);
      if (iVar1 < 0) {
        mem(iVar4 + 0x54) = 0xfffffff0;
        mem(iVar4 + 0x88) = 1;
      }
      else if ((iVar1 < 1) &&
              (iVar1 = mem(iVar4 + 0x10) / 2,
              mem(iVar4 + 8) + iVar1 < mem(iVar3 + 4) + mem(iVar3 + 0xc) / 2)) {
        mem(iVar4 + 0x54) = 0xfffffff0;
        mem(iVar4 + 0x88) = 1;
      }
      else {
        mem(iVar4 + 0x54) = 0x10;
        mem(iVar4 + 0x88) = 0;
      }
      mem(iVar4 + 0x58) = 0xfffffffb;
      e.state = '\x02';
      return iVar1;
    }
    break;
  case 0x3c:
    iVar3 = 0x20000;
    iVar1 = mem(iVar4 + 0x5c);
    if ((iVar1 != 4) && (mem(iVar3 + 0x58) != 3)) {
      if (iVar1 == 0x11) {
        FUN_00403e50(iVar3);
        iVar3 = mem(iVar4 + 0x70) + 1;
        mem(iVar4 + 0x70) = iVar3;
        iVar1 = iVar3 / 100;
        if (shin::score_awards_life(iVar3)) {
          mem(iVar4 + 0x8c) = mem(iVar4 + 0x8c) + 1;
          iVar4 = ctx.sound(0x475200);
          return iVar4;
        }
      }
      else if ((((iVar1 == 2) || (iVar1 == 10)) || (iVar1 == 0xe)) && (0 < mem(iVar4 + 0x58)))
      {
        iVar1 = mem(iVar3 + 8) + mem(iVar3 + 0x2c) + -0x14;
        if (mem(iVar4 + 0xc) + mem(iVar4 + 0x30) < iVar1) {
          mem(iVar4 + 0x58) = 0xfffffff1;
          FUN_00403e50(iVar3);
          iVar3 = mem(iVar4 + 0x70) + 1;
          mem(iVar4 + 0x70) = iVar3;
          iVar1 = iVar3 / 100;
          if (shin::score_awards_life(iVar3)) {
            mem(iVar4 + 0x8c) = mem(iVar4 + 0x8c) + 1;
            iVar4 = ctx.sound(0x475200);
            return iVar4;
          }
        }
        else if (mem(iVar4 + 0xb8) == 0) {
          FUN_0040d7f0(iVar4);
          iVar1 = mem(iVar3 + 0xc) / 2;
          if (mem(iVar4 + 8) + mem(iVar4 + 0x10) / 2 < mem(iVar3 + 4) + iVar1) {
            mem(iVar4 + 0x54) = 0xfffffff0;
            mem(iVar4 + 0x88) = 1;
          }
          else {
            mem(iVar4 + 0x54) = 0x10;
            mem(iVar4 + 0x88) = 0;
          }
          mem(iVar4 + 0x58) = 0xfffffffb;
          return iVar1;
        }
      }
      else if (mem(iVar4 + 0xb8) == 0) {
        FUN_0040d7f0(iVar4);
        iVar1 = mem(iVar3 + 0xc) / 2;
        if (mem(iVar4 + 8) + mem(iVar4 + 0x10) / 2 < mem(iVar3 + 4) + iVar1) {
          mem(iVar4 + 0x54) = 0xfffffff0;
          mem(iVar4 + 0x88) = 1;
        }
        else {
          mem(iVar4 + 0x54) = 0x10;
          mem(iVar4 + 0x88) = 0;
        }
        mem(iVar4 + 0x58) = 0xfffffffb;
        return iVar1;
      }
    }
    break;
  case 0x3d:
    iVar3 = 0x20000;
    iVar1 = mem(iVar4 + 0x5c);
    if ((iVar1 != 4) && (mem(iVar3 + 0x50) != 4)) {
      if (iVar1 == 0x11) {
        FUN_004463b0(iVar3);
        iVar3 = mem(iVar4 + 0x70) + 1;
        mem(iVar4 + 0x70) = iVar3;
        iVar1 = iVar3 / 100;
        if (shin::score_awards_life(iVar3)) {
          mem(iVar4 + 0x8c) = mem(iVar4 + 0x8c) + 1;
          iVar4 = ctx.sound(0x475200);
          return iVar4;
        }
      }
      else if ((((iVar1 == 2) || (iVar1 == 10)) || (iVar1 == 0xe)) && (0 < mem(iVar4 + 0x58)))
      {
        iVar1 = mem(iVar3 + 8) + mem(iVar3 + 0x24) + -0x14;
        if (mem(iVar4 + 0xc) + mem(iVar4 + 0x30) < iVar1) {
          mem(iVar4 + 0x58) = 0xfffffff1;
          FUN_004463b0(iVar3);
          iVar3 = mem(iVar4 + 0x70) + 1;
          mem(iVar4 + 0x70) = iVar3;
          iVar1 = iVar3 / 100;
          if (shin::score_awards_life(iVar3)) {
            mem(iVar4 + 0x8c) = mem(iVar4 + 0x8c) + 1;
            iVar4 = ctx.sound(0x475200);
            return iVar4;
          }
        }
        else if (mem(iVar4 + 0xb8) == 0) {
          FUN_0040d7f0(iVar4);
          iVar1 = mem(iVar4 + 0x10) / 2;
          if (mem(iVar4 + 8) + iVar1 < mem(iVar3 + 4) + mem(iVar3 + 100) / 2) {
            mem(iVar4 + 0x54) = 0xfffffff0;
            mem(iVar4 + 0x88) = 1;
          }
          else {
            mem(iVar4 + 0x54) = 0x10;
            mem(iVar4 + 0x88) = 0;
          }
          mem(iVar4 + 0x58) = 0xfffffffb;
          return iVar1;
        }
      }
      else if (mem(iVar4 + 0xb8) == 0) {
        FUN_0040d7f0(iVar4);
        iVar1 = mem(iVar4 + 0x10) / 2;
        if (mem(iVar4 + 8) + iVar1 < mem(iVar3 + 4) + mem(iVar3 + 100) / 2) {
          mem(iVar4 + 0x54) = 0xfffffff0;
          mem(iVar4 + 0x88) = 1;
        }
        else {
          mem(iVar4 + 0x54) = 0x10;
          mem(iVar4 + 0x88) = 0;
        }
        mem(iVar4 + 0x58) = 0xfffffffb;
        return iVar1;
      }
    }
    break;
  case 0x3e:
    iVar3 = 0x20000;
    iVar1 = mem(iVar4 + 0x5c);
    if ((iVar1 != 4) && (mem(iVar3 + 0x50) != 4)) {
      if (iVar1 == 0x11) {
        FUN_004393d0(iVar3);
        iVar3 = mem(iVar4 + 0x70) + 1;
        mem(iVar4 + 0x70) = iVar3;
        iVar1 = iVar3 / 100;
        if (shin::score_awards_life(iVar3)) {
          mem(iVar4 + 0x8c) = mem(iVar4 + 0x8c) + 1;
          iVar4 = ctx.sound(0x475200);
          return iVar4;
        }
      }
      else if ((((iVar1 == 2) || (iVar1 == 10)) || (iVar1 == 0xe)) && (0 < mem(iVar4 + 0x58)))
      {
        iVar1 = mem(iVar3 + 8) + mem(iVar3 + 0x24) + -0x14;
        if (mem(iVar4 + 0xc) + mem(iVar4 + 0x30) < iVar1) {
          mem(iVar4 + 0x58) = 0xfffffff1;
          FUN_004393d0(iVar3);
          iVar3 = mem(iVar4 + 0x70) + 1;
          mem(iVar4 + 0x70) = iVar3;
          iVar1 = iVar3 / 100;
          if (shin::score_awards_life(iVar3)) {
            mem(iVar4 + 0x8c) = mem(iVar4 + 0x8c) + 1;
            iVar4 = ctx.sound(0x475200);
            return iVar4;
          }
        }
        else if (mem(iVar4 + 0xb8) == 0) {
          FUN_0040d7f0(iVar4);
          iVar1 = mem(iVar3 + 0x6c) / 2;
          if (mem(iVar4 + 8) + mem(iVar4 + 0x10) / 2 < mem(iVar3 + 4) + iVar1) {
            mem(iVar4 + 0x54) = 0xfffffff0;
            mem(iVar4 + 0x88) = 1;
          }
          else {
            mem(iVar4 + 0x54) = 0x10;
            mem(iVar4 + 0x88) = 0;
          }
          mem(iVar4 + 0x58) = 0xfffffffb;
          return iVar1;
        }
      }
      else if (mem(iVar4 + 0xb8) == 0) {
        FUN_0040d7f0(iVar4);
        iVar1 = mem(iVar3 + 0x6c) / 2;
        if (mem(iVar4 + 8) + mem(iVar4 + 0x10) / 2 < mem(iVar3 + 4) + iVar1) {
          mem(iVar4 + 0x54) = 0xfffffff0;
          mem(iVar4 + 0x88) = 1;
        }
        else {
          mem(iVar4 + 0x54) = 0x10;
          mem(iVar4 + 0x88) = 0;
        }
        mem(iVar4 + 0x58) = 0xfffffffb;
        return iVar1;
      }
    }
    break;
  case 0x3f:
    iVar3 = 0x20000;
    iVar1 = mem(iVar4 + 0x5c);
    if ((iVar1 != 4) && (mem(iVar3 + 0x58) != 4)) {
      if (iVar1 == 0x11) {
        FUN_0041a4e0(iVar3);
        iVar3 = mem(iVar4 + 0x70) + 1;
        mem(iVar4 + 0x70) = iVar3;
        iVar1 = iVar3 / 100;
        if (shin::score_awards_life(iVar3)) {
          mem(iVar4 + 0x8c) = mem(iVar4 + 0x8c) + 1;
          iVar4 = ctx.sound(0x475200);
          return iVar4;
        }
      }
      else if ((((iVar1 == 2) || (iVar1 == 10)) || (iVar1 == 0xe)) && (0 < mem(iVar4 + 0x58)))
      {
        iVar1 = mem(iVar3 + 8) + mem(iVar3 + 0x2c) + -0x14;
        if (mem(iVar4 + 0xc) + mem(iVar4 + 0x30) < iVar1) {
          mem(iVar4 + 0x58) = 0xfffffff1;
          FUN_0041a4e0(iVar3);
          iVar3 = mem(iVar4 + 0x70) + 1;
          mem(iVar4 + 0x70) = iVar3;
          iVar1 = iVar3 / 100;
          if (shin::score_awards_life(iVar3)) {
            mem(iVar4 + 0x8c) = mem(iVar4 + 0x8c) + 1;
            iVar4 = ctx.sound(0x475200);
            return iVar4;
          }
        }
        else if (mem(iVar4 + 0xb8) == 0) {
          FUN_0040d7f0(iVar4);
          iVar1 = mem(iVar4 + 0x10) / 2;
          if (mem(iVar4 + 8) + iVar1 < mem(iVar3 + 4) + mem(iVar3 + 0xc) / 2) {
            mem(iVar4 + 0x54) = 0xfffffff0;
            mem(iVar4 + 0x88) = 1;
          }
          else {
            mem(iVar4 + 0x54) = 0x10;
            mem(iVar4 + 0x88) = 0;
          }
          mem(iVar4 + 0x58) = 0xfffffffb;
          return iVar1;
        }
      }
      else if (mem(iVar4 + 0xb8) == 0) {
        FUN_0040d7f0(iVar4);
        iVar1 = mem(iVar4 + 0x10) / 2;
        if (mem(iVar4 + 8) + iVar1 < mem(iVar3 + 4) + mem(iVar3 + 0xc) / 2) {
          mem(iVar4 + 0x54) = 0xfffffff0;
          mem(iVar4 + 0x88) = 1;
        }
        else {
          mem(iVar4 + 0x54) = 0x10;
          mem(iVar4 + 0x88) = 0;
        }
        mem(iVar4 + 0x58) = 0xfffffffb;
        return iVar1;
      }
    }
    break;
  case 0x40:
    iVar3 = 0x20000;
    iVar1 = mem(iVar4 + 0x5c);
    if ((iVar1 != 4) && (mem(iVar3 + 0x58) != 4)) {
      if (iVar1 == 0x11) {
        FUN_0044c610(iVar3);
        iVar3 = mem(iVar4 + 0x70) + 1;
        mem(iVar4 + 0x70) = iVar3;
        iVar1 = iVar3 / 100;
        if (shin::score_awards_life(iVar3)) {
          mem(iVar4 + 0x8c) = mem(iVar4 + 0x8c) + 1;
          iVar4 = ctx.sound(0x475200);
          return iVar4;
        }
      }
      else if ((((iVar1 == 2) || (iVar1 == 10)) || (iVar1 == 0xe)) && (0 < mem(iVar4 + 0x58)))
      {
        iVar1 = mem(iVar3 + 8) + mem(iVar3 + 0x2c) + -0x14;
        if (mem(iVar4 + 0xc) + mem(iVar4 + 0x30) < iVar1) {
          mem(iVar4 + 0x58) = 0xfffffff1;
          FUN_0044c610(iVar3);
          iVar3 = mem(iVar4 + 0x70) + 1;
          mem(iVar4 + 0x70) = iVar3;
          iVar1 = iVar3 / 100;
          if (shin::score_awards_life(iVar3)) {
            mem(iVar4 + 0x8c) = mem(iVar4 + 0x8c) + 1;
            iVar4 = ctx.sound(0x475200);
            return iVar4;
          }
        }
        else if (mem(iVar4 + 0xb8) == 0) {
          FUN_0040d7f0(iVar4);
          iVar1 = mem(iVar4 + 0x10) / 2;
          if (mem(iVar4 + 8) + iVar1 < mem(iVar3 + 4) + mem(iVar3 + 0xc) / 2) {
            mem(iVar4 + 0x54) = 0xfffffff0;
            mem(iVar4 + 0x88) = 1;
          }
          else {
            mem(iVar4 + 0x54) = 0x10;
            mem(iVar4 + 0x88) = 0;
          }
          mem(iVar4 + 0x58) = 0xfffffffb;
          return iVar1;
        }
      }
      else if (mem(iVar4 + 0xb8) == 0) {
        FUN_0040d7f0(iVar4);
        iVar1 = mem(iVar4 + 0x10) / 2;
        if (mem(iVar4 + 8) + iVar1 < mem(iVar3 + 4) + mem(iVar3 + 0xc) / 2) {
          mem(iVar4 + 0x54) = 0xfffffff0;
          mem(iVar4 + 0x88) = 1;
        }
        else {
          mem(iVar4 + 0x54) = 0x10;
          mem(iVar4 + 0x88) = 0;
        }
        mem(iVar4 + 0x58) = 0xfffffffb;
        return iVar1;
      }
    }
    break;
  case 0x41:
    iVar3 = 0x20000;
    iVar1 = mem(iVar4 + 0x5c);
    if ((iVar1 != 4) && (mem(iVar3 + 0x58) != 4)) {
      if (iVar1 == 0x11) {
        FUN_004286e0(iVar3);
        iVar3 = mem(iVar4 + 0x70) + 1;
        mem(iVar4 + 0x70) = iVar3;
        iVar1 = iVar3 / 100;
        if (shin::score_awards_life(iVar3)) {
          mem(iVar4 + 0x8c) = mem(iVar4 + 0x8c) + 1;
          iVar4 = ctx.sound(0x475200);
          return iVar4;
        }
      }
      else if ((((iVar1 == 2) || (iVar1 == 10)) || (iVar1 == 0xe)) && (0 < mem(iVar4 + 0x58)))
      {
        iVar1 = mem(iVar3 + 8) + mem(iVar3 + 0x2c) + -0x14;
        if (mem(iVar4 + 0xc) + mem(iVar4 + 0x30) < iVar1) {
          mem(iVar4 + 0x58) = 0xfffffff1;
          FUN_004286e0(iVar3);
          iVar3 = mem(iVar4 + 0x70) + 1;
          mem(iVar4 + 0x70) = iVar3;
          iVar1 = iVar3 / 100;
          if (shin::score_awards_life(iVar3)) {
            mem(iVar4 + 0x8c) = mem(iVar4 + 0x8c) + 1;
            iVar4 = ctx.sound(0x475200);
            return iVar4;
          }
        }
        else if (mem(iVar4 + 0xb8) == 0) {
          FUN_0040d7f0(iVar4);
          iVar1 = mem(iVar4 + 0x10) / 2;
          if (mem(iVar4 + 8) + iVar1 < mem(iVar3 + 4) + mem(iVar3 + 0xc) / 2) {
            mem(iVar4 + 0x54) = 0xfffffff0;
            mem(iVar4 + 0x88) = 1;
          }
          else {
            mem(iVar4 + 0x54) = 0x10;
            mem(iVar4 + 0x88) = 0;
          }
          mem(iVar4 + 0x58) = 0xfffffffb;
          return iVar1;
        }
      }
      else if (mem(iVar4 + 0xb8) == 0) {
        FUN_0040d7f0(iVar4);
        iVar1 = mem(iVar4 + 0x10) / 2;
        if (mem(iVar4 + 8) + iVar1 < mem(iVar3 + 4) + mem(iVar3 + 0xc) / 2) {
          mem(iVar4 + 0x54) = 0xfffffff0;
          mem(iVar4 + 0x88) = 1;
        }
        else {
          mem(iVar4 + 0x54) = 0x10;
          mem(iVar4 + 0x88) = 0;
        }
        mem(iVar4 + 0x58) = 0xfffffffb;
        return iVar1;
      }
    }
    break;
  case 0x42:
    iVar3 = 0x20000;
    iVar1 = mem(iVar4 + 0x5c);
    if ((iVar1 != 4) && (mem(iVar3 + 0x50) != 4)) {
      if (iVar1 == 0x11) {
        FUN_00437860(iVar3);
        iVar3 = mem(iVar4 + 0x70) + 1;
        mem(iVar4 + 0x70) = iVar3;
        iVar1 = iVar3 / 100;
        if (shin::score_awards_life(iVar3)) {
          mem(iVar4 + 0x8c) = mem(iVar4 + 0x8c) + 1;
          iVar4 = ctx.sound(0x475200);
          return iVar4;
        }
      }
      else if ((((iVar1 == 2) || (iVar1 == 10)) || (iVar1 == 0xe)) && (0 < mem(iVar4 + 0x58)))
      {
        iVar1 = mem(iVar3 + 0x24) + mem(iVar3 + 8) + -0x14;
        if (mem(iVar4 + 0xc) + mem(iVar4 + 0x30) < iVar1) {
          mem(iVar4 + 0x58) = 0xfffffff1;
          FUN_00437860(iVar3);
          iVar3 = mem(iVar4 + 0x70) + 1;
          mem(iVar4 + 0x70) = iVar3;
          iVar1 = iVar3 / 100;
          if (shin::score_awards_life(iVar3)) {
            mem(iVar4 + 0x8c) = mem(iVar4 + 0x8c) + 1;
            iVar4 = ctx.sound(0x475200);
            return iVar4;
          }
        }
        else if (mem(iVar4 + 0xb8) == 0) {
          FUN_0040d7f0(iVar4);
          iVar1 = mem(iVar3 + 100) / 2;
          if (mem(iVar4 + 8) + mem(iVar4 + 0x10) / 2 < mem(iVar3 + 4) + iVar1) {
            mem(iVar4 + 0x54) = 0xfffffff0;
            mem(iVar4 + 0x88) = 1;
          }
          else {
            mem(iVar4 + 0x54) = 0x10;
            mem(iVar4 + 0x88) = 0;
          }
          mem(iVar4 + 0x58) = 0xfffffffb;
          return iVar1;
        }
      }
      else if (mem(iVar4 + 0xb8) == 0) {
        FUN_0040d7f0(iVar4);
        iVar1 = mem(iVar3 + 100) / 2;
        if (mem(iVar4 + 8) + mem(iVar4 + 0x10) / 2 < mem(iVar3 + 4) + iVar1) {
          mem(iVar4 + 0x54) = 0xfffffff0;
          mem(iVar4 + 0x88) = 1;
        }
        else {
          mem(iVar4 + 0x54) = 0x10;
          mem(iVar4 + 0x88) = 0;
        }
        mem(iVar4 + 0x58) = 0xfffffffb;
        return iVar1;
      }
    }
    break;
  case 0x43:
    iVar3 = 0x20000;
    iVar1 = mem(iVar4 + 0x5c);
    if ((iVar1 != 4) && (mem(iVar3 + 0x58) != 4)) {
      if (iVar1 == 0x11) {
        FUN_00403720(iVar3);
        iVar3 = mem(iVar4 + 0x70) + 1;
        mem(iVar4 + 0x70) = iVar3;
        iVar1 = iVar3 / 100;
        if (shin::score_awards_life(iVar3)) {
          mem(iVar4 + 0x8c) = mem(iVar4 + 0x8c) + 1;
          iVar4 = ctx.sound(0x475200);
          return iVar4;
        }
      }
      else if ((((iVar1 == 2) || (iVar1 == 10)) || (iVar1 == 0xe)) && (0 < mem(iVar4 + 0x58)))
      {
        iVar1 = mem(iVar3 + 8) + mem(iVar3 + 0x2c) + -0x14;
        if (mem(iVar4 + 0xc) + mem(iVar4 + 0x30) < iVar1) {
          mem(iVar4 + 0x58) = 0xfffffff1;
          FUN_00403720(iVar3);
          iVar3 = mem(iVar4 + 0x70) + 1;
          mem(iVar4 + 0x70) = iVar3;
          iVar1 = iVar3 / 100;
          if (shin::score_awards_life(iVar3)) {
            mem(iVar4 + 0x8c) = mem(iVar4 + 0x8c) + 1;
            iVar4 = ctx.sound(0x475200);
            return iVar4;
          }
        }
        else if (mem(iVar4 + 0xb8) == 0) {
          FUN_0040d7f0(iVar4);
          iVar1 = mem(iVar4 + 0x10) / 2;
          if (mem(iVar4 + 8) + iVar1 < mem(iVar3 + 4) + mem(iVar3 + 0xc) / 2) {
            mem(iVar4 + 0x54) = 0xfffffff0;
            mem(iVar4 + 0x88) = 1;
          }
          else {
            mem(iVar4 + 0x54) = 0x10;
            mem(iVar4 + 0x88) = 0;
          }
          mem(iVar4 + 0x58) = 0xfffffffb;
          return iVar1;
        }
      }
      else if (mem(iVar4 + 0xb8) == 0) {
        FUN_0040d7f0(iVar4);
        iVar1 = mem(iVar4 + 0x10) / 2;
        if (mem(iVar4 + 8) + iVar1 < mem(iVar3 + 4) + mem(iVar3 + 0xc) / 2) {
          mem(iVar4 + 0x54) = 0xfffffff0;
          mem(iVar4 + 0x88) = 1;
        }
        else {
          mem(iVar4 + 0x54) = 0x10;
          mem(iVar4 + 0x88) = 0;
        }
        mem(iVar4 + 0x58) = 0xfffffffb;
        return iVar1;
      }
    }
    break;
  case 0x44:
    iVar3 = 0x20000;
    iVar1 = mem(iVar4 + 0x5c);
    if ((iVar1 != 4) && (mem(iVar3 + 0x58) != 4)) {
      if (iVar1 == 0x11) {
        FUN_0043cd20(iVar3);
        iVar3 = mem(iVar4 + 0x70) + 1;
        mem(iVar4 + 0x70) = iVar3;
        iVar1 = iVar3 / 100;
        if (shin::score_awards_life(iVar3)) {
          mem(iVar4 + 0x8c) = mem(iVar4 + 0x8c) + 1;
          iVar4 = ctx.sound(0x475200);
          return iVar4;
        }
      }
      else if ((((iVar1 == 2) || (iVar1 == 10)) || (iVar1 == 0xe)) && (0 < mem(iVar4 + 0x58)))
      {
        iVar1 = mem(iVar4 + 0xc) + mem(iVar4 + 0x30);
        if (iVar1 < mem(iVar3 + 8) + mem(iVar3 + 0x2c) + -0x14) {
          mem(iVar4 + 0x58) = 0xfffffff1;
          FUN_0043cd20(iVar3);
          iVar3 = mem(iVar4 + 0x70) + 1;
          mem(iVar4 + 0x70) = iVar3;
          iVar1 = iVar3 / 100;
          if (shin::score_awards_life(iVar3)) {
            mem(iVar4 + 0x8c) = mem(iVar4 + 0x8c) + 1;
            iVar4 = ctx.sound(0x475200);
            return iVar4;
          }
        }
        else if (mem(iVar4 + 0xb8) == 0) {
          FUN_0040d7f0(iVar4);
          iVar1 = mem(iVar3 + 0xc) / 2;
          if (mem(iVar4 + 8) + mem(iVar4 + 0x10) / 2 < mem(iVar3 + 4) + iVar1) {
            mem(iVar4 + 0x54) = 0xfffffff0;
            mem(iVar4 + 0x88) = 1;
          }
          else {
            mem(iVar4 + 0x54) = 0x10;
            mem(iVar4 + 0x88) = 0;
          }
          mem(iVar4 + 0x58) = 0xfffffffb;
          return iVar1;
        }
      }
      else if (mem(iVar4 + 0xb8) == 0) {
        FUN_0040d7f0(iVar4);
        iVar1 = mem(iVar3 + 0xc) / 2;
        if (mem(iVar4 + 8) + mem(iVar4 + 0x10) / 2 < mem(iVar3 + 4) + iVar1) {
          mem(iVar4 + 0x54) = 0xfffffff0;
          mem(iVar4 + 0x88) = 1;
        }
        else {
          mem(iVar4 + 0x54) = 0x10;
          mem(iVar4 + 0x88) = 0;
        }
        mem(iVar4 + 0x58) = 0xfffffffb;
        return iVar1;
      }
    }
    break;
  case 0x45:
    iVar3 = 0x20000;
    iVar1 = mem(iVar4 + 0x5c);
    if ((iVar1 != 4) && (mem(iVar3 + 0x58) != 4)) {
      if (iVar1 == 0x11) {
        FUN_0042c4c0(iVar3);
        iVar3 = mem(iVar4 + 0x70) + 1;
        mem(iVar4 + 0x70) = iVar3;
        iVar1 = iVar3 / 100;
        if (shin::score_awards_life(iVar3)) {
          mem(iVar4 + 0x8c) = mem(iVar4 + 0x8c) + 1;
          iVar4 = ctx.sound(0x475200);
          return iVar4;
        }
      }
      else if ((((iVar1 == 2) || (iVar1 == 10)) || (iVar1 == 0xe)) && (0 < mem(iVar4 + 0x58)))
      {
        iVar1 = mem(iVar4 + 0xc) + mem(iVar4 + 0x30);
        if (iVar1 < mem(iVar3 + 8) + mem(iVar3 + 0x2c) + -0x14) {
          mem(iVar4 + 0x58) = 0xfffffff1;
          FUN_0042c4c0(iVar3);
          iVar3 = mem(iVar4 + 0x70) + 1;
          mem(iVar4 + 0x70) = iVar3;
          iVar1 = iVar3 / 100;
          if (shin::score_awards_life(iVar3)) {
            mem(iVar4 + 0x8c) = mem(iVar4 + 0x8c) + 1;
            iVar4 = ctx.sound(0x475200);
            return iVar4;
          }
        }
        else if (mem(iVar4 + 0xb8) == 0) {
          FUN_0040d7f0(iVar4);
          iVar1 = mem(iVar4 + 0x10) / 2;
          if (mem(iVar4 + 8) + iVar1 < mem(iVar3 + 4) + mem(iVar3 + 0xc) / 2) {
            mem(iVar4 + 0x54) = 0xfffffff0;
            mem(iVar4 + 0x88) = 1;
          }
          else {
            mem(iVar4 + 0x54) = 0x10;
            mem(iVar4 + 0x88) = 0;
          }
          mem(iVar4 + 0x58) = 0xfffffffb;
          return iVar1;
        }
      }
      else if (mem(iVar4 + 0xb8) == 0) {
        FUN_0040d7f0(iVar4);
        iVar1 = mem(iVar4 + 0x10) / 2;
        if (mem(iVar4 + 8) + iVar1 < mem(iVar3 + 4) + mem(iVar3 + 0xc) / 2) {
          mem(iVar4 + 0x54) = 0xfffffff0;
          mem(iVar4 + 0x88) = 1;
        }
        else {
          mem(iVar4 + 0x54) = 0x10;
          mem(iVar4 + 0x88) = 0;
        }
        mem(iVar4 + 0x58) = 0xfffffffb;
        return iVar1;
      }
    }
    break;
  case 0x46:
    iVar3 = 0x20000;
    iVar1 = mem(iVar4 + 0x5c);
    if ((iVar1 != 4) && (mem(iVar3 + 0x50) != 4)) {
      if (iVar1 == 0x11) {
        FUN_00432f90(iVar3);
        iVar3 = mem(iVar4 + 0x70) + 1;
        mem(iVar4 + 0x70) = iVar3;
        iVar1 = iVar3 / 100;
        if (shin::score_awards_life(iVar3)) {
          mem(iVar4 + 0x8c) = mem(iVar4 + 0x8c) + 1;
          iVar4 = ctx.sound(0x475200);
          return iVar4;
        }
      }
      else if ((((iVar1 == 2) || (iVar1 == 10)) || (iVar1 == 0xe)) && (0 < mem(iVar4 + 0x58)))
      {
        iVar1 = mem(iVar4 + 0xc) + mem(iVar4 + 0x30);
        if (iVar1 < mem(iVar3 + 0x24) + mem(iVar3 + 8) + -0x14) {
          mem(iVar4 + 0x58) = 0xfffffff1;
          FUN_00432f90(iVar3);
          iVar3 = mem(iVar4 + 0x70) + 1;
          mem(iVar4 + 0x70) = iVar3;
          iVar1 = iVar3 / 100;
          if (shin::score_awards_life(iVar3)) {
            mem(iVar4 + 0x8c) = mem(iVar4 + 0x8c) + 1;
            iVar4 = ctx.sound(0x475200);
            return iVar4;
          }
        }
        else if (mem(iVar4 + 0xb8) == 0) {
          FUN_0040d7f0(iVar4);
          iVar1 = mem(iVar3 + 100) / 2;
          if (mem(iVar4 + 8) + mem(iVar4 + 0x10) / 2 < mem(iVar3 + 4) + iVar1) {
            mem(iVar4 + 0x54) = 0xfffffff0;
            mem(iVar4 + 0x88) = 1;
          }
          else {
            mem(iVar4 + 0x54) = 0x10;
            mem(iVar4 + 0x88) = 0;
          }
          mem(iVar4 + 0x58) = 0xfffffffb;
          return iVar1;
        }
      }
      else if (mem(iVar4 + 0xb8) == 0) {
        FUN_0040d7f0(iVar4);
        iVar1 = mem(iVar3 + 100) / 2;
        if (mem(iVar4 + 8) + mem(iVar4 + 0x10) / 2 < mem(iVar3 + 4) + iVar1) {
          mem(iVar4 + 0x54) = 0xfffffff0;
          mem(iVar4 + 0x88) = 1;
        }
        else {
          mem(iVar4 + 0x54) = 0x10;
          mem(iVar4 + 0x88) = 0;
        }
        mem(iVar4 + 0x58) = 0xfffffffb;
        return iVar1;
      }
    }
    break;
  case 0x47:
    iVar3 = 0x20000;
    iVar1 = mem(iVar4 + 0x5c);
    if ((iVar1 != 4) && (mem(iVar3 + 0x58) != 3)) {
      if (iVar1 == 0x11) {
        FUN_00442a20(iVar3);
        iVar3 = mem(iVar4 + 0x70) + 1;
        mem(iVar4 + 0x70) = iVar3;
        iVar1 = iVar3 / 100;
        if (shin::score_awards_life(iVar3)) {
          mem(iVar4 + 0x8c) = mem(iVar4 + 0x8c) + 1;
          iVar4 = ctx.sound(0x475200);
          return iVar4;
        }
      }
      else if ((((iVar1 == 2) || (iVar1 == 10)) || (iVar1 == 0xe)) && (0 < mem(iVar4 + 0x58)))
      {
        iVar1 = mem(iVar3 + 8) + mem(iVar3 + 0x2c) + -0x14;
        if (mem(iVar4 + 0xc) + mem(iVar4 + 0x30) < iVar1) {
          mem(iVar4 + 0x58) = 0xfffffff1;
          FUN_00442a20(iVar3);
          iVar3 = mem(iVar4 + 0x70) + 1;
          mem(iVar4 + 0x70) = iVar3;
          iVar1 = iVar3 / 100;
          if (shin::score_awards_life(iVar3)) {
            mem(iVar4 + 0x8c) = mem(iVar4 + 0x8c) + 1;
            iVar4 = ctx.sound(0x475200);
            return iVar4;
          }
        }
        else if (mem(iVar4 + 0xb8) == 0) {
          FUN_0040d7f0(iVar4);
          iVar1 = mem(iVar3 + 0xc) / 2;
          if (mem(iVar4 + 8) + mem(iVar4 + 0x10) / 2 < mem(iVar3 + 4) + iVar1) {
            mem(iVar4 + 0x54) = 0xfffffff0;
            mem(iVar4 + 0x88) = 1;
          }
          else {
            mem(iVar4 + 0x54) = 0x10;
            mem(iVar4 + 0x88) = 0;
          }
          mem(iVar4 + 0x58) = 0xfffffffb;
          return iVar1;
        }
      }
      else if (mem(iVar4 + 0xb8) == 0) {
        FUN_0040d7f0(iVar4);
        iVar1 = mem(iVar3 + 0xc) / 2;
        if (mem(iVar4 + 8) + mem(iVar4 + 0x10) / 2 < mem(iVar3 + 4) + iVar1) {
          mem(iVar4 + 0x54) = 0xfffffff0;
          mem(iVar4 + 0x88) = 1;
        }
        else {
          mem(iVar4 + 0x54) = 0x10;
          mem(iVar4 + 0x88) = 0;
        }
        mem(iVar4 + 0x58) = 0xfffffffb;
        return iVar1;
      }
    }
    break;
  case 0x48:
    iVar3 = 0x20000;
    iVar1 = mem(iVar4 + 0x5c);
    if ((iVar1 != 4) && (mem(iVar3 + 0x58) != 3)) {
      if (iVar1 == 0x11) {
        FUN_0041b4b0(iVar3);
        iVar3 = mem(iVar4 + 0x70) + 1;
        mem(iVar4 + 0x70) = iVar3;
        iVar1 = iVar3 / 100;
        if (shin::score_awards_life(iVar3)) {
          mem(iVar4 + 0x8c) = mem(iVar4 + 0x8c) + 1;
          iVar4 = ctx.sound(0x475200);
          return iVar4;
        }
      }
      else if ((((iVar1 == 2) || (iVar1 == 10)) || (iVar1 == 0xe)) && (0 < mem(iVar4 + 0x58)))
      {
        iVar1 = mem(iVar3 + 8) + mem(iVar3 + 0x2c) + -0x14;
        if (mem(iVar4 + 0xc) + mem(iVar4 + 0x30) < iVar1) {
          mem(iVar4 + 0x58) = 0xfffffff1;
          FUN_0041b4b0(iVar3);
          iVar3 = mem(iVar4 + 0x70) + 1;
          mem(iVar4 + 0x70) = iVar3;
          iVar1 = iVar3 / 100;
          if (shin::score_awards_life(iVar3)) {
            mem(iVar4 + 0x8c) = mem(iVar4 + 0x8c) + 1;
            iVar4 = ctx.sound(0x475200);
            return iVar4;
          }
        }
        else if (mem(iVar4 + 0xb8) == 0) {
          FUN_0040d7f0(iVar4);
          iVar1 = mem(iVar4 + 0x10) / 2;
          if (mem(iVar4 + 8) + iVar1 < mem(iVar3 + 4) + mem(iVar3 + 0xc) / 2) {
            mem(iVar4 + 0x54) = 0xfffffff0;
            mem(iVar4 + 0x88) = 1;
          }
          else {
            mem(iVar4 + 0x54) = 0x10;
            mem(iVar4 + 0x88) = 0;
          }
          mem(iVar4 + 0x58) = 0xfffffffb;
          return iVar1;
        }
      }
      else if (mem(iVar4 + 0xb8) == 0) {
        FUN_0040d7f0(iVar4);
        iVar1 = mem(iVar4 + 0x10) / 2;
        if (mem(iVar4 + 8) + iVar1 < mem(iVar3 + 4) + mem(iVar3 + 0xc) / 2) {
          mem(iVar4 + 0x54) = 0xfffffff0;
          mem(iVar4 + 0x88) = 1;
        }
        else {
          mem(iVar4 + 0x54) = 0x10;
          mem(iVar4 + 0x88) = 0;
        }
        mem(iVar4 + 0x58) = 0xfffffffb;
        return iVar1;
      }
    }
    break;
  case 0x49:
    iVar3 = 0x20000;
    iVar1 = mem(iVar4 + 0x5c);
    if ((iVar1 != 4) && (mem(iVar3 + 0x50) != 4)) {
      if (iVar1 == 0x11) {
        FUN_0044d220(iVar3);
        iVar3 = mem(iVar4 + 0x70) + 1;
        mem(iVar4 + 0x70) = iVar3;
        iVar1 = iVar3 / 100;
        if (shin::score_awards_life(iVar3)) {
          mem(iVar4 + 0x8c) = mem(iVar4 + 0x8c) + 1;
          iVar4 = ctx.sound(0x475200);
          return iVar4;
        }
      }
      else if ((((iVar1 == 2) || (iVar1 == 10)) || (iVar1 == 0xe)) && (0 < mem(iVar4 + 0x58)))
      {
        iVar1 = mem(iVar3 + 0x24) + mem(iVar3 + 8) + -0x14;
        if (mem(iVar4 + 0xc) + mem(iVar4 + 0x30) < iVar1) {
          mem(iVar4 + 0x58) = 0xfffffff1;
          FUN_0044d220(iVar3);
          iVar3 = mem(iVar4 + 0x70) + 1;
          mem(iVar4 + 0x70) = iVar3;
          iVar1 = iVar3 / 100;
          if (shin::score_awards_life(iVar3)) {
            mem(iVar4 + 0x8c) = mem(iVar4 + 0x8c) + 1;
            iVar4 = ctx.sound(0x475200);
            return iVar4;
          }
        }
        else if (mem(iVar4 + 0xb8) == 0) {
          FUN_0040d7f0(iVar4);
          iVar1 = mem(iVar4 + 0x10) / 2;
          if (mem(iVar4 + 8) + iVar1 < mem(iVar3 + 4) + mem(iVar3 + 100) / 2) {
            mem(iVar4 + 0x54) = 0xfffffff0;
            mem(iVar4 + 0x88) = 1;
          }
          else {
            mem(iVar4 + 0x54) = 0x10;
            mem(iVar4 + 0x88) = 0;
          }
          mem(iVar4 + 0x58) = 0xfffffffb;
          return iVar1;
        }
      }
      else if (mem(iVar4 + 0xb8) == 0) {
        FUN_0040d7f0(iVar4);
        iVar1 = mem(iVar4 + 0x10) / 2;
        if (mem(iVar4 + 8) + iVar1 < mem(iVar3 + 4) + mem(iVar3 + 100) / 2) {
          mem(iVar4 + 0x54) = 0xfffffff0;
          mem(iVar4 + 0x88) = 1;
        }
        else {
          mem(iVar4 + 0x54) = 0x10;
          mem(iVar4 + 0x88) = 0;
        }
        mem(iVar4 + 0x58) = 0xfffffffb;
        return iVar1;
      }
    }
    break;
  case 0x4a:
    iVar3 = 0x20000;
    iVar1 = mem(iVar4 + 0x5c);
    if ((iVar1 != 4) && (iVar2 = mem(iVar3 + 0x50), iVar2 != 7)) {
      if (iVar1 == 0x11) {
        if (iVar2 == 6) {
          iVar4 = FUN_0040d7f0(iVar4);
          return iVar4;
        }
        FUN_00438670(iVar3);
        iVar3 = mem(iVar4 + 0x70) + 1;
        mem(iVar4 + 0x70) = iVar3;
        iVar1 = iVar3 / 100;
        if (shin::score_awards_life(iVar3)) {
          mem(iVar4 + 0x8c) = mem(iVar4 + 0x8c) + 1;
          iVar4 = ctx.sound(0x475200);
          return iVar4;
        }
      }
      else if (iVar2 == 6) {
        if (mem(iVar4 + 0xb8) == 0) {
          FUN_0040d7f0(iVar4);
          iVar1 = mem(iVar4 + 0x10) / 2;
          if (mem(iVar4 + 8) + iVar1 < mem(iVar3 + 4) + mem(iVar3 + 100) / 2) {
            mem(iVar4 + 0x54) = 0xfffffff0;
            mem(iVar4 + 0x88) = 1;
          }
          else {
            mem(iVar4 + 0x54) = 0x10;
            mem(iVar4 + 0x88) = 0;
          }
          mem(iVar4 + 0x58) = 0xfffffffb;
          return iVar1;
        }
      }
      else if ((((iVar1 == 2) || (iVar1 == 10)) || (iVar1 == 0xe)) && (0 < mem(iVar4 + 0x58)))
      {
        iVar1 = mem(iVar3 + 0x24) + mem(iVar3 + 8) + -0x14;
        if (mem(iVar4 + 0xc) + mem(iVar4 + 0x30) < iVar1) {
          mem(iVar4 + 0x58) = 0xfffffff1;
          FUN_00438670(iVar3);
          iVar3 = mem(iVar4 + 0x70) + 1;
          mem(iVar4 + 0x70) = iVar3;
          iVar1 = iVar3 / 100;
          if (shin::score_awards_life(iVar3)) {
            mem(iVar4 + 0x8c) = mem(iVar4 + 0x8c) + 1;
            iVar4 = ctx.sound(0x475200);
            return iVar4;
          }
        }
        else if (mem(iVar4 + 0xb8) == 0) {
          FUN_0040d7f0(iVar4);
          iVar1 = mem(iVar4 + 0x10) / 2;
          if (mem(iVar4 + 8) + iVar1 < mem(iVar3 + 4) + mem(iVar3 + 100) / 2) {
            mem(iVar4 + 0x54) = 0xfffffff0;
            mem(iVar4 + 0x88) = 1;
          }
          else {
            mem(iVar4 + 0x54) = 0x10;
            mem(iVar4 + 0x88) = 0;
          }
          mem(iVar4 + 0x58) = 0xfffffffb;
          return iVar1;
        }
      }
      else if (mem(iVar4 + 0xb8) == 0) {
        FUN_0040d7f0(iVar4);
        iVar1 = mem(iVar4 + 0x10) / 2;
        if (mem(iVar4 + 8) + iVar1 < mem(iVar3 + 4) + mem(iVar3 + 100) / 2) {
          mem(iVar4 + 0x54) = 0xfffffff0;
          mem(iVar4 + 0x88) = 1;
        }
        else {
          mem(iVar4 + 0x54) = 0x10;
          mem(iVar4 + 0x88) = 0;
        }
        mem(iVar4 + 0x58) = 0xfffffffb;
        return iVar1;
      }
    }
    break;
  case 0x4b:
    iVar3 = 0x20000;
    iVar1 = mem(iVar4 + 0x5c);
    if ((iVar1 != 4) && (iVar2 = mem(iVar3 + 0x50), iVar2 != 4)) {
      if (iVar1 == 0x11) {
        if (iVar2 == 3) {
          iVar4 = FUN_0040d7f0(iVar4);
          return iVar4;
        }
        FUN_00422680(iVar3);
        iVar3 = mem(iVar4 + 0x70) + 1;
        mem(iVar4 + 0x70) = iVar3;
        iVar1 = iVar3 / 100;
        if (shin::score_awards_life(iVar3)) {
          mem(iVar4 + 0x8c) = mem(iVar4 + 0x8c) + 1;
          iVar4 = ctx.sound(0x475200);
          return iVar4;
        }
      }
      else if (iVar2 == 3) {
        if (mem(iVar4 + 0xb8) == 0) {
          FUN_0040d7f0(iVar4);
          iVar1 = mem(iVar3 + 0x6c) / 2;
          if (mem(iVar4 + 8) + mem(iVar4 + 0x10) / 2 < mem(iVar3 + 4) + iVar1) {
            mem(iVar4 + 0x54) = 0xfffffff0;
            mem(iVar4 + 0x88) = 1;
          }
          else {
            mem(iVar4 + 0x54) = 0x10;
            mem(iVar4 + 0x88) = 0;
          }
          mem(iVar4 + 0x58) = 0xfffffffb;
          return iVar1;
        }
      }
      else if ((((iVar1 == 2) || (iVar1 == 10)) || (iVar1 == 0xe)) && (0 < mem(iVar4 + 0x58)))
      {
        iVar1 = mem(iVar4 + 0xc) + mem(iVar4 + 0x30);
        if (iVar1 < mem(iVar3 + 0x24) + mem(iVar3 + 8) + -0x14) {
          mem(iVar4 + 0x58) = 0xfffffff1;
          FUN_00422680(iVar3);
          iVar3 = mem(iVar4 + 0x70) + 1;
          mem(iVar4 + 0x70) = iVar3;
          iVar1 = iVar3 / 100;
          if (shin::score_awards_life(iVar3)) {
            mem(iVar4 + 0x8c) = mem(iVar4 + 0x8c) + 1;
            iVar4 = ctx.sound(0x475200);
            return iVar4;
          }
        }
        else if (mem(iVar4 + 0xb8) == 0) {
          FUN_0040d7f0(iVar4);
          iVar1 = mem(iVar3 + 0x6c) / 2;
          if (mem(iVar4 + 8) + mem(iVar4 + 0x10) / 2 < mem(iVar3 + 4) + iVar1) {
            mem(iVar4 + 0x54) = 0xfffffff0;
            mem(iVar4 + 0x88) = 1;
          }
          else {
            mem(iVar4 + 0x54) = 0x10;
            mem(iVar4 + 0x88) = 0;
          }
          mem(iVar4 + 0x58) = 0xfffffffb;
          return iVar1;
        }
      }
      else if (mem(iVar4 + 0xb8) == 0) {
        FUN_0040d7f0(iVar4);
        iVar1 = mem(iVar3 + 0x6c) / 2;
        if (mem(iVar4 + 8) + mem(iVar4 + 0x10) / 2 < mem(iVar3 + 4) + iVar1) {
          mem(iVar4 + 0x54) = 0xfffffff0;
          mem(iVar4 + 0x88) = 1;
        }
        else {
          mem(iVar4 + 0x54) = 0x10;
          mem(iVar4 + 0x88) = 0;
        }
        mem(iVar4 + 0x58) = 0xfffffffb;
        return iVar1;
      }
    }
    break;
  case 0x4c:
    iVar3 = 0x20000;
    iVar1 = mem(iVar4 + 0x5c);
    if ((iVar1 != 4) && (mem(iVar3 + 0x50) != 4)) {
      if (iVar1 == 0x11) {
        FUN_00405360(iVar3);
        iVar3 = mem(iVar4 + 0x70) + 1;
        mem(iVar4 + 0x70) = iVar3;
        iVar1 = iVar3 / 100;
        if (shin::score_awards_life(iVar3)) {
          mem(iVar4 + 0x8c) = mem(iVar4 + 0x8c) + 1;
          iVar4 = ctx.sound(0x475200);
          return iVar4;
        }
      }
      else if ((((iVar1 == 2) || (iVar1 == 10)) || (iVar1 == 0xe)) && (0 < mem(iVar4 + 0x58)))
      {
        iVar1 = mem(iVar3 + 0x24) + mem(iVar3 + 8) + -0x14;
        if (mem(iVar4 + 0xc) + mem(iVar4 + 0x30) < iVar1) {
          mem(iVar4 + 0x58) = 0xfffffff1;
          FUN_00405360(iVar3);
          iVar3 = mem(iVar4 + 0x70) + 1;
          mem(iVar4 + 0x70) = iVar3;
          iVar1 = iVar3 / 100;
          if (shin::score_awards_life(iVar3)) {
            mem(iVar4 + 0x8c) = mem(iVar4 + 0x8c) + 1;
            iVar4 = ctx.sound(0x475200);
            return iVar4;
          }
        }
        else if (mem(iVar4 + 0xb8) == 0) {
          FUN_0040d7f0(iVar4);
          iVar1 = mem(iVar4 + 0x10) / 2;
          if (mem(iVar4 + 8) + iVar1 < mem(iVar3 + 4) + mem(iVar3 + 100) / 2) {
            mem(iVar4 + 0x54) = 0xfffffff0;
            mem(iVar4 + 0x88) = 1;
          }
          else {
            mem(iVar4 + 0x54) = 0x10;
            mem(iVar4 + 0x88) = 0;
          }
          mem(iVar4 + 0x58) = 0xfffffffb;
          return iVar1;
        }
      }
      else if (mem(iVar4 + 0xb8) == 0) {
        FUN_0040d7f0(iVar4);
        iVar1 = mem(iVar4 + 0x10) / 2;
        if (mem(iVar4 + 8) + iVar1 < mem(iVar3 + 4) + mem(iVar3 + 100) / 2) {
          mem(iVar4 + 0x54) = 0xfffffff0;
          mem(iVar4 + 0x88) = 1;
        }
        else {
          mem(iVar4 + 0x54) = 0x10;
          mem(iVar4 + 0x88) = 0;
        }
        mem(iVar4 + 0x58) = 0xfffffffb;
        return iVar1;
      }
    }
    break;
  case 0x4d:
    iVar3 = 0x20000;
    iVar1 = mem(iVar4 + 0x5c);
    if ((iVar1 != 4) && (mem(iVar3 + 0x50) != 4)) {
      if (iVar1 == 0x11) {
        FUN_00444ad0(iVar3);
        iVar3 = mem(iVar4 + 0x70) + 1;
        mem(iVar4 + 0x70) = iVar3;
        iVar1 = iVar3 / 100;
        if (shin::score_awards_life(iVar3)) {
          mem(iVar4 + 0x8c) = mem(iVar4 + 0x8c) + 1;
          iVar4 = ctx.sound(0x475200);
          return iVar4;
        }
      }
      else if ((((iVar1 == 2) || (iVar1 == 10)) || (iVar1 == 0xe)) && (0 < mem(iVar4 + 0x58)))
      {
        iVar1 = mem(iVar3 + 0x24) + mem(iVar3 + 8) + -0x14;
        if (mem(iVar4 + 0xc) + mem(iVar4 + 0x30) < iVar1) {
          mem(iVar4 + 0x58) = 0xfffffff1;
          FUN_00444ad0(iVar3);
          iVar3 = mem(iVar4 + 0x70) + 1;
          mem(iVar4 + 0x70) = iVar3;
          iVar1 = iVar3 / 100;
          if (shin::score_awards_life(iVar3)) {
            mem(iVar4 + 0x8c) = mem(iVar4 + 0x8c) + 1;
            iVar4 = ctx.sound(0x475200);
            return iVar4;
          }
        }
        else if (mem(iVar4 + 0xb8) == 0) {
          FUN_0040d7f0(iVar4);
          iVar1 = mem(iVar4 + 0x10) / 2;
          if (mem(iVar4 + 8) + iVar1 < mem(iVar3 + 4) + mem(iVar3 + 100) / 2) {
            mem(iVar4 + 0x54) = 0xfffffff0;
            mem(iVar4 + 0x88) = 1;
          }
          else {
            mem(iVar4 + 0x54) = 0x10;
            mem(iVar4 + 0x88) = 0;
          }
          mem(iVar4 + 0x58) = 0xfffffffb;
          return iVar1;
        }
      }
      else if (mem(iVar4 + 0xb8) == 0) {
        FUN_0040d7f0(iVar4);
        iVar1 = mem(iVar4 + 0x10) / 2;
        if (mem(iVar4 + 8) + iVar1 < mem(iVar3 + 4) + mem(iVar3 + 100) / 2) {
          mem(iVar4 + 0x54) = 0xfffffff0;
          mem(iVar4 + 0x88) = 1;
        }
        else {
          mem(iVar4 + 0x54) = 0x10;
          mem(iVar4 + 0x88) = 0;
        }
        mem(iVar4 + 0x58) = 0xfffffffb;
        return iVar1;
      }
    }
    break;
  case 0x4e:
    iVar3 = 0x20000;
    iVar2 = mem(iVar4 + 0x5c);
    if ((iVar2 != 4) && (iVar1 = mem(iVar3 + 0x50), iVar1 != 4)) {
      if (iVar2 == 0x11) {
        if (iVar1 == 3) {
          iVar4 = FUN_0040d7f0(iVar4);
          return iVar4;
        }
        FUN_0042ad00(iVar3);
        iVar3 = mem(iVar4 + 0x70) + 1;
        mem(iVar4 + 0x70) = iVar3;
        iVar1 = iVar3 / 100;
        if (shin::score_awards_life(iVar3)) {
          mem(iVar4 + 0x8c) = mem(iVar4 + 0x8c) + 1;
          iVar4 = ctx.sound(0x475200);
          return iVar4;
        }
      }
      else if (iVar1 == 3) {
        if (mem(iVar4 + 0xb8) == 0) {
          FUN_0040d7f0(iVar4);
          iVar1 = mem(iVar4 + 0x10) / 2;
          if (mem(iVar4 + 8) + iVar1 < mem(iVar3 + 4) + mem(iVar3 + 100) / 2) {
            mem(iVar4 + 0x54) = 0xfffffff0;
            mem(iVar4 + 0x88) = 1;
          }
          else {
            mem(iVar4 + 0x54) = 0x10;
            mem(iVar4 + 0x88) = 0;
          }
          mem(iVar4 + 0x58) = 0xfffffffb;
          return iVar1;
        }
      }
      else if ((((iVar2 == 2) || (iVar2 == 10)) || (iVar2 == 0xe)) && (0 < mem(iVar4 + 0x58)))
      {
        iVar1 = mem(iVar3 + 0x24) + mem(iVar3 + 8) + -0x14;
        if (mem(iVar4 + 0xc) + mem(iVar4 + 0x30) < iVar1) {
          mem(iVar4 + 0x58) = 0xfffffff1;
          FUN_0042ad00(iVar3);
          iVar3 = mem(iVar4 + 0x70) + 1;
          mem(iVar4 + 0x70) = iVar3;
          iVar1 = iVar3 / 100;
          if (shin::score_awards_life(iVar3)) {
            mem(iVar4 + 0x8c) = mem(iVar4 + 0x8c) + 1;
            iVar4 = ctx.sound(0x475200);
            return iVar4;
          }
        }
        else if (mem(iVar4 + 0xb8) == 0) {
          FUN_0040d7f0(iVar4);
          iVar1 = mem(iVar4 + 0x10) / 2;
          if (mem(iVar4 + 8) + iVar1 < mem(iVar3 + 4) + mem(iVar3 + 100) / 2) {
            mem(iVar4 + 0x54) = 0xfffffff0;
            mem(iVar4 + 0x88) = 1;
          }
          else {
            mem(iVar4 + 0x54) = 0x10;
            mem(iVar4 + 0x88) = 0;
          }
          mem(iVar4 + 0x58) = 0xfffffffb;
          return iVar1;
        }
      }
      else if (mem(iVar4 + 0xb8) == 0) {
        FUN_0040d7f0(iVar4);
        iVar1 = mem(iVar4 + 0x10) / 2;
        if (mem(iVar4 + 8) + iVar1 < mem(iVar3 + 4) + mem(iVar3 + 100) / 2) {
          mem(iVar4 + 0x54) = 0xfffffff0;
          mem(iVar4 + 0x88) = 1;
        }
        else {
          mem(iVar4 + 0x54) = 0x10;
          mem(iVar4 + 0x88) = 0;
        }
        mem(iVar4 + 0x58) = 0xfffffffb;
        return iVar1;
      }
    }
    break;
  case 0x4f:
    iVar3 = 0x20000;
    iVar1 = mem(iVar4 + 0x5c);
    if ((iVar1 != 4) && (mem(iVar3 + 0x58) != 3)) {
      if (iVar1 == 0x11) {
        FUN_0042ca50(iVar3);
        iVar3 = mem(iVar4 + 0x70) + 1;
        mem(iVar4 + 0x70) = iVar3;
        iVar1 = iVar3 / 100;
        if (shin::score_awards_life(iVar3)) {
          mem(iVar4 + 0x8c) = mem(iVar4 + 0x8c) + 1;
          iVar4 = ctx.sound(0x475200);
          return iVar4;
        }
      }
      else if ((((iVar1 == 2) || (iVar1 == 10)) || (iVar1 == 0xe)) && (0 < mem(iVar4 + 0x58)))
      {
        iVar1 = mem(iVar3 + 8) + mem(iVar3 + 0x2c) + -0x14;
        if (mem(iVar4 + 0xc) + mem(iVar4 + 0x30) < iVar1) {
          mem(iVar4 + 0x58) = 0xfffffff1;
          FUN_0042ca50(iVar3);
          iVar3 = mem(iVar4 + 0x70) + 1;
          mem(iVar4 + 0x70) = iVar3;
          iVar1 = iVar3 / 100;
          if (shin::score_awards_life(iVar3)) {
            mem(iVar4 + 0x8c) = mem(iVar4 + 0x8c) + 1;
            iVar4 = ctx.sound(0x475200);
            return iVar4;
          }
        }
        else if (mem(iVar4 + 0xb8) == 0) {
          FUN_0040d7f0(iVar4);
          iVar1 = mem(iVar4 + 0x10) / 2;
          if (mem(iVar4 + 8) + iVar1 < mem(iVar3 + 4) + mem(iVar3 + 0xc) / 2) {
            mem(iVar4 + 0x54) = 0xfffffff0;
            mem(iVar4 + 0x88) = 1;
          }
          else {
            mem(iVar4 + 0x54) = 0x10;
            mem(iVar4 + 0x88) = 0;
          }
          mem(iVar4 + 0x58) = 0xfffffffb;
          return iVar1;
        }
      }
      else if (mem(iVar4 + 0xb8) == 0) {
        FUN_0040d7f0(iVar4);
        iVar1 = mem(iVar4 + 0x10) / 2;
        if (mem(iVar4 + 8) + iVar1 < mem(iVar3 + 4) + mem(iVar3 + 0xc) / 2) {
          mem(iVar4 + 0x54) = 0xfffffff0;
          mem(iVar4 + 0x88) = 1;
        }
        else {
          mem(iVar4 + 0x54) = 0x10;
          mem(iVar4 + 0x88) = 0;
        }
        mem(iVar4 + 0x58) = 0xfffffffb;
        return iVar1;
      }
    }
    break;
  case 0x50:
    iVar3 = 0x20000;
    iVar1 = mem(iVar4 + 0x5c);
    if ((iVar1 != 4) && (mem(iVar3 + 0x58) != 3)) {
      if (iVar1 == 0x11) {
        FUN_00402fd0(iVar3);
        iVar3 = mem(iVar4 + 0x70) + 1;
        mem(iVar4 + 0x70) = iVar3;
        iVar1 = iVar3 / 100;
        if (shin::score_awards_life(iVar3)) {
          mem(iVar4 + 0x8c) = mem(iVar4 + 0x8c) + 1;
          iVar4 = ctx.sound(0x475200);
          return iVar4;
        }
      }
      else if ((((iVar1 == 2) || (iVar1 == 10)) || (iVar1 == 0xe)) && (0 < mem(iVar4 + 0x58)))
      {
        iVar1 = mem(iVar3 + 8) + mem(iVar3 + 0x2c) + -0x14;
        if (mem(iVar4 + 0xc) + mem(iVar4 + 0x30) < iVar1) {
          mem(iVar4 + 0x58) = 0xfffffff1;
          FUN_00402fd0(iVar3);
          iVar3 = mem(iVar4 + 0x70) + 1;
          mem(iVar4 + 0x70) = iVar3;
          iVar1 = iVar3 / 100;
          if (shin::score_awards_life(iVar3)) {
            mem(iVar4 + 0x8c) = mem(iVar4 + 0x8c) + 1;
            iVar4 = ctx.sound(0x475200);
            return iVar4;
          }
        }
        else if (mem(iVar4 + 0xb8) == 0) {
          FUN_0040d7f0(iVar4);
          iVar1 = mem(iVar4 + 0x10) / 2;
          if (mem(iVar4 + 8) + iVar1 < mem(iVar3 + 4) + mem(iVar3 + 0xc) / 2) {
            mem(iVar4 + 0x54) = 0xfffffff0;
            mem(iVar4 + 0x88) = 1;
          }
          else {
            mem(iVar4 + 0x54) = 0x10;
            mem(iVar4 + 0x88) = 0;
          }
          mem(iVar4 + 0x58) = 0xfffffffb;
          return iVar1;
        }
      }
      else if (mem(iVar4 + 0xb8) == 0) {
        FUN_0040d7f0(iVar4);
        iVar1 = mem(iVar4 + 0x10) / 2;
        if (mem(iVar4 + 8) + iVar1 < mem(iVar3 + 4) + mem(iVar3 + 0xc) / 2) {
          mem(iVar4 + 0x54) = 0xfffffff0;
          mem(iVar4 + 0x88) = 1;
        }
        else {
          mem(iVar4 + 0x54) = 0x10;
          mem(iVar4 + 0x88) = 0;
        }
        mem(iVar4 + 0x58) = 0xfffffffb;
        return iVar1;
      }
    }
    break;
  case 0x51:
    iVar3 = 0x20000;
    iVar1 = mem(iVar4 + 0x5c);
    if ((iVar1 != 4) && (mem(iVar3 + 0x58) != 3)) {
      if (iVar1 == 0x11) {
        FUN_0043b5e0(iVar3);
        iVar3 = mem(iVar4 + 0x70) + 1;
        mem(iVar4 + 0x70) = iVar3;
        iVar1 = iVar3 / 100;
        if (shin::score_awards_life(iVar3)) {
          mem(iVar4 + 0x8c) = mem(iVar4 + 0x8c) + 1;
          iVar4 = ctx.sound(0x475200);
          return iVar4;
        }
      }
      else if ((((iVar1 == 2) || (iVar1 == 10)) || (iVar1 == 0xe)) && (0 < mem(iVar4 + 0x58)))
      {
        iVar1 = mem(iVar3 + 8) + mem(iVar3 + 0x2c) + -0x14;
        if (mem(iVar4 + 0xc) + mem(iVar4 + 0x30) < iVar1) {
          mem(iVar4 + 0x58) = 0xfffffff1;
          FUN_0043b5e0(iVar3);
          iVar3 = mem(iVar4 + 0x70) + 1;
          mem(iVar4 + 0x70) = iVar3;
          iVar1 = iVar3 / 100;
          if (shin::score_awards_life(iVar3)) {
            mem(iVar4 + 0x8c) = mem(iVar4 + 0x8c) + 1;
            iVar4 = ctx.sound(0x475200);
            return iVar4;
          }
        }
        else if (mem(iVar4 + 0xb8) == 0) {
          FUN_0040d7f0(iVar4);
          iVar1 = mem(iVar4 + 0x10) / 2;
          if (mem(iVar4 + 8) + iVar1 < mem(iVar3 + 4) + mem(iVar3 + 0xc) / 2) {
            mem(iVar4 + 0x54) = 0xfffffff0;
            mem(iVar4 + 0x88) = 1;
          }
          else {
            mem(iVar4 + 0x54) = 0x10;
            mem(iVar4 + 0x88) = 0;
          }
          mem(iVar4 + 0x58) = 0xfffffffb;
          return iVar1;
        }
      }
      else if (mem(iVar4 + 0xb8) == 0) {
        FUN_0040d7f0(iVar4);
        iVar1 = mem(iVar4 + 0x10) / 2;
        if (mem(iVar4 + 8) + iVar1 < mem(iVar3 + 4) + mem(iVar3 + 0xc) / 2) {
          mem(iVar4 + 0x54) = 0xfffffff0;
          mem(iVar4 + 0x88) = 1;
        }
        else {
          mem(iVar4 + 0x54) = 0x10;
          mem(iVar4 + 0x88) = 0;
        }
        mem(iVar4 + 0x58) = 0xfffffffb;
        return iVar1;
      }
    }
    break;
  case 0x52:
    iVar3 = 0x20000;
    iVar1 = mem(iVar4 + 0x5c);
    if ((iVar1 != 4) && (mem(iVar3 + 0x58) != 3)) {
      if (iVar1 == 0x11) {
        FUN_0041e230(iVar3);
        iVar3 = mem(iVar4 + 0x70) + 1;
        mem(iVar4 + 0x70) = iVar3;
        iVar1 = iVar3 / 100;
        if (shin::score_awards_life(iVar3)) {
          mem(iVar4 + 0x8c) = mem(iVar4 + 0x8c) + 1;
          iVar4 = ctx.sound(0x475200);
          return iVar4;
        }
      }
      else if ((((iVar1 == 2) || (iVar1 == 10)) || (iVar1 == 0xe)) && (0 < mem(iVar4 + 0x58)))
      {
        iVar1 = mem(iVar3 + 8) + mem(iVar3 + 0x2c) + -0x14;
        if (mem(iVar4 + 0xc) + mem(iVar4 + 0x30) < iVar1) {
          mem(iVar4 + 0x58) = 0xfffffff1;
          FUN_0041e230(iVar3);
          iVar3 = mem(iVar4 + 0x70) + 1;
          mem(iVar4 + 0x70) = iVar3;
          iVar1 = iVar3 / 100;
          if (shin::score_awards_life(iVar3)) {
            mem(iVar4 + 0x8c) = mem(iVar4 + 0x8c) + 1;
            iVar4 = ctx.sound(0x475200);
            return iVar4;
          }
        }
        else if (mem(iVar4 + 0xb8) == 0) {
          FUN_0040d7f0(iVar4);
          iVar1 = mem(iVar4 + 0x10) / 2;
          if (mem(iVar4 + 8) + iVar1 < mem(iVar3 + 4) + mem(iVar3 + 0xc) / 2) {
            mem(iVar4 + 0x54) = 0xfffffff0;
            mem(iVar4 + 0x88) = 1;
          }
          else {
            mem(iVar4 + 0x54) = 0x10;
            mem(iVar4 + 0x88) = 0;
          }
          mem(iVar4 + 0x58) = 0xfffffffb;
          return iVar1;
        }
      }
      else if (mem(iVar4 + 0xb8) == 0) {
        FUN_0040d7f0(iVar4);
        iVar1 = mem(iVar4 + 0x10) / 2;
        if (mem(iVar4 + 8) + iVar1 < mem(iVar3 + 4) + mem(iVar3 + 0xc) / 2) {
          mem(iVar4 + 0x54) = 0xfffffff0;
          mem(iVar4 + 0x88) = 1;
        }
        else {
          mem(iVar4 + 0x54) = 0x10;
          mem(iVar4 + 0x88) = 0;
        }
        mem(iVar4 + 0x58) = 0xfffffffb;
        return iVar1;
      }
    }
    break;
  case 0x53:
    iVar3 = 0x20000;
    iVar2 = mem(iVar4 + 0x5c);
    if ((iVar2 != 4) && (iVar1 = mem(iVar3 + 0x58), iVar1 != 3)) {
      if (iVar2 == 0x11) {
        if (iVar1 == 4) {
          iVar4 = FUN_0040d7f0(iVar4);
          return iVar4;
        }
        FUN_00443610(iVar3);
        iVar3 = mem(iVar4 + 0x70) + 1;
        mem(iVar4 + 0x70) = iVar3;
        iVar1 = iVar3 / 100;
        if (shin::score_awards_life(iVar3)) {
          mem(iVar4 + 0x8c) = mem(iVar4 + 0x8c) + 1;
          iVar4 = ctx.sound(0x475200);
          return iVar4;
        }
      }
      else if (iVar1 == 4) {
        if (mem(iVar4 + 0xb8) == 0) {
          FUN_0040d7f0(iVar4);
          iVar1 = mem(iVar3 + 0xc) / 2;
          if (mem(iVar4 + 8) + mem(iVar4 + 0x10) / 2 < mem(iVar3 + 4) + iVar1) {
            mem(iVar4 + 0x54) = 0xfffffff0;
            mem(iVar4 + 0x88) = 1;
          }
          else {
            mem(iVar4 + 0x54) = 0x10;
            mem(iVar4 + 0x88) = 0;
          }
          mem(iVar4 + 0x58) = 0xfffffffb;
          return iVar1;
        }
      }
      else if ((((iVar2 == 2) || (iVar2 == 10)) || (iVar2 == 0xe)) && (0 < mem(iVar4 + 0x58)))
      {
        iVar1 = mem(iVar3 + 8) + mem(iVar3 + 0x2c) + -0x14;
        if (mem(iVar4 + 0xc) + mem(iVar4 + 0x30) < iVar1) {
          mem(iVar4 + 0x58) = 0xfffffff1;
          FUN_00443610(iVar3);
          iVar3 = mem(iVar4 + 0x70) + 1;
          mem(iVar4 + 0x70) = iVar3;
          iVar1 = iVar3 / 100;
          if (shin::score_awards_life(iVar3)) {
            mem(iVar4 + 0x8c) = mem(iVar4 + 0x8c) + 1;
            iVar4 = ctx.sound(0x475200);
            return iVar4;
          }
        }
        else if (mem(iVar4 + 0xb8) == 0) {
          FUN_0040d7f0(iVar4);
          iVar1 = mem(iVar3 + 0xc) / 2;
          if (mem(iVar4 + 8) + mem(iVar4 + 0x10) / 2 < mem(iVar3 + 4) + iVar1) {
            mem(iVar4 + 0x54) = 0xfffffff0;
            mem(iVar4 + 0x88) = 1;
          }
          else {
            mem(iVar4 + 0x54) = 0x10;
            mem(iVar4 + 0x88) = 0;
          }
          mem(iVar4 + 0x58) = 0xfffffffb;
          return iVar1;
        }
      }
      else if (mem(iVar4 + 0xb8) == 0) {
        FUN_0040d7f0(iVar4);
        iVar1 = mem(iVar3 + 0xc) / 2;
        if (mem(iVar4 + 8) + mem(iVar4 + 0x10) / 2 < mem(iVar3 + 4) + iVar1) {
          mem(iVar4 + 0x54) = 0xfffffff0;
          mem(iVar4 + 0x88) = 1;
        }
        else {
          mem(iVar4 + 0x54) = 0x10;
          mem(iVar4 + 0x88) = 0;
        }
        mem(iVar4 + 0x58) = 0xfffffffb;
        return iVar1;
      }
    }
    break;
  case 0x55:
    iVar3 = 0x20000;
    iVar1 = mem(iVar4 + 0x5c);
    if ((iVar1 != 4) && (mem(iVar3 + 0x58) != 4)) {
      if (iVar1 == 0x11) {
        FUN_0041dab0(iVar3);
        iVar3 = mem(iVar4 + 0x70) + 1;
        mem(iVar4 + 0x70) = iVar3;
        iVar1 = iVar3 / 100;
        if (shin::score_awards_life(iVar3)) {
          mem(iVar4 + 0x8c) = mem(iVar4 + 0x8c) + 1;
          iVar4 = ctx.sound(0x475200);
          return iVar4;
        }
      }
      else if ((((iVar1 == 2) || (iVar1 == 10)) || (iVar1 == 0xe)) && (0 < mem(iVar4 + 0x58)))
      {
        iVar1 = mem(iVar3 + 8) + mem(iVar3 + 0x2c) + -0x14;
        if (mem(iVar4 + 0xc) + mem(iVar4 + 0x30) < iVar1) {
          mem(iVar4 + 0x58) = 0xfffffff1;
          FUN_0041dab0(iVar3);
          iVar3 = mem(iVar4 + 0x70) + 1;
          mem(iVar4 + 0x70) = iVar3;
          iVar1 = iVar3 / 100;
          if (shin::score_awards_life(iVar3)) {
            mem(iVar4 + 0x8c) = mem(iVar4 + 0x8c) + 1;
            iVar4 = ctx.sound(0x475200);
            return iVar4;
          }
        }
        else if (mem(iVar4 + 0xb8) == 0) {
          FUN_0040d7f0(iVar4);
          iVar1 = mem(iVar4 + 0x10) / 2;
          if (mem(iVar4 + 8) + iVar1 < mem(iVar3 + 4) + mem(iVar3 + 0xc) / 2) {
            mem(iVar4 + 0x54) = 0xfffffff0;
            mem(iVar4 + 0x88) = 1;
          }
          else {
            mem(iVar4 + 0x54) = 0x10;
            mem(iVar4 + 0x88) = 0;
          }
          mem(iVar4 + 0x58) = 0xfffffffb;
          return iVar1;
        }
      }
      else if (mem(iVar4 + 0xb8) == 0) {
        FUN_0040d7f0(iVar4);
        iVar1 = mem(iVar4 + 0x10) / 2;
        if (mem(iVar4 + 8) + iVar1 < mem(iVar3 + 4) + mem(iVar3 + 0xc) / 2) {
          mem(iVar4 + 0x54) = 0xfffffff0;
          mem(iVar4 + 0x88) = 1;
        }
        else {
          mem(iVar4 + 0x54) = 0x10;
          mem(iVar4 + 0x88) = 0;
        }
        mem(iVar4 + 0x58) = 0xfffffffb;
        return iVar1;
      }
    }
    break;
  case 0x56:
    iVar3 = 0x20000;
    iVar1 = mem(iVar4 + 0x5c);
    if ((iVar1 != 4) && (mem(iVar3 + 0x58) != 3)) {
      if (iVar1 == 0x11) {
        FUN_00420ea0(iVar3);
        iVar3 = mem(iVar4 + 0x70) + 1;
        mem(iVar4 + 0x70) = iVar3;
        iVar1 = iVar3 / 100;
        if (shin::score_awards_life(iVar3)) {
          mem(iVar4 + 0x8c) = mem(iVar4 + 0x8c) + 1;
          iVar4 = ctx.sound(0x475200);
          return iVar4;
        }
      }
      else if ((((iVar1 == 2) || (iVar1 == 10)) || (iVar1 == 0xe)) && (0 < mem(iVar4 + 0x58)))
      {
        iVar1 = mem(iVar3 + 8) + mem(iVar3 + 0x2c) + -0x14;
        if (mem(iVar4 + 0xc) + mem(iVar4 + 0x30) < iVar1) {
          mem(iVar4 + 0x58) = 0xfffffff1;
          FUN_00420ea0(iVar3);
          iVar3 = mem(iVar4 + 0x70) + 1;
          mem(iVar4 + 0x70) = iVar3;
          iVar1 = iVar3 / 100;
          if (shin::score_awards_life(iVar3)) {
            mem(iVar4 + 0x8c) = mem(iVar4 + 0x8c) + 1;
            iVar4 = ctx.sound(0x475200);
            return iVar4;
          }
        }
        else if (mem(iVar4 + 0xb8) == 0) {
          FUN_0040d7f0(iVar4);
          iVar1 = mem(iVar4 + 0x10) / 2;
          if (mem(iVar4 + 8) + iVar1 < mem(iVar3 + 4) + mem(iVar3 + 0xc) / 2) {
            mem(iVar4 + 0x54) = 0xfffffff0;
            mem(iVar4 + 0x88) = 1;
          }
          else {
            mem(iVar4 + 0x54) = 0x10;
            mem(iVar4 + 0x88) = 0;
          }
          mem(iVar4 + 0x58) = 0xfffffffb;
          return iVar1;
        }
      }
      else if (mem(iVar4 + 0xb8) == 0) {
        FUN_0040d7f0(iVar4);
        iVar1 = mem(iVar4 + 0x10) / 2;
        if (mem(iVar4 + 8) + iVar1 < mem(iVar3 + 4) + mem(iVar3 + 0xc) / 2) {
          mem(iVar4 + 0x54) = 0xfffffff0;
          mem(iVar4 + 0x88) = 1;
        }
        else {
          mem(iVar4 + 0x54) = 0x10;
          mem(iVar4 + 0x88) = 0;
        }
        mem(iVar4 + 0x58) = 0xfffffffb;
      }
    }
  }
  return iVar1;
}


#undef FUN_004012e0
#undef FUN_00401d10
#undef FUN_00402fd0
#undef FUN_00403720
#undef FUN_00403e50
#undef FUN_00405360
#undef FUN_0040d7f0
#undef FUN_0040d890
#undef FUN_0040d950
#undef FUN_00414820
#undef FUN_00415ab0
#undef FUN_0041a4e0
#undef FUN_0041b4b0
#undef FUN_0041ba30
#undef FUN_0041c810
#undef FUN_0041dab0
#undef FUN_0041e230
#undef FUN_0041f7f0
#undef FUN_00420660
#undef FUN_00420ea0
#undef FUN_00422680
#undef FUN_004242a0
#undef FUN_004245a0
#undef FUN_004286e0
#undef FUN_00428b30
#undef FUN_0042ad00
#undef FUN_0042c4c0
#undef FUN_0042ca50
#undef FUN_0042ebb0
#undef FUN_00432f90
#undef FUN_00434ad0
#undef FUN_00434f90
#undef FUN_00437860
#undef FUN_00438670
#undef FUN_004393d0
#undef FUN_0043aa70
#undef FUN_0043b5e0
#undef FUN_0043c0a0
#undef FUN_0043cd20
#undef FUN_0043d120
#undef FUN_0043e450
#undef FUN_00440a10
#undef FUN_00441290
#undef FUN_00442a20
#undef FUN_00443610
#undef FUN_00444ad0
#undef FUN_004463b0
#undef FUN_00447f70
#undef FUN_0044c610
#undef FUN_0044d220
#undef FUN_0044de50

}
}
