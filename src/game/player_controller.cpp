#include "player_controller.hpp"
namespace shin {
static std::int32_t invoke(std::uint32_t id,PlayerPayload& p,PlayerContext& ctx,std::initializer_list<std::int64_t> args){
 switch(id){
case 0x40d730: p.at(0x5c) = 0;
  p.at(0x1c) = 0;
  p.at(0x18) = 0x0045c714;
  p.at(0x24) = 1;
  p.at(0x28) = 100;
  p.at(0x20) = 0; return 0;
case 0x40d760: p.at(0x1c) = 0;
  p.at(0x20) = 0;
  p.at(0x5c) = 1;
  p.at(0x18) = 0x0045c718;
  p.at(0x24) = 4;
  p.at(0x28) = 5; return 0;
case 0x40d790: p.at(0x5c) = 2;
  p.at(0x1c) = 0;
  p.at(0x20) = 0;
  p.at(0x18) = 0x0045c728;
  p.at(0x28) = 10;
  p.at(0x24) = 2; return 0;
case 0x40d7c0: p.at(0x5c) = 5;
  p.at(0x1c) = 0;
  p.at(0x20) = 0;
  p.at(0x18) = 0x0045c730;
  p.at(0x24) = 1;
  p.at(0x28) = 5; return 0;
case 0x40d830: p.at(0x1c) = 0;
  p.at(0x20) = 0;
  p.at(0x5c) = 6;
  p.at(0x18) = 0x0045c738;
  p.at(0x24) = 2;
  p.at(0x28) = 5; return 0;
case 0x40d860: p.at(0x1c) = 0;
  p.at(0x20) = 0;
  p.at(0x5c) = 0xb;
  p.at(0x18) = 0x0045c770;
  p.at(0x24) = 2;
  p.at(0x28) = 5; return 0;
case 0x40d890: p.at(0x1c) = 0;
  p.at(0x20) = 0;
  p.at(0x5c) = 7;
  p.at(0x18) = 0x0045c748;
  p.at(0x24) = 1;
  p.at(0x28) = 5; return 0;
case 0x40d8c0: p.at(0x1c) = 0;
  p.at(0x20) = 0;
  p.at(0x5c) = 8;
  p.at(0x18) = 0x0045c768;
  p.at(0x24) = 1;
  p.at(0x28) = 5; return 0;
case 0x40d8f0: p.at(0x1c) = 0;
  p.at(0x20) = 0;
  p.at(0x5c) = 9;
  p.at(0x18) = 0x0045c750;
  p.at(0x24) = 4;
  p.at(0x28) = 5; return 0;
case 0x40d920: p.at(0x1c) = 0;
  p.at(0x20) = 0;
  p.at(0x5c) = 10;
  p.at(0x68) = 0;
  p.at(0x6c) = 0;
  p.at(0x18) = 0x0045c760;
  p.at(0x24) = 2;
  p.at(0x28) = 5; return 0;
case 0x40d950: p.at(0x1c) = 0;
  p.at(0x20) = 0;
  p.at(0x5c) = 0xc;
  p.at(0x18) = 0x0045c778;
  p.at(0x24) = 1;
  p.at(0x28) = 100; return 0;
case 0x40d980: p.at(0x1c) = 0;
  p.at(0x20) = 0;
  p.at(0x5c) = 0xd;
  p.at(0x18) = 0x0045c780;
  p.at(0x24) = 4;
  p.at(0x28) = 5; return 0;
case 0x40d9b0: p.at(0x1c) = 0;
  p.at(0x20) = 0;
  p.at(0x5c) = 0xe;
  p.at(0x18) = 0x0045c790;
  p.at(0x24) = 2;
  p.at(0x28) = 10; return 0;
case 0x40d9e0: p.at(0x1c) = 0;
  p.at(0x20) = 0;
  p.at(0x5c) = 0xf;
  p.at(0x18) = 0x0045c798;
  p.at(0x24) = 1;
  p.at(0x28) = 5; return 0;
case 0x40da10: p.at(0x1c) = 0;
  p.at(0x20) = 0;
  p.at(0x5c) = 0x10;
  p.at(0x18) = 0x0045c79c;
  p.at(0x24) = 1;
  p.at(0x28) = 5; return 0;
case 0x40da40: p.at(0x1c) = 0;
  p.at(0x20) = 0;
  p.at(0x5c) = 0x5e;
  p.at(0x18) = 0x0045c7a0;
  p.at(0x24) = 2;
  p.at(0x28) = 5; return 0;
case 0x40da70: p.at(0x1c) = 0;
  p.at(0x20) = 0;
  p.at(0x5c) = 0x11;
  p.at(0x18) = 0x0045c7a8;
  p.at(0x24) = 4;
  p.at(0x28) = 5; return 0;
case 0x40daa0: p.at(0x1c) = 0;
  p.at(0x20) = 0;
  p.at(0x5c) = 0x12;
  p.at(0x18) = 0x0045c7b8;
  p.at(0x24) = 2;
  p.at(0x28) = 10; return 0;
default:return ctx.call(id,p,args);
 }
}
void update_player(PlayerPayload& p,PlayerContext& ctx){
 std::int32_t iVar1=0,iVar2=0,iVar4=0,uVar7=0;

  
                    

  if (ctx.camera_y() + 0x230 < p.at(0xc)) {
    p.at(0xc) = ctx.camera_y() + 0x26c;
  }
  p.at(0x4c) = p.at(8);
  p.at(0x50) = p.at(0xc);
  switch(p.at(0x5c)) {
  case 0:
    if (p.at(0x20) == 99) {
      invoke(0x0040d830,p,ctx,{0});
    }
    if ((ctx.global(0x0046d4b8) != 0) || (ctx.global(0x0046d4bc) != 0)) {
      invoke(0x0040d760,p,ctx,{0});
    }
    if (ctx.global(0x0046d4b0) == 0) {
      p.at(0x94) = 0;
    }
    else if (p.at(0x94) == 0) {
      invoke(0x0040d790,p,ctx,{0});
      p.at(0x58) = -15;
      p.at(0x94) = 1;
    }
    iVar2 = invoke(0x00431d60,p,ctx,{0x2c,p.at(8),p.at(0xc) + 4});
    if ((iVar2 == 0) && (p.at(100) == 0)) {
      invoke(0x0040d790,p,ctx,{0});
      p.at(0x58) = 4;
    }
    if (ctx.global(0x0046d4e8) == 0) {
      p.at(0xe8) = 0;
    }
    else if (p.at(0xe8) == 0) {
      if (ctx.global(0x0046cd04) == 0) {
        if (0 < p.at(0xe0)) {
          if (p.at(0x88) == 0) {
            invoke(0x0042d640,p,ctx,{0x41,0x0042d730,0x0042d7c0,6,p.at(8),
                         p.at(0xc),-10});
            p.at(0xe0) = p.at(0xe0) + -1;
          }
          else {
            invoke(0x0042d640,p,ctx,{0x41,0x0042d730,0x0042d7c0,6,p.at(8),
                         p.at(0xc),10});
            p.at(0xe0) = p.at(0xe0) + -1;
          }
        }
      }
      else if ((ctx.global(0x0046cd04) == 1) && (0 < p.at(0xe4))) {
        if (p.at(0x88) == 0) {
          uVar7 = -10;
        }
        else {
          uVar7 = 10;
        }
        invoke(0x0042fc60,p,ctx,{0x42,0x0042fd50,0x0042fde0,6,p.at(8),
                     p.at(0xc),uVar7});
        p.at(0xe4) = p.at(0xe4) + -1;
      }
      p.at(0xe8) = 1;
    }
    break;
  case 1:
    if (p.at(0x90) == 0) {
      p.at(0x90) = 1;
    }
    if (((ctx.global(0x0046d4b8) == 0) && (ctx.global(0x0046d4bc) == 0)) && (p.at(0x94) == 0)) {
      invoke(0x0040d730,p,ctx,{0});
    }
    if (ctx.global(0x0046d4b0) == 0) {
      p.at(0x94) = 0;
    }
    else if (p.at(0x94) == 0) {
      invoke(0x0040d790,p,ctx,{0});
      p.at(0x58) = -15;
      p.at(0x94) = 1;
    }
    iVar2 = invoke(0x00431d60,p,ctx,{0x2c,p.at(8),p.at(0xc) + 4});
    if ((iVar2 == 0) && (p.at(100) == 0)) {
      invoke(0x0040d790,p,ctx,{0});
      p.at(0x58) = 4;
    }
    if (ctx.global(0x0046d4b8) != 0) {
      p.at(0x54) = -8;
      p.at(0x88) = 0;
    }
    if (ctx.global(0x0046d4bc) != 0) {
      p.at(0x54) = 8;
      p.at(0x88) = 1;
    }
    if (ctx.global(0x0046d4e8) == 0) {
      p.at(0xe8) = 0;
    }
    else if (p.at(0xe8) == 0) {
      if (ctx.global(0x0046cd04) == 0) {
        if (0 < p.at(0xe0)) {
          if (p.at(0x88) == 0) {
            invoke(0x0042d640,p,ctx,{0x41,0x0042d730,0x0042d7c0,6,p.at(8),
                         p.at(0xc),-10});
            p.at(0xe0) = p.at(0xe0) + -1;
          }
          else {
            invoke(0x0042d640,p,ctx,{0x41,0x0042d730,0x0042d7c0,6,p.at(8),
                         p.at(0xc),10});
            p.at(0xe0) = p.at(0xe0) + -1;
          }
        }
      }
      else if ((ctx.global(0x0046cd04) == 1) && (0 < p.at(0xe4))) {
        if (p.at(0x88) == 0) {
          uVar7 = -10;
        }
        else {
          uVar7 = 10;
        }
        invoke(0x0042fc60,p,ctx,{0x42,0x0042fd50,0x0042fde0,6,p.at(8),
                     p.at(0xc),uVar7});
        p.at(0xe4) = p.at(0xe4) + -1;
      }
      p.at(0xe8) = 1;
    }
    break;
  case 2:
    p.at(0x20) = 0;
    if (p.at(0x58) < 0) {
      p.at(0x18) = 0x0045c728;
      p.at(0x1c) = 0;
    }
    else if (0 < p.at(0x58)) {
      p.at(0x18) = 0x0045c728;
      p.at(0x1c) = 1;
    }
    if ((p.at(0x58) < 1) ||
       ((iVar2 = invoke(0x00431d60,p,ctx,{0x2c,p.at(8),p.at(0xc) + 4}),
        iVar2 == 0 && (p.at(100) != 1)))) {
      if (ctx.global(0x0046d4b8) != 0) {
        p.at(0x54) = -8;
        p.at(0x88) = 0;
      }
      if (ctx.global(0x0046d4bc) != 0) {
        p.at(0x54) = 8;
        p.at(0x88) = 1;
      }
    }
    else {
      invoke(0x0040d7c0,p,ctx,{0});
      p.at(0x58) = 0;
    }
    break;
  case 4:
    if ((p.at(0x1c) == 1) && (p.at(0x20) == 4)) {
      invoke(0x0040d730,p,ctx,{0});
      p.at(0x8c) = p.at(0x8c) + -1;
      p.grant_damage_immunity();
      if (p.at(0x8c) == 0) {
        p.owner_state = 2;
        iVar4 = 0;
        do {
          invoke(0x0044d440,p,ctx,{iVar4});
          iVar4 = iVar4 + 1;
        } while (iVar4 < 10);
        invoke(0x0044d790,p,ctx,{0x2d,0x0044d800,0x0044d8e0,5});
        return;
      }
      if (p.at(0xb4) == 1) {
        invoke(0x0044d440,p,ctx,{6});
        invoke(0x0044d440,p,ctx,{7});
        ctx.global(0x0046142c) = ctx.global(0x00461428);
        invoke(0x00427900,p,ctx,{});
        p.at(8) = ctx.global(0x0077bf4c) << 5;
        p.at(0xc) = ctx.global(0x0077bf50) << 5;
        p.at(0xb4) = 0;
        p.at(0x100) = 0;
        return;
      }
    }
    break;
  case 5:
    if (p.at(0x20) == 5) {
      invoke(0x0040d730,p,ctx,{0});
    }
    else {
      if (ctx.global(0x0046d4b0) == 0) {
        p.at(0x94) = 0;
      }
      else if (p.at(0x94) == 0) {
        invoke(0x0040d790,p,ctx,{0});
        p.at(0x58) = -15;
        p.at(0x94) = 1;
      }
      iVar2 = invoke(0x00431d60,p,ctx,{0x2c,p.at(8),p.at(0xc) + 4});
      if ((iVar2 == 0) && (p.at(100) == 0)) {
        invoke(0x0040d790,p,ctx,{0});
        p.at(0x58) = 4;
      }
      if (ctx.global(0x0046d4b8) != 0) {
        p.at(0x54) = -8;
        p.at(0x88) = 0;
      }
      if (ctx.global(0x0046d4bc) != 0) {
        p.at(0x54) = 8;
        p.at(0x88) = 1;
      }
    }
    break;
  case 6:
    if ((ctx.global(0x0046d4b8) != 0) || (ctx.global(0x0046d4bc) != 0)) {
      invoke(0x0040d760,p,ctx,{0});
    }
    if (p.at(0x94) != 0) {
      invoke(0x0040d790,p,ctx,{0});
      p.at(0x58) = -15;
    }
    iVar2 = invoke(0x00431d60,p,ctx,{0x2c,p.at(8),p.at(0xc) + 4});
    if ((iVar2 == 0) && (p.at(100) == 0)) {
      invoke(0x0040d790,p,ctx,{0});
      p.at(0x58) = 4;
    }
    if ((p.at(0x1c) == 1) && (p.at(0x20) == 4)) {
      invoke(0x0040d860,p,ctx,{0});
    }
    if (ctx.global(0x0046d4e8) == 0) {
      p.at(0xe8) = 0;
    }
    else if (p.at(0xe8) == 0) {
      if (ctx.global(0x0046cd04) == 0) {
        if (0 < p.at(0xe0)) {
          if (p.at(0x88) == 0) {
            invoke(0x0042d640,p,ctx,{0x41,0x0042d730,0x0042d7c0,6,p.at(8),
                         p.at(0xc),-10});
            p.at(0xe0) = p.at(0xe0) + -1;
          }
          else {
            invoke(0x0042d640,p,ctx,{0x41,0x0042d730,0x0042d7c0,6,p.at(8),
                         p.at(0xc),10});
            p.at(0xe0) = p.at(0xe0) + -1;
          }
        }
      }
      else if ((ctx.global(0x0046cd04) == 1) && (0 < p.at(0xe4))) {
        if (p.at(0x88) == 0) {
          uVar7 = -10;
        }
        else {
          uVar7 = 10;
        }
        invoke(0x0042fc60,p,ctx,{0x42,0x0042fd50,0x0042fde0,6,p.at(8),
                     p.at(0xc),uVar7});
        p.at(0xe4) = p.at(0xe4) + -1;
      }
      p.at(0xe8) = 1;
    }
    break;
  case 7:
    if ((ctx.global(0x0046d4b8) != 0) || (ctx.global(0x0046d4bc) != 0)) {
      invoke(0x0040d8f0,p,ctx,{0});
    }
    if (ctx.global(0x0046d4b0) == 0) {
      p.at(0x94) = 0;
    }
    else if (p.at(0x94) == 0) {
      invoke(0x0040d920,p,ctx,{0});
      p.at(0x58) = -15;
      p.at(0x94) = 1;
    }
    iVar2 = invoke(0x00431d60,p,ctx,{0x2c,p.at(8),p.at(0xc) + 4});
    if ((iVar2 != 0) || (p.at(100) != 0)) break;
    invoke(0x0040d920,p,ctx,{0});
    goto LAB_00406cc2;
  case 8:
    if (p.at(0xc0) == 1) {
      invoke(0x00446c30,p,ctx,{ctx.global(0x004762a4)});
      p.at(0xc0) = 0;
    }
    if (p.at(0x20) == 5) {
      invoke(0x0040d890,p,ctx,{0});
    }
    else {
      if (ctx.global(0x0046d4b0) == 0) {
        p.at(0x94) = 0;
      }
      else if (p.at(0x94) == 0) {
        invoke(0x0040d920,p,ctx,{0});
        p.at(0x58) = -15;
        p.at(0x94) = 1;
      }
      iVar2 = invoke(0x00431d60,p,ctx,{0x2c,p.at(8),p.at(0xc) + 4});
      if ((iVar2 == 0) && (p.at(100) == 0)) {
        invoke(0x0040d920,p,ctx,{0});
        p.at(0x58) = 4;
      }
      if (ctx.global(0x0046d4b8) != 0) {
        p.at(0x54) = -8;
        p.at(0x88) = 0;
      }
      if (ctx.global(0x0046d4bc) != 0) {
        p.at(0x54) = 8;
        p.at(0x88) = 1;
      }
    }
    break;
  case 9:
    if (ctx.global(0x0046d4b8) == 0) {
      if ((ctx.global(0x0046d4bc) == 0) && (p.at(0x94) == 0)) {
        invoke(0x0040d890,p,ctx,{0});
      }
      if (ctx.global(0x0046d4b8) != 0) goto LAB_00406591;
    }
    else {
LAB_00406591:
      p.at(0x54) = -8;
      p.at(0x88) = 0;
    }
    if (ctx.global(0x0046d4bc) != 0) {
      p.at(0x54) = 8;
      p.at(0x88) = 1;
    }
    if (ctx.global(0x0046d4b0) == 0) {
      p.at(0x94) = 0;
    }
    else if (p.at(0x94) == 0) {
      invoke(0x0040d920,p,ctx,{0});
      p.at(0x58) = -15;
      p.at(0x94) = 1;
    }
    iVar2 = invoke(0x00431d60,p,ctx,{0x2c,p.at(8),p.at(0xc) + 4});
    if ((iVar2 != 0) || (p.at(100) != 0)) break;
    invoke(0x0040d920,p,ctx,{0});
    goto LAB_00406cc2;
  case 10:
    iVar2 = p.at(0x6c) + 1;
    p.at(0x6c) = iVar2;
    if (ctx.global(0x0046d4e8) == 0) {
      p.at(0x68) = 0;
    }
    else if (p.at(0x68) == 0) {
      iVar2 = iVar2 / 0x28 + -8;
      p.at(0x58) = iVar2;
      if (0 < iVar2) {
        p.at(0x58) = 0;
      }
      p.at(0x68) = 1;
    }
    if (0 < p.at(0x58)) {
      iVar2 = 0x2c;
      iVar1 = invoke(0x00431d60,p,ctx,{iVar2,p.at(8),p.at(0xc) + 4});
      if ((((iVar1 != 0) &&
           (iVar1 = invoke(0x00431f90,p,ctx,{iVar2,p.at(8),p.at(0xc)}),
           iVar1 == 0)) &&
          (iVar2 = invoke(0x004320c0,p,ctx,{iVar2,p.at(8),p.at(0xc)}),
          iVar2 == 0)) || (p.at(100) == 1)) {
        invoke(0x0040d8c0,p,ctx,{0});
        break;
      }
    }
    if (ctx.global(0x0046d4b8) != 0) {
      p.at(0x54) = -8;
      p.at(0x88) = 0;
    }
    if (ctx.global(0x0046d4bc) != 0) {
      p.at(0x54) = 8;
      p.at(0x88) = 1;
    }
    break;
  case 0xb:
    if ((ctx.global(0x0046d4b8) != 0) || (ctx.global(0x0046d4bc) != 0)) {
      invoke(0x0040d760,p,ctx,{0});
    }
    if (p.at(0x94) != 0) {
      invoke(0x0040d790,p,ctx,{0});
      p.at(0x58) = -15;
    }
    iVar2 = invoke(0x00431d60,p,ctx,{0x2c,p.at(8),p.at(0xc) + 4});
    if ((iVar2 == 0) && (p.at(100) == 0)) {
      invoke(0x0040d790,p,ctx,{0});
      p.at(0x58) = 4;
    }
    if ((p.at(0x1c) == 1) && (p.at(0x20) == 4)) {
      invoke(0x0040d730,p,ctx,{0});
    }
    if (ctx.global(0x0046d4e8) == 0) {
      p.at(0xe8) = 0;
    }
    else {
      if (p.at(0xe8) == 0) {
        if ((ctx.global(0x0046cd04) == 0) && (0 < p.at(0xe0))) {
          if (p.at(0x88) == 0) {
            invoke(0x0042d640,p,ctx,{0x41,0x0042d730,0x0042d7c0,6,p.at(8),
                         p.at(0xc),-10});
            p.at(0xe0) = p.at(0xe0) + -1;
          }
          else {
            invoke(0x0042d640,p,ctx,{0x41,0x0042d730,0x0042d7c0,6,p.at(8),
                         p.at(0xc),10});
            p.at(0xe0) = p.at(0xe0) + -1;
          }
        }
      }
      else if ((ctx.global(0x0046cd04) == 1) && (0 < p.at(0xe4))) {
        if (p.at(0x88) == 0) {
          uVar7 = -10;
        }
        else {
          uVar7 = 10;
        }
        invoke(0x0042fc60,p,ctx,{0x42,0x0042fd50,0x0042fde0,6,p.at(8),
                     p.at(0xc),uVar7});
        p.at(0xe4) = p.at(0xe4) + -1;
      }
      p.at(0xe8) = 1;
    }
    break;
  case 0xc:
    if (p.at(0x20) == 99) {
      invoke(0x0040da10,p,ctx,{0});
    }
    if ((ctx.global(0x0046d4b8) != 0) || (ctx.global(0x0046d4bc) != 0)) {
      invoke(0x0040d980,p,ctx,{0});
    }
    if (ctx.global(0x0046d4b0) == 0) {
      p.at(0x94) = 0;
    }
    else if (p.at(0x94) == 0) {
      invoke(0x0040d9b0,p,ctx,{0});
      p.at(0x58) = -15;
      p.at(0x94) = 1;
    }
    iVar2 = invoke(0x00431d60,p,ctx,{0x2c,p.at(8),p.at(0xc) + 4});
    if ((iVar2 == 0) && (p.at(100) == 0)) {
      invoke(0x0040d9b0,p,ctx,{0});
      p.at(0x58) = 4;
    }
    if (ctx.global(0x0046d4e8) != 0) {
      invoke(0x0040da70,p,ctx,{0});
    }
    break;
  case 0xd:
    if (((ctx.global(0x0046d4b8) == 0) && (ctx.global(0x0046d4bc) == 0)) && (p.at(0x94) == 0)) {
      invoke(0x0040d950,p,ctx,{0});
    }
    if (ctx.global(0x0046d4b0) == 0) {
      p.at(0x94) = 0;
    }
    else if (p.at(0x94) == 0) {
      invoke(0x0040d9b0,p,ctx,{0});
      p.at(0x58) = -15;
      p.at(0x94) = 1;
    }
    iVar2 = invoke(0x00431d60,p,ctx,{0x2c,p.at(8),p.at(0xc) + 4});
    if ((iVar2 == 0) && (p.at(100) == 0)) {
      invoke(0x0040d9b0,p,ctx,{0});
      p.at(0x58) = 4;
    }
    if (ctx.global(0x0046d4b8) != 0) {
      p.at(0x54) = -8;
      p.at(0x88) = 0;
    }
    if (ctx.global(0x0046d4bc) != 0) {
      p.at(0x54) = 8;
      p.at(0x88) = 1;
    }
    if (ctx.global(0x0046d4e8) != 0) {
      invoke(0x0040da70,p,ctx,{0});
    }
    break;
  case 0xe:
    p.at(0x20) = 0;
    if (p.at(0x58) < 0) {
      p.at(0x18) = 0x0045c790;
      p.at(0x1c) = 0;
    }
    else if (0 < p.at(0x58)) {
      p.at(0x18) = 0x0045c790;
      p.at(0x1c) = 1;
    }
    if (0 < p.at(0x58)) {
      iVar2 = 0x2c;
      iVar1 = invoke(0x00431d60,p,ctx,{iVar2,p.at(8),p.at(0xc) + 4});
      if ((((iVar1 != 0) &&
           (iVar1 = invoke(0x00431f90,p,ctx,{iVar2,p.at(8),p.at(0xc)}),
           iVar1 == 0)) &&
          (iVar2 = invoke(0x004320c0,p,ctx,{iVar2,p.at(8),p.at(0xc)}),
          iVar2 == 0)) || (p.at(100) == 1)) {
        invoke(0x0040d9e0,p,ctx,{0});
        p.at(0x58) = 0;
        break;
      }
    }
    if (ctx.global(0x0046d4b8) != 0) {
      p.at(0x54) = -8;
      p.at(0x88) = 0;
    }
    if (ctx.global(0x0046d4bc) != 0) {
      p.at(0x54) = 8;
      p.at(0x88) = 1;
    }
    break;
  case 0xf:
    if (p.at(0x20) == 4) {
      invoke(0x0040d950,p,ctx,{0});
    }
    else {
      if (ctx.global(0x0046d4b0) == 0) {
        p.at(0x94) = 0;
      }
      else if (p.at(0x94) == 0) {
        invoke(0x0040d9b0,p,ctx,{0});
        p.at(0x58) = -15;
        p.at(0x94) = 1;
      }
      iVar2 = invoke(0x00431d60,p,ctx,{0x2c,p.at(8),p.at(0xc) + 4});
      if ((iVar2 == 0) && (p.at(100) == 0)) {
        invoke(0x0040d9b0,p,ctx,{0});
        p.at(0x58) = 4;
      }
      if (ctx.global(0x0046d4b8) != 0) {
        p.at(0x54) = -8;
        p.at(0x88) = 0;
      }
      if (ctx.global(0x0046d4bc) != 0) {
        p.at(0x54) = 8;
        p.at(0x88) = 1;
      }
    }
    break;
  case 0x10:
    if ((ctx.global(0x0046d4b8) != 0) || (ctx.global(0x0046d4bc) != 0)) {
      invoke(0x0040d980,p,ctx,{0});
    }
    if (p.at(0x94) != 0) {
      invoke(0x0040d9b0,p,ctx,{0});
      p.at(0x58) = -15;
    }
    iVar2 = invoke(0x00431d60,p,ctx,{0x2c,p.at(8),p.at(0xc) + 4});
    if ((iVar2 == 0) && (p.at(100) == 0)) {
      invoke(0x0040d9b0,p,ctx,{0});
      p.at(0x58) = 4;
    }
    if ((p.at(0x1c) == 0) && (p.at(0x20) == 4)) {
      invoke(0x0040d950,p,ctx,{0});
    }
    break;
  case 0x11:
    iVar2 = invoke(0x00431d60,p,ctx,{0x2c,p.at(8),p.at(0xc) + 4});
    if ((iVar2 == 0) && (p.at(100) == 0)) {
      invoke(0x0040d9b0,p,ctx,{0});
      p.at(0x58) = 4;
    }
    p.at(0x54) = -16;
    if (p.at(0x88) != 0) {
      p.at(0x54) = 0x10;
    }
    if (p.at(0xc4) == 0x14) {
      invoke(0x0040da40,p,ctx,{0});
      p.at(0xc4) = 0;
    }
    else {
      p.at(0xc4) = p.at(0xc4) + 1;
    }
    break;
  case 0x5e:
    iVar2 = p.at(0x104) + 1;
    p.at(0x104) = iVar2;
    if (iVar2 == 0x28) {
      p.at(0x104) = 0;
      invoke(0x0040d950,p,ctx,{0});
    }
    iVar2 = invoke(0x00431d60,p,ctx,{0x2c,p.at(8),p.at(0xc) + 4});
    if ((iVar2 != 0) || (p.at(100) != 0)) break;
    invoke(0x0040d9b0,p,ctx,{0});
LAB_00406cc2:
    p.at(0x58) = 4;
  }
  p.at(0x44) = p.at(8) - ctx.camera_x();
  iVar2 = p.at(0xc) - ctx.camera_y();
  p.at(0x48) = iVar2;
  if (ctx.global(0x46d4ac)!=0 && p.at(0xb8)%5==0) ctx.render(p);
  if (0 < p.at(0x54)) {
    p.at(0x54) = p.at(0x54) + -1;
  }
  if (p.at(0x54) < 0) {
    p.at(0x54) = p.at(0x54) + 1;
  }
  if (p.at(0x58) < 0xb) {
    p.at(0x58) = p.at(0x58) + 1;
  }
  if (p.at(0x100) == 1) {
    p.at(8) = p.at(8) + p.at(0x54);
  }
  p.at(0xc) = p.at(0xc) + p.at(0x58);
  if (p.at(8) < 0) {
    p.at(8) = 0;
  }
  if (p.at(0xc) < 0) {
    p.at(0xc) = 0;
  }
  iVar2 = p.at(0x20) + 1;
  p.at(0x20) = iVar2;
  if (p.at(0x28) < iVar2) {
    p.at(0x20) = 0;
    iVar2 = p.at(0x1c) + 1;
    p.at(0x1c) = iVar2;
    if (p.at(0x24) + -1 < iVar2) {
      p.at(0x1c) = 0;
    }
  }
  p.at(100) = 0;
  if (ctx.global(0x0046d4ec) != 0) {
    p.at(0x98) = 1;
  }
  if ((p.at(0x98) == 1) && (ctx.global(0x0046d4ec) == 0)) {
    p.at(0x98) = 0;
    ctx.global(0x00471938) = 0;
    ctx.global(0x0047193c) = 0;
    ctx.global(0x00471940) = 0;
    invoke(0x0042eff0,p,ctx,{0x16,0x0042f060,0x0042f510,9});
    return;
  }

  if (ctx.global(0x0046d4f0) != 0) {
    iVar4 = 0;
    ctx.global(0x0046142c) = ctx.global(0x00461428);
    invoke(0x7922c8,p,ctx,{0x0045e6b8,0,0,0});
    do {
      invoke(0x0044d440,p,ctx,{iVar4});
      iVar4 = iVar4 + 1;
    } while (iVar4 < 10);
    invoke(0x0043f1d0,p,ctx,{});
    return;
  }
  if (ctx.global(0x0045c0a8) == 0) {
    ctx.global(0x0046142c) = ctx.global(0x00461428);
    ctx.global(0x00461428) = ctx.global(0x00461428) + 1;
    ctx.global(0x0045c0a8) = 2;
    invoke(0x0044d440,p,ctx,{6});
    invoke(0x0044d440,p,ctx,{7});
    invoke(0x004321f0,p,ctx,{});
    invoke(0x00427900,p,ctx,{});
    p.at(8) = ctx.global(0x0077bf4c) << 5;
    p.at(0xc) = ctx.global(0x0077bf50) << 5;
    p.at(0xb4) = 0;
  }
  if (ctx.global(0x0046c500) == 0) {
    ctx.global(0x00471928) = 0;
    ctx.global(0x00471938) = 0;
    ctx.global(0x0047193c) = 0;
    ctx.global(0x00471940) = 0;
    ctx.global(0x0046c500) = 2;
    invoke(0x00449350,p,ctx,{0xe,0x004493e0,0x00449570,5});
  }
  return;

}
}
