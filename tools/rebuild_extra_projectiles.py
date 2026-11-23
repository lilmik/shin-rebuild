from pathlib import Path
import re,struct
base=Path('shin_pseudocode/recovered_callbacks_round5/functions')
def body(name):
 s=(base/(name+'.c')).read_text();return s[s.index('{',s.index('\nvoid '))+1:s.rindex('}')]
def convert(s):
 s=re.sub(r'\*\((?:int|undefined4) \*\)\(iVar1 \+ (0x[0-9a-f]+|[0-9]+)\)',r'p.at(\1)',s)
 s=s.replace('*(undefined1 *)(param_1 + 1)', 'p.state').replace('uint ','unsigned ').replace('undefined4 ','int ')
 s=s.replace('*(int *)(DAT_0077bf48 + 4)','cx').replace('*(int *)(DAT_0077bf48 + 8)','cy')
 s=re.sub(r'  iVar1 = \*\(int \*\)\(param_1 \+ 0xc\);','',s)
 return s
s=body('0x44a130');a=s.index('  if (DAT_0046d4ac');z=s.index('  if (0 <',a);s=s[:a]+'  render(p, f);\n'+s[z:];s=convert(s)
t=convert(body('0x44a340'));t=t.replace('FUN_00431130(iVar1 + 0x28,','probe(').replace('FUN_00431130(iVar4,','probe(').replace('iVar4 = iVar1 + 0x28;', 'iVar4 = 0;')
# Original tables and resource strings from the retained, immutable unpacked PE.
b=Path('shin_pseudocode/restored_static.exe').read_bytes();pe=struct.unpack_from('<I',b,60)[0];o=pe+24+struct.unpack_from('<H',b,pe+20)[0];ss=[struct.unpack_from('<8sIIII',b,o+i*40) for i in range(struct.unpack_from('<H',b,pe+6)[0])]
def rd(a,n):return next(b[p+a-0x400000-v:p+a-0x400000-v+n] for _,sz,v,r,p in ss if v<=a-0x400000<v+max(sz,r))
names=[rd(0x471420+i*20,20).split(b'\0')[0].decode().replace('.raw','.app') for i in range(4)]
seq=struct.unpack('<4i',rd(0x471478,16))
header='''#pragma once
#include "player_object_response.hpp"
#include "collision_world.hpp"
#include "../assets/asset_store.hpp"
namespace shin::game {
// Recovered 44a040/44a130/44a340: flame projectile, actor kind 5d.
class Stage2ExtraProjectiles {
 AssetStore&assets_;CollisionWorld world_;
 static constexpr const char* images[]={NAMES};
 static constexpr int sequence[]={SEQ};
 void render(EntityPayload&p,IndexedSurface&f){int index=sequence[p.at(0x18)%4];f.blit(assets_.image(images[index]),p.at(0x40),p.at(0x44),true,0,0,-1,-1,p.at(0x68)!=0);}
 void terrain(EntityPayload&p,int param_2){auto probe=[&](int x,int y){return world_.body({10,10,40,40},x,y);};TERRAIN}
public:
 std::vector<EntityPayload> targets;
 Stage2ExtraProjectiles(AssetStore&a,const MapData&m,const AppImage&b):assets_(a),world_(m,b){}
 void spawn(int x,int y,int vx,int vy){EntityPayload p;p.kind=0x5d;p.state=1;p.at(4)=p.at(0x48)=x;p.at(8)=p.at(0x4c)=y;p.at(0x50)=vx;p.at(0x54)=vy;p.at(0x28)=p.at(0x2c)=10;p.at(0x30)=p.at(0x34)=40;p.at(0x58)=1;p.at(0x20)=4;p.at(0x24)=4;auto&im=assets_.image(images[3]);p.at(12)=im.width;p.at(16)=im.height;targets.push_back(p);}
 void tick(int cx,int cy,IndexedSurface&f){for(auto&p:targets){if(p.state==2)continue;if(p.state==1){p.state=0;continue;} update(p,cx,cy,f);int material=world_.body({10,10,40,40},p.at(4),p.at(8));if(material)terrain(p,material);}targets.erase(std::remove_if(targets.begin(),targets.end(),[](auto&p){return p.state==2;}),targets.end());}
 void update(EntityPayload&p,int cx,int cy,IndexedSurface&f){UPDATE}
};
}
'''
header=header.replace('NAMES',','.join('"'+x+'"' for x in names)).replace('SEQ',','.join(map(str,seq))).replace('TERRAIN',t).replace('UPDATE',s)
Path('shin_rebuild/src/game/stage2_extra_projectiles.hpp').write_text(header)
print(names,seq)
