import struct,collections,json
from pathlib import Path
b=Path('D:/ws_vs/lbxx/lbxx_asset/RESOURCE.RES').read_bytes();out={}
for i in range(struct.unpack_from('<I',b)[0]):
 p=4+i*28;name=b[p:p+20].split(b'\0')[0].decode()
 if name in ['stage3.map','stage4.map','stage5.map','stage6.map']:
  o,n=struct.unpack_from('<II',b,p+20);d=b[o:o+n];w,h=struct.unpack_from('<II',d);v=[]
  for j in range(w*h):
   t,ox,oy=struct.unpack_from('<hhh',d,8+j*10+4)
   if t>=0:v.append({'x':j%w,'y':j//w,'type':hex(t),'offset_x':ox,'offset_y':oy})
  out[name]={'width':w,'height':h,'counts':dict(collections.Counter(x['type'] for x in v)),'placements':v}
Path('shin_rebuild/later_route_inventory.json').write_text(json.dumps(out,indent=2));print(json.dumps({n:{'size':[v['width'],v['height']],'types':v['counts']} for n,v in out.items()}))
