from pathlib import Path
import re
src=Path('shin_pseudocode/recovered_callbacks_round2/functions/0x4098f0.c').read_text();s=src[src.index('{',src.index('int callback_')):]
s=s.replace('int iVar1;','int iVar1=0;')
s=s.replace('*(int *)(param_1 + 0xc)','0x10000').replace('*(int *)(param_2 + 0xc)','0x20000')
s=s.replace('*param_2','static_cast<signed char>(e.kind)').replace('param_2[1]','e.state').replace('= param_2;','= 0x30000;')
s=re.sub(r'\*\((?:int|undefined4) \*\)\((iVar[1-4]) \+ (0x[0-9a-f]+|\d+)\)',r'mem(\1 + \2)',s)
s=re.sub(r'\(\*\*\(code \*\*\)\(\*DAT_00475200 \+ 0x30\)\)\(DAT_00475200,0,0,0\)',r'ctx.sound(0x475200)',s)
s=re.sub(r'\b_?DAT_([0-9a-f]+)\b',r'ctx.global(0x\1)',s)
s=re.sub(r'\bcallback_([0-9a-f]+)\b',r'0x\1',s)
s=re.sub(r'\bFUN_([0-9a-f]+)\b(?!\s*\()',r'0x\1',s)
s=re.sub(r'&ctx.global\(0x([0-9a-f]+)\)',r'0x\1',s)
s=s.replace('param_2','0x30000')
ids=sorted(set(re.findall(r'FUN_([0-9a-f]+)\s*\(',s)))
s=s.replace('while (iVar1 != 0) {','while (iVar1 != 0) { guard();').replace('while (iVar2 != 0) {','while (iVar2 != 0) { guard();')
head='''#include "player_object_response.hpp"
namespace shin::game {
std::int32_t respond_player_object(PlayerPayload&p,EntityPayload&e,ObjectResponseContext&ctx,unsigned loop_budget){
 auto guard=[&](){if(loop_budget==0)throw std::runtime_error("object response scan budget exhausted");--loop_budget;};
 auto mem=[&](int address)->std::int32_t&{if(address>=0x10000&&address<0x10110)return p.at(address-0x10000);if(address>=0x20000&&address<0x20200)return e.at(address-0x20000);throw std::out_of_range("invalid logical entity address");};
 auto invoke=[&](std::uint32_t id,std::initializer_list<std::int64_t>args)->std::int32_t{
 if(id==0x40d7f0){p.at(0x1c)=0;p.at(0x20)=0;p.at(0x5c)=4;p.at(0x18)=0x45c740;p.at(0x24)=2;p.at(0x28)=5;return ctx.sound(0x4752e8);}
 if(id==0x40d890){p.at(0x1c)=0;p.at(0x20)=0;p.at(0x5c)=7;p.at(0x18)=0x45c748;p.at(0x24)=1;p.at(0x28)=5;return 0;}
 return ctx.call(id,p,e,args);};
'''
for i in ids:head+=f'#define FUN_{i}(...) invoke(0x{i},{{__VA_ARGS__}})\n'
# Keep source block scoped inside function.
out=head+s+'\n'+''.join(f'#undef FUN_{i}\n' for i in ids)+'\n}\n}\n'
Path('shin_rebuild/src/game/player_object_response.cpp').write_text(out)
print('dependencies',len(ids),'unconverted deref',s.count('*('))
