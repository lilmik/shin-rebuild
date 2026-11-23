# Developer maintenance: see tools/README.md; historical scripts may require local analysis inputs and can generate files.
from pathlib import Path
import json
root=Path('shin_rebuild')
p=root/'MECHANISM_COMPLETENESS.json'
d=json.loads(p.read_text(encoding='utf8'))
runtime=(root/'src/game/level_runtime.hpp').read_text(encoding='utf-8-sig')
partial='\u90e8\u5206\u884c\u4e3a'
unbound='\u672a\u7ed1\u5b9a'
for s in d['stages']:
 for e in s['entries']:
  t=int(e['map_type'],16)
  if t==0x23:
   assert 'else if(m.type==0x23)' in runtime and 'emitters_.tick' in runtime and 'falling_hazards_.tick' in runtime
   e['status']=partial;e['detail']='Runtime: emitters_.add -> emitters_.tick -> falling_hazards_.tick; original callbacks 4389c0/438a70/44e2c0/44e3b0. Manual behavior verification pending.'
  if t==0x79 and 'else if(m.type==0x79)' in runtime and 'falling_dolls_' in runtime:
   e['status']=partial;e['detail']='Runtime now spawns dol.raw and updates falling_dolls_; rebound, damage and lifecycle require manual verification.'
  if (t==0x74 and 'final_encounter_.begin' in runtime) or (t in (0x6f,0x70) and 'sixth_scenes_.begin' in runtime):
   e['status']=partial;e['detail']='Runtime cinematic and progression chain connected; manual verification pending.'
  if t in (0x6c,0x6d,0x6e,0x71,0x72,0x73,0x7d,0x7e,0x5f):
   e['status']=partial;e['detail']='0.2.20: original dialogue timelines, card/puzzle entries or contact fire integrated. Runtime/manual equivalence not yet verified.'
 s['unbound_placements']=sum(e['placements'] for e in s['entries'] if e['status']==unbound)
 s['partially_implemented_placements']=sum(e['placements'] for e in s['entries'] if e['status']==partial)
p.write_text(json.dumps(d,ensure_ascii=False,indent=2),encoding='utf8')
lines=['# 1~7\u5173\u673a\u5173\u5b8c\u6574\u6027\u5ba1\u8ba1','',d['scope'],'','|\u5173\u5361|\u673a\u5173\u70b9|\u672a\u7ed1\u5b9a|\u90e8\u5206\u884c\u4e3a|','|---|---:|---:|---:|']
for s in d['stages']:lines.append('|%s|%s|%s|%s|'%(s['stage'],s['mechanism_placements'],s['unbound_placements'],s['partially_implemented_placements']))
for s in d['stages']:
 lines+=['','## %s: %s'%(s['stage'],s['map']),'','|\u7c7b\u578b|\u70b9\u6570|\u539f\u914d\u7f6e\u8d44\u6e90|\u72b6\u6001|\u8bf4\u660e|','|---|---:|---|---|---|']
 for e in s['entries']:lines.append('|%s|%s|%s|%s|%s|'%(e['map_type'],e['placements'],e['resource'],e['status'],e['detail']))
lines+=['','\u672c\u8868\u662f\u9759\u6001\u63a5\u5165\u5ba1\u8ba1\uff0c\u4e0d\u4ee3\u8868\u5b8c\u6574\u901a\u5173\u6216\u6d4b\u8bd5\u901a\u8fc7\u3002\u52a8\u6001Boss\u4e0d\u8ba1\u5165\u5730\u56fe\u673a\u5173\u653e\u7f6e\u70b9\u3002','MAP14/15/7a use dispatcher427610; absence from426380 is not evidence of no-op.','MAP70 exit requires four actors and helpers; MAP74 requires final pair and ending.']
(root/'MECHANISM_COMPLETENESS.md').write_text('\n'.join(lines),encoding='utf8')
print([(s['stage'],[(e['map_type'],e['placements']) for e in s['entries'] if e['status']==unbound]) for s in d['stages']])
