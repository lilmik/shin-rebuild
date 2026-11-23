# Developer maintenance: see tools/README.md; historical scripts may require local analysis inputs and can generate files.
from pathlib import Path
import re
root=Path('../shin_pseudocode/recovered_callbacks_round3/functions');out=[]
for addr,name in [('42e130','ball_terrain'),('430a50','bomb_terrain')]:
 s=(root/f'0x{addr}.c').read_text(encoding='utf-8-sig');s=s[s.index('{',s.index('void callback_'))+1:s.rfind('}')]
 s=re.sub(r'/\*.*?\*/','',s,flags=re.S).replace('  iVar1 = *(int *)(param_1 + 0xc);','')
 s=re.sub(r'\*\((?:int|undefined4)\s*\*\)\(iVar1 \+ (0x[0-9a-f]+|\d+)\)',r'p.at(\1)',s)
 s=s.replace('*(undefined1 *)(param_1 + 1)','p.owner_state').replace('iVar1 + 0x14','0x14')
 s=re.sub(r'\bcallback_([0-9a-f]{8})',lambda m:'0x'+m[1],s)
 for call in ['00431130','0042b300']:
  pat=re.compile('FUN_'+call+r'\(')
  while (m:=pat.search(s)):
   i=m.end();j=i;depth=1
   while depth:
    if s[j]=='(':depth+=1
    if s[j]==')':depth-=1
    j+=1
   s=s[:m.start()]+f'ctx.call(0x{call},p,{{'+s[i:j-1]+'})'+s[j:]
 s=s.replace('  int iVar1;','')
 out.append('static void '+name+'(Projectile&p,int param_2,ProjectileContext&ctx){'+s+'}\n')
p=Path('src/game/projectiles.cpp');s=p.read_text(encoding='utf-8-sig').split('static void ball_terrain')[0]+'\n'+''.join(out)+'void projectile_terrain_collision(Projectile&p,int side,ProjectileContext&ctx){if(p.kind==ProjectileKind::ball)ball_terrain(p,side,ctx);else bomb_terrain(p,side,ctx);}\n}\n';p.write_text(s)
