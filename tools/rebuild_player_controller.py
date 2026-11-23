from pathlib import Path
import re
root=Path('../shin_pseudocode');dest=Path('src/game')
source=(root/'recovered_callbacks_round3/functions/0x405cf0.c').read_text(encoding='utf-8-sig')
s=source[source.index('{',source.index('void callback_'))+1:source.rfind('}')]
s=re.sub(r'  (?:int|undefined4) \w+;\n','',s)
s=re.sub(r'/\*.*?\*/','',s,flags=re.S)
s=s.replace('  iVar4 = *(int *)(param_1 + 0xc);','')
a=s.index('  if ((DAT_0046d4ac != 0)');b=s.index('  if (0 < *(int *)(iVar4 + 0x54))',a)
s=s[:a]+'  if (ctx.global(0x46d4ac)!=0 && p.at(0xb8)%5==0) ctx.render(p);\n'+s[b:]
s=s.replace('*(int *)(DAT_0077bf48 + 8)','ctx.camera_y()').replace('*(int *)(DAT_0077bf48 + 4)','ctx.camera_x()')
s=re.sub(r'\*\((?:int|undefined4|undefined|undefined1)\s*\**\)\(iVar4 \+ (0x[0-9a-f]+|\d+)\)',r'p.at(\1)',s)
s=s.replace('*(undefined1 *)(param_1 + 1)','p.owner_state')
s=re.sub(r'&DAT_([0-9a-f]{8})',lambda m:'0x'+m[1],s)
s=re.sub(r'_?DAT_([0-9a-f]{8})',lambda m:'ctx.global(0x'+m[1]+')',s)
s=re.sub(r'\b(?:callback|FUN)_([0-9a-f]{8})\b(?!\s*\()',lambda m:'0x'+m[1],s)
s=re.sub(r'\bs_\w+_([0-9a-f]{8})\b',lambda m:'0x'+m[1],s)
s=s.replace('FUN_0044d440(iVar4)','FUN_0044d440(group_index)')
s=s.replace('iVar4 + 0x2c','0x2c').replace('iVar4);','0);').replace('iVar4,','0,')
# Translate balanced calls; host must provide external effects; local transitions below handled internally.
def calls(t):
 pat=re.compile(r'\b(FUN_([0-9a-f]{8})|ciSendStringA)\(')
 while True:
  matches=list(pat.finditer(t))
  if not matches:break
  m=matches[-1];i=m.end();level=1;j=i
  while level:
   if t[j]=='(':level+=1
   elif t[j]==')':level-=1
   j+=1
  args=t[i:j-1];addr='0x'+m[2] if m[2] else '0x7922c8'
  t=t[:m.start()]+f'invoke({addr},p,ctx,{{{args}}})'+t[j:]
 return t
s=calls(s).replace('group_index','iVar4')
s=re.sub(r'0x([89a-f][0-9a-f]{7})\b',lambda m:str(int(m[1],16)-2**32),s)
s=s.replace('iVar4 = 0;','iVar4 = 0;')
# Extract pure state setter helpers, rejecting any side effects beyond field stores.
setters=[]
for f in sorted((root/'recovered_functions_final/functions').glob('0x40d*.c')):
 addr=int(f.stem,16)
 if not 0x40d730<=addr<=0x40db00:continue
 text=f.read_text(encoding='utf-8-sig');body=text[text.index('{',text.index('void FUN_'))+1:text.rfind('}')] if 'void FUN_' in text else ''
 body=re.sub(r'/\*.*?\*/','',body,flags=re.S)
 body=re.sub(r'\*\((?:int|undefined4|undefined)\s*\**\)\(param_1 \+ (0x[0-9a-f]+|\d+)\)',r'p.at(\1)',body)
 body=re.sub(r'&DAT_([0-9a-f]{8})',lambda m:'0x'+m[1],body)
 body=body.replace('return;','')
 if re.sub(r'p.at\((?:0x[0-9a-f]+|\d+)\)\s*=\s*(?:0x[0-9a-f]+|\d+);|\s+','',body)=='':setters.append((addr,body.strip()))
head='''#pragma once
#include <array>
#include <cstdint>
#include <initializer_list>
#include <stdexcept>
namespace shin {
struct PlayerPayload {
 std::array<std::int32_t,68> words{};
 std::uint8_t owner_state=0;
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
'''
(dest/'player_controller.hpp').write_text(head)
helpers='static std::int32_t invoke(std::uint32_t id,PlayerPayload& p,PlayerContext& ctx,std::initializer_list<std::int64_t> args){\n switch(id){\n'
for addr,body in setters:helpers+=f'case 0x{addr:x}: {body} return 0;\n'
helpers+='default:return ctx.call(id,p,args);\n }\n}\n'
(dest/'player_controller.cpp').write_text('#include "player_controller.hpp"\nnamespace shin {\n'+helpers+'void update_player(PlayerPayload& p,PlayerContext& ctx){\n std::int32_t iVar1=0,iVar2=0,iVar4=0,uVar7=0;\n'+s+'\n}\n}\n')
print('setter helpers',len(setters),'cases',len(re.findall('case ',s)))
