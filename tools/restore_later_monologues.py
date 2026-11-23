from pathlib import Path
import re,struct,json
exec(Path('shin_rebuild/tools/rebuild_stage2_scene_events.py').read_text().split('configs=')[0])
lines=[]
extra=[(108,0x411d50,[0x411f40,0x411f70,0x411fa0,0x411fa0],[4340,5730,8480,9890],[0x77d440,0x77d430,0x77d43c,0x77d438]),(109,0x411ff0,[0x4121c0],[1770],[0x4785bc]),(110,0x412210,[0x4123f0,0x412420,0x412450],[2500,3750,11420],[0x473180,0,0x47317c]),(113,0x412960,[0x412b40,0x412b70],[2000,4850],[0x477dfc,0]),(114,0x412bc0,[0x412da0,0x412dd0],[1950,4360],[0x7903c4,0x7903bc]),(115,0x412e20,[0x412ff0],[3950],[0x78e790])]

for typ,loader,helpers,times,voices in [(85,0x4113f0,[0x4115d0,0x411600,0x411630],[3830,6120,10330],[0x47793c,0x477930,0x477938]),(107,0x411b30,[0x411d00],[5400],[0x77ff94])]+extra:
 txt=code(loader);start=int(re.search(r'pcVar\d+ = s_\w+_([0-9a-f]{8})',txt)[1],16);end=int(re.search(r'pcVar\d+ < &DAT_([0-9a-f]{8})',txt)[1],16);names=[read(a,20).split(b'\0')[0].decode() for a in range(start,end,20)];poses=[]
 for h in helpers:
  c=code(h);state=int(re.search(r'0x5c\) = (0x[0-9a-f]+|\d+)',c)[1],0);table=int(re.search(r'&DAT_([0-9a-f]+)',c)[1],16);n=int(re.search(r'0x24\) = (0x[0-9a-f]+|\d+)',c)[1],0);inds=struct.unpack('<'+'I'*n,read(table,4*n));delay=int(re.search(r'0x28\) = (0x[0-9a-f]+|\d+)',c)[1],0);poses.append('{'+str(state)+','+str(delay)+',{'+','.join(map(str,inds))+'}}')
 lines.append('case '+str(typ)+':names={'+','.join(json.dumps(x) for x in names)+'};poses={'+','.join(poses)+'};times={'+','.join(map(str,times))+'};voices={'+','.join(map(str,voices))+'};break;')
Path('shin_rebuild/src/game/later_monologues_data.inc').write_text('\n'.join(lines))
