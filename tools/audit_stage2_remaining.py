# Developer maintenance: see tools/README.md; historical scripts may require local analysis inputs and can generate files.
import struct,json
from pathlib import Path
b=Path(r'D:/ws_vs/lbxx/lbxx_asset/RESOURCE.RES').read_bytes();e={}
for i in range(struct.unpack_from('<I',b)[0]):
 p=4+i*28;n=b[p:p+20].split(b'\0')[0].decode();o,s=struct.unpack_from('<II',b,p+20);e[n]=b[o:o+s]
d=e['stage2.map'];w,h=struct.unpack_from('<II',d);types={0x14,0x15,0x25,0x46,0x47,0x48,0x4a,0x57};rows=[]
for i in range(w*h):
 c=struct.unpack_from('<5h',d,8+i*10)
 if c[2] in types:rows.append(dict(type=hex(c[2]),cell_x=i%w,cell_y=i//w,raw3=c[3],raw4=c[4]))
Path('shin_rebuild/stage2_remaining_placements.json').write_text(json.dumps(rows,indent=2));print('remaining',len(rows));print('minigame entrances',[r for r in rows if r['type']=='0x57'])
