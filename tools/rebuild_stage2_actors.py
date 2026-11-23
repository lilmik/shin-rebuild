from pathlib import Path
import re,struct
base=Path('shin_pseudocode/recovered_callbacks_round5/functions');out=Path('shin_rebuild/src/game');binary=Path('shin_pseudocode/restored_static.exe').read_bytes();off=lambda a:a-0x45a000+0x58000
def body(addr):
 s=(base/f'0x{addr:x}.c').read_text();s=s[s.index('{',s.index('\nvoid '))+1:s.rfind('}')];return s

def translate(s):
 s=re.sub(r'/\*.*?\*/','',s,flags=re.S)
 s=re.sub(r'\*\((?:int|undefined4) \*\)\((?:iVar1|param_1) \+ ([^)]+)\)',r'e.at(\1)',s)
 s=re.sub(r'\*\(undefined \*\*\)\(param_1 \+ (0x[0-9a-f]+)\) = &DAT_00([0-9a-f]+);',lambda m:'e.at('+m[1]+') = 0x'+m[2]+';',s)
 s=s.replace('  iVar1 = *(int *)(param_1 + 0xc);','')
 s=re.sub(r'\*\(undefined1 \*\)\(param_1 \+ 1\) = 2;', 'payload.state=2;',s)
 s=s.replace('undefined4','int').replace('uint','unsigned')
 s=s.replace('*(int *)(DAT_0077bf48 + 4)','cx').replace('*(int *)(DAT_0077bf48 + 8)','cy')
 s=s.replace('*(int *)(DAT_00473d20 + 8)','px').replace('*(int *)(DAT_00473d20 + 0x10)','pw')
 s=s.replace('0xfffffff0','-16')
 s=s.replace('int iVar1;', 'int iVar1=0;')
 s=re.sub(r'  int uVar(?:[4-9]|10);\n','',s)
 s=s.replace('_DAT_0045c334 = _DAT_0045c334 + 1;','defeated=true;')
 return s
