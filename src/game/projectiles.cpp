#include "projectiles.hpp"
#include <cstring>
namespace shin {
static int bits(unsigned v){int n;std::memcpy(&n,&v,4);return n;}
void initialize_projectile(Projectile&p,ProjectileKind k,int x,int y,int vx,int w,int h,ProjectileContext&ctx){
 p.kind=k;p.owner_state=1;p.at(4)=x;p.at(8)=y;p.at(12)=w;p.at(16)=h;
 p.at(0x14)=2;p.at(0x18)=2;p.at(0x1c)=12;p.at(0x20)=12;
 p.at(0x2c)=0;p.at(0x30)=0;p.at(0x3c)=vx;p.at(0x40)=-10;
 p.at(0x24)=ctx.register_collision(0x434b40,k==ProjectileKind::ball?0x42d810:0x42fe30,p);
 p.at(0x28)=ctx.register_collision(0x431050,k==ProjectileKind::ball?0x42e130:0x430a50,p);
 p.at(0x34)=x;p.at(0x38)=y;p.at(0x44)=0;
}
void update_projectile(Projectile&p,ProjectileContext&ctx){
 unsigned d=unsigned(p.at(4))-unsigned(ctx.camera_x())-320u;
 unsigned sign=(d>>31)?0xffffffffu:0u;
 // Preserve original signed abs arithmetic, including INT_MIN behavior.
 if(bits((d^sign)-sign)>=961)return; // Original freezes out-of-range; DOES NOT delete.
 p.at(0x34)=p.at(4);p.at(0x38)=p.at(8);
 p.at(0x2c)=bits(unsigned(p.at(4))-unsigned(ctx.camera_x()));p.at(0x30)=bits(unsigned(p.at(8))-unsigned(ctx.camera_y()));
 if(ctx.present())ctx.render(p);
 if(p.at(0x40)<11)++p.at(0x40);
 p.at(4)=bits(unsigned(p.at(4))+unsigned(p.at(0x3c)));p.at(8)=bits(unsigned(p.at(8))+unsigned(p.at(0x40)));
}
void projectile_entity_collision(Projectile&p,unsigned k,void*t,ProjectileContext&ctx){ctx.entity_collision(p.kind==ProjectileKind::ball?0x42d810:0x42fe30,p,k,t);}
void destroy_projectile(Projectile&p,int token,ProjectileContext&ctx){ctx.unregister_collision(0x434bb0,p.at(0x24));ctx.unregister_collision(0x4310c0,p.at(0x28));ctx.release_allocation(p.at(0));ctx.release_allocation(token);}

static void ball_terrain(Projectile&p,int param_2,ProjectileContext&ctx){

  int iVar2;
  int iVar3;
  int iVar4;
  
                    

  if (param_2 == 1) {
    iVar3 = 0;
    iVar4 = 0x14;
    do {
      iVar2 = ctx.call(0x00431130,p,{iVar4,p.at(4),p.at(8) - iVar3});
      if (iVar2 == 0) {
        p.at(8) = p.at(8) - iVar3;
        p.at(0x40) = -p.at(0x40);
        iVar4 = p.at(0x44) + 1;
        p.at(0x44) = iVar4;
        if (iVar4 != 3) {
          return;
        }
        p.owner_state = 2;
        return;
      }
      iVar3 = iVar3 + 1;
    } while (iVar3 < 0x1a);
    iVar3 = p.at(4);
    iVar2 = p.at(0x34);
    if (iVar2 < iVar3) {
      if (iVar2 <= iVar3) {
LAB_0042e186:
        iVar2 = ctx.call(0x00431130,p,{iVar4,iVar3,p.at(0x38)});
        if (iVar2 != 0) goto code_r0x0042e198;
        p.at(4) = iVar3;
        p.at(0x3c) = -p.at(0x3c);
        iVar3 = p.at(0x44) + 1;
        p.at(0x44) = iVar3;
joined_r0x0042e212:
        if (iVar3 == 3) {
          p.owner_state = 2;
        }
      }
    }
    else if ((iVar3 < iVar2) && (iVar3 <= iVar2)) {
      do {
        iVar2 = ctx.call(0x00431130,p,{iVar4,iVar3,p.at(0x38)});
        if (iVar2 == 0) {
          p.at(4) = iVar3;
          p.at(0x3c) = -p.at(0x3c);
          iVar3 = p.at(0x44) + 1;
          p.at(0x44) = iVar3;
          goto joined_r0x0042e212;
        }
        iVar3 = iVar3 + 1;
      } while (iVar3 <= p.at(0x34));
    }
LAB_0042e218:
    iVar3 = p.at(8);
    if (p.at(0x38) < iVar3) {
      while (iVar2 = ctx.call(0x00431130,p,{iVar4,p.at(4),iVar3}), iVar2 != 0) {
        iVar3 = iVar3 + -1;
        if (iVar3 < p.at(0x38)) {
          return;
        }
      }
      p.at(8) = iVar3;
      p.at(0x40) = -p.at(0x40);
      iVar4 = p.at(0x44) + 1;
      p.at(0x44) = iVar4;
      if (iVar4 == 3) {
        p.owner_state = 2;
        return;
      }
    }
    else if (iVar3 < p.at(0x38)) {
      while (iVar2 = ctx.call(0x00431130,p,{iVar4,p.at(4),iVar3}), iVar2 != 0) {
        iVar3 = iVar3 + 1;
        if (p.at(0x38) < iVar3) {
          return;
        }
      }
      p.at(8) = iVar3;
      p.at(0x40) = -p.at(0x40);
      iVar4 = p.at(0x44) + 1;
      p.at(0x44) = iVar4;
      if (iVar4 == 3) {
        p.owner_state = 2;
        return;
      }
    }
  }
  else {
    if (param_2 != 2) {
      return;
    }
    iVar4 = 0;
    do {
      iVar3 = ctx.call(0x00431130,p,{0x14,p.at(4),p.at(8) - iVar4});
      if (iVar3 == 0) {
        p.at(8) = p.at(8) - iVar4;
        iVar4 = p.at(0x40) + 2;
        p.at(0x40) = iVar4;
        p.at(0x40) = -iVar4;
        iVar4 = p.at(0x44) + 1;
        p.at(0x44) = iVar4;
        if (iVar4 != 3) {
          return;
        }
        p.owner_state = 2;
        return;
      }
      iVar4 = iVar4 + 1;
    } while (iVar4 < 0x1a);
    iVar4 = p.at(8);
    if (p.at(0x38) < iVar4) {
      while (iVar3 = ctx.call(0x00431130,p,{0x14,p.at(4),iVar4}), iVar3 != 0) {
        iVar4 = iVar4 + -1;
        if (iVar4 < p.at(0x38)) {
          return;
        }
      }
      p.at(8) = iVar4;
      p.at(0x40) = -p.at(0x40);
      iVar4 = p.at(0x44) + 1;
      p.at(0x44) = iVar4;
      if (iVar4 == 3) {
        p.owner_state = 2;
      }
    }
  }
  return;
code_r0x0042e198:
  iVar3 = iVar3 + -1;
  if (iVar3 < p.at(0x34)) goto LAB_0042e218;
  goto LAB_0042e186;
}
static void bomb_terrain(Projectile&p,int param_2,ProjectileContext&ctx){

  int iVar2;
  int iVar3;
  int iVar4;
  
                    

  if (param_2 == 1) {
    iVar3 = 0;
    iVar4 = 0x14;
    do {
      iVar2 = ctx.call(0x00431130,p,{iVar4,p.at(4),p.at(8) - iVar3});
      if (iVar2 == 0) {
        p.at(8) = p.at(8) - iVar3;
        p.at(0x40) = -p.at(0x40);
        iVar4 = p.at(0x44) + 1;
        p.at(0x44) = iVar4;
        if (iVar4 != 1) {
          return;
        }
        p.owner_state = 2;
        ctx.call(0x0042b300,p,{0x42,0x0042b3d0,0x0042b460,8,p.at(4),
                     p.at(8)});
        return;
      }
      iVar3 = iVar3 + 1;
    } while (iVar3 < 0x1a);
    iVar3 = p.at(4);
    if (p.at(0x34) < iVar3) {
      do {
        iVar2 = ctx.call(0x00431130,p,{iVar4,iVar3,p.at(0x38)});
        if (iVar2 == 0) {
          p.at(4) = iVar3;
          p.at(0x3c) = -p.at(0x3c);
          iVar3 = p.at(0x44) + 1;
          p.at(0x44) = iVar3;
          goto joined_r0x00430b47;
        }
        iVar3 = iVar3 + -1;
      } while (p.at(0x34) <= iVar3);
    }
    else if (iVar3 < p.at(0x34)) {
LAB_00430b18:
      iVar2 = ctx.call(0x00431130,p,{iVar4,iVar3,p.at(0x38)});
      if (iVar2 != 0) goto code_r0x00430b2a;
      p.at(4) = iVar3;
      p.at(0x3c) = -p.at(0x3c);
      iVar3 = p.at(0x44) + 1;
      p.at(0x44) = iVar3;
joined_r0x00430b47:
      if (iVar3 == 1) {
        p.owner_state = 2;
        ctx.call(0x0042b300,p,{0x42,0x0042b3d0,0x0042b460,8,p.at(4),
                     p.at(8)});
      }
    }
LAB_00430b6b:
    iVar3 = p.at(8);
    if (p.at(0x38) < iVar3) {
      while (iVar2 = ctx.call(0x00431130,p,{iVar4,p.at(4),iVar3}), iVar2 != 0) {
        iVar3 = iVar3 + -1;
        if (iVar3 < p.at(0x38)) {
          return;
        }
      }
      p.at(8) = iVar3;
      p.at(0x40) = -p.at(0x40);
      iVar4 = p.at(0x44) + 1;
      p.at(0x44) = iVar4;
      if (iVar4 == 1) {
        p.owner_state = 2;
        ctx.call(0x0042b300,p,{0x42,0x0042b3d0,0x0042b460,8,p.at(4),
                     p.at(8)});
        return;
      }
    }
    else if (iVar3 < p.at(0x38)) {
      while (iVar2 = ctx.call(0x00431130,p,{iVar4,p.at(4),iVar3}), iVar2 != 0) {
        iVar3 = iVar3 + 1;
        if (p.at(0x38) < iVar3) {
          return;
        }
      }
      p.at(8) = iVar3;
      p.at(0x40) = -p.at(0x40);
      iVar4 = p.at(0x44) + 1;
      p.at(0x44) = iVar4;
      if (iVar4 == 1) {
        p.owner_state = 2;
        ctx.call(0x0042b300,p,{0x42,0x0042b3d0,0x0042b460,8,p.at(4),
                     p.at(8)});
        return;
      }
    }
  }
  else {
    if (param_2 != 2) {
      return;
    }
    iVar4 = 0;
    do {
      iVar3 = ctx.call(0x00431130,p,{0x14,p.at(4),p.at(8) - iVar4});
      if (iVar3 == 0) {
        p.at(8) = p.at(8) - iVar4;
        iVar4 = p.at(0x40) + 2;
        p.at(0x40) = iVar4;
        p.at(0x40) = -iVar4;
        iVar4 = p.at(0x44) + 1;
        p.at(0x44) = iVar4;
        if (iVar4 != 1) {
          return;
        }
        p.owner_state = 2;
        ctx.call(0x0042b300,p,{0x42,0x0042b3d0,0x0042b460,8,p.at(4),
                     p.at(8)});
        return;
      }
      iVar4 = iVar4 + 1;
    } while (iVar4 < 0x1a);
    iVar4 = p.at(8);
    if (p.at(0x38) < iVar4) {
      while (iVar3 = ctx.call(0x00431130,p,{0x14,p.at(4),iVar4}), iVar3 != 0) {
        iVar4 = iVar4 + -1;
        if (iVar4 < p.at(0x38)) {
          return;
        }
      }
      p.at(8) = iVar4;
      p.at(0x40) = -p.at(0x40);
      iVar4 = p.at(0x44) + 1;
      p.at(0x44) = iVar4;
      if (iVar4 == 1) {
        p.owner_state = 2;
        ctx.call(0x0042b300,p,{0x42,0x0042b3d0,0x0042b460,8,p.at(4),
                     p.at(8)});
      }
    }
  }
  return;
code_r0x00430b2a:
  iVar3 = iVar3 + 1;
  if (p.at(0x34) < iVar3) goto LAB_00430b6b;
  goto LAB_00430b18;
}
void projectile_terrain_collision(Projectile&p,int side,ProjectileContext&ctx){if(p.kind==ProjectileKind::ball)ball_terrain(p,side,ctx);else bomb_terrain(p,side,ctx);}
}
