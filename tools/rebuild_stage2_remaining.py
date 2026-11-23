# Developer maintenance: see tools/README.md; historical scripts may require local analysis inputs and can generate files.
from pathlib import Path
exec(Path('shin_rebuild/tools/rebuild_extra_projectiles.py').read_text().split("s=body('0x44a130')")[0])
b=Path('shin_pseudocode/restored_static.exe').read_bytes();pe=struct.unpack_from('<I',b,60)[0];o=pe+24+struct.unpack_from('<H',b,pe+20)[0];ss=[struct.unpack_from('<8sIIII',b,o+i*40) for i in range(struct.unpack_from('<H',b,pe+6)[0])]
def rd(a,n):return next(b[p+a-0x400000-v:p+a-0x400000-v+n] for _,sz,v,r,p in ss if v<=a-0x400000<v+max(sz,r))
names=[rd(0x45f208+i*20,20).split(b'\0')[0].decode().replace('.raw','.app') for i in range(12)];seq=struct.unpack('<8i',rd(0x45f308,32));print(names,seq)
s=Path('shin_rebuild/src/game/stage2_extra_enemies.hpp').read_text().replace('Stage2ExtraEnemy','Stage2RemainingEnemy').replace('int type_;','int type_;\n static constexpr const char* names[]={'+','.join('"'+n+'"' for n in names)+'};\n static constexpr int sequence[]={'+','.join(map(str,seq))+'};')
a=s.index(' static bool supports');z=s.index(' void hurt()',a)
s=s[:a]+''' static bool supports(int t){return t==0x25;}
 static std::string image_name(int,int i){return names[sequence[i]];}
 Stage2RemainingEnemy(int type,int x,int y,AssetStore&a):type_(type){if(!supports(type))throw std::invalid_argument("map25 type");auto&e=payload;e.kind=0x71;e.at(4)=e.at(0x48)=x;e.at(8)=e.at(0x4c)=y;auto&im=a.image(names[11]);e.at(0xc)=im.width;e.at(0x10)=im.height;e.at(0x28)=22;e.at(0x2c)=33;e.at(0x30)=59;e.at(0x34)=193;pose(1);}
'''+s[z:]
s=s.replace('state==4','state==3').replace('!=4','!=3').replace('==4','==3').replace('pose(4)','pose(3)').replace('"RESOURCE/dem-boy.wav":"RESOURCE/dem-h2.wav"','"RESOURCE/dem-h1.wav":"RESOURCE/dem-h1.wav"')
s=s.replace('type_==0x1d?20:10','10')
s=s.replace('if(type_==0x1e&&++e.at(0x6c)==100)','if(false&&++e.at(0x6c)==100)')
Path('shin_rebuild/src/game/stage2_remaining_entities.hpp').write_text(s)
