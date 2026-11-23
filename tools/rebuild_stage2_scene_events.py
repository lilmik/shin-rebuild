# Developer maintenance: see tools/README.md; historical scripts may require local analysis inputs and can generate files.
from pathlib import Path
import re,struct,json
root=Path('shin_pseudocode/recovered_callbacks_round5/functions');b=Path('shin_pseudocode/restored_static.exe').read_bytes();pe=struct.unpack_from('<I',b,60)[0];ns=struct.unpack_from('<H',b,pe+6)[0];opt=struct.unpack_from('<H',b,pe+20)[0];sections=[]
for i in range(ns):
 p=pe+24+opt+40*i;vs,va,rs,rp=struct.unpack_from('<IIII',b,p+8);sections.append((va,max(vs,rs),rp))
def read(addr,n):
 r=addr-0x400000
 for va,size,rp in sections:
  if va<=r<va+size:return b[rp+r-va:rp+r-va+n]
 raise ValueError(hex(addr))
def code(a):return (root/f'0x{a:x}.c').read_text()
configs=[(0x46,0x40ed10,[0x40ef00,0x40ef30,0x40ef60,0x40ef90],[6320,7320,8320,12320],['57','58']),(0x47,0x40efe0,[0x40f250,0x40f280,0x40f2b0,0x40f2e0,0x40f310],[1000,5760,6760,7760,11320],['59','60','62']),(0x48,0x40f360,[0x40f550],[2760],['65']),(0x4a,0x40f800,[0x40f9e0,0x40fa10],[4660,8030],['70','71'])]
lines=[]
for typ,load,helpers,times,sounds in configs:
 txt=code(load);start=int(re.search(r'pcVar7 = s_\w+_([0-9a-f]{8})',txt)[1],16);end=int(re.search(r'pcVar7 < &DAT_([0-9a-f]{8})',txt)[1],16)
 names=[read(a,20).split(b'\0')[0].decode() for a in range(start,end,20)]
 lines.append(f'case {typ}: names_={{'+','.join(json.dumps(n) for n in names)+'}; times_={'+','.join(map(str,times))+'}; poses_={')
 poses=[]
 for helper in helpers:
  c=code(helper);state=int(re.search(r'0x5c\) = (0x[0-9a-f]+|\d+)',c)[1],0);table=int(re.search(r'&DAT_([0-9a-f]+)',c)[1],16);count=int(re.search(r'0x24\) = (0x[0-9a-f]+|\d+)',c)[1],0);inds=struct.unpack('<'+'I'*count,read(table,count*4));poses.append('{'+str(state)+',{'+','.join(map(str,inds))+'}}')
 lines[-1]+=','.join(poses)+'}; sounds_={'+','.join(json.dumps('RESOURCE/'+n+'.wav') for n in sounds)+'};break;'
Path('shin_rebuild/src/game/stage2_scene_event_data.inc').write_text('\n'.join(lines))