for cls,upd,ter,helpers,ns,ne,tab,te,kind,rect in [
 ('Stage2Guard',0x44cc40,0x44d020,[0x44d160,0x44d190,0x44d1c0,0x44d1f0,0x44d220],0x471780,0x4718ac,0x4718b0,0x4718f0,0x72,[28,14,46,71]),
 ('Stage2Boss',0x438020,0x438470,[0x4385b0,0x4385e0,0x438610,0x438640,0x438670],0x46e1c8,0x46e344,0x46e344,0x46e398,0x73,[38,10,113,189])]:
 names=[binary[off(a):off(a)+20].split(b'\0')[0].decode() for a in range(ns,ne,20)];idx=struct.unpack_from('<'+'i'*((te-tab)//4),binary,off(tab))
 s='#pragma once\n#include "player_object_response.hpp"\n#include "collision_world.hpp"\n#include "../assets/asset_store.hpp"\n#include <cstdlib>\nnamespace shin::game {\nclass '+cls+' {\n'
 s+='public:\n struct ProjectileEvent {int x,y,vx,vy;unsigned callback;};\n EntityPayload payload;bool respawn_requested=false,defeated=false;std::vector<ProjectileEvent>projectiles;std::vector<int>spawn_requests;std::vector<std::string>sound_requests;\n'
 s+='static inline const char*names[]={'+','.join('"'+n+'"' for n in names)+'};\nstatic inline const int indices[]={'+','.join(map(str,idx))+'};\n'
 s+=cls+'(int x,int y,AssetStore&a){auto&e=payload;e.kind='+str(kind)+';e.at(4)=e.at(0x40)=x;e.at(8)=e.at(0x44)=y;'
 for p,v in zip([0x20,0x24,0x28,0x2c],rect):s+=f'e.at({p})={v};'
 s+=f'const auto&last=a.image(names[{len(names)-1}]);e.at(100)=last.width;e.at(104)=last.height;h{helpers[1]:x}();'
 if kind==0x73:s+='e.at(0x6c)=10;spawn_requests.push_back(0x31);'
 s+='}\n'
 for h in helpers:
  text=body(h)
  if h==helpers[-1]:text=text[:text.index('  if (DAT_')]+ 'sound_requests.push_back("RESOURCE/'+('dem-tib.wav' if kind==0x73 else 'dem-h2.wav')+'");'
  text=translate(text)
  s+='void h%x(){auto&e=payload;'%h+text+'}\n'
 s+='void hurt(){h%x();}\n'%helpers[-1]
 s+='void platform(int d){if(payload.at(0x50)=='+('7' if kind==0x73 else '4')+')return;if(d>=0&&d<=2){switch(d){case 0:h%x();break;case 1:h%x();break;case 2:h%x();break;}}}\n'%tuple(helpers[:3])
 text=body(upd)
 start=text.index('  if (DAT_0046d4ac != 0)');end=text.index('\n  if (0 <',start)
 text=text[:start]+f'  frame.blit(a.image(names[indices[(e.at(0xc)-{tab})/4+e.at(0x10)]]),e.at(4)-cx,e.at(8)-cy,true,0,0,-1,-1,e.at(0x60)!=0);\n'+text[end:]
 text=re.sub(r'FUN_004262e0\(.*?;', 'respawn_requested=true;',text,flags=re.S)
 text=re.sub(r'FUN_004(?:365c0|02350)\(0x5b,callback_00([0-9a-f]+),.*?,6,(.*?)\);',lambda m:'projectiles.push_back({'+m[2]+',0x'+m[1]+'});',text,flags=re.S)
 text=re.sub(r'FUN_0043c140\(.*?;', 'spawn_requests.push_back(0x2d);',text,flags=re.S)
 for h in helpers:text=re.sub(r'FUN_00%x\((?:iVar1)?\)'%h,'h%x()'%h,text)
 text=translate(text)
 if kind==0x73:text=text.replace('  if (0 < e.at(0x48))', '  frame.blit(a.image("bsfc21.app"),10,50);for(int hp=0;hp<e.at(0x6c);++hp)frame.blit(a.image("rlife.app"),50+20*hp,50);\n  if (0 < e.at(0x48))')
 if kind==0x73:
  s+='''bool cinematic=false;
 void cinematic_pose(int state){auto&e=payload;e.at(0x50)=state;e.at(0x10)=e.at(0x14)=0;e.at(0xc)=state==8?0x46e3d0:0x46e3d8;e.at(0x18)=state==8?2:1;e.at(0x1c)=10;}
 void cinematic_tick(int cx,int cy,AssetStore&a,IndexedSurface&frame){auto&e=payload;e.at(0x48)=0;e.at(0x38)=e.at(4)-cx;e.at(0x3c)=e.at(8)-cy;frame.blit(a.image(e.at(0x50)==8&&e.at(0x10)==1?"2#422.app":"2#421.app"),e.at(0x38),e.at(0x3c));if(e.at(0x4c)<11)++e.at(0x4c);e.at(8)+=e.at(0x4c);if(++e.at(0x14)>e.at(0x1c)){e.at(0x14)=0;if(++e.at(0x10)>=e.at(0x18))e.at(0x10)=0;}}
'''
  text='if(cinematic){cinematic_tick(cx,cy,a,frame);return;}'+text
 s+='void tick(int cx,int cy,int px,int pw,AssetStore&a,IndexedSurface&frame){if(payload.state==2)return;auto&e=payload;'+text+'}\n' 
 text=translate(body(ter));text=text.replace('FUN_00431130','probe')
 s+='void terrain(const CollisionWorld&world,int param_2){auto&e=payload;auto probe=[&](int,int x,int y){return world.body({e.at(0x20),e.at(0x24),e.at(0x28),e.at(0x2c)},x,y);};'+text+'}\n};\n}\n'
 (out/('stage2_boss.hpp' if kind==0x73 else 'stage2_guard.hpp')).write_text(s)


