"""Inspect RESOURCE.RES and optionally export payloads to an explicitly supplied output directory."""
import argparse, collections, hashlib, json, struct, wave
from pathlib import Path
ap=argparse.ArgumentParser(); ap.add_argument('archive',type=Path);ap.add_argument('output',type=Path);ap.add_argument('--extract',action='store_true');a=ap.parse_args();a.output.mkdir(parents=True,exist_ok=True)
b=a.archive.read_bytes(); count=struct.unpack_from('<I',b)[0]
assert count <= (len(b)-4)//28
rows=[];end=4+count*28
for i in range(count):
 raw,off,size=struct.unpack_from('<20sII',b,4+i*28);name=raw.split(b'\0')[0].decode('cp932');assert off==end and size<=len(b)-off;end=off+size;d=b[off:end];r=dict(index=i,name=name,offset=off,size=size,sha256=hashlib.sha256(d).hexdigest());ext=Path(name).suffix.lower()
 if ext=='.app':
  w,h,n=struct.unpack_from('<III',d);r.update(width=w,height=h,payload_size=n,payload_size_matches=n==size-12)
 if ext=='.map':
  w,h=struct.unpack_from('<II',d);r.update(width=w,height=h,cell_bytes=10,layout_matches=size==8+w*h*10)
 if ext=='.pal':r.update(colors=size//3,max_channel=max(d))
 rows.append(r)
 if a.extract:
  assert Path(name).name==name and name not in ('.','..') and ':' not in name and '\\' not in name
  dest=a.output/'extracted'/name;dest.parent.mkdir(exist_ok=True);dest.write_bytes(d)
assert end==len(b)
external=[]
for p in sorted((a.archive.parent/'RESOURCE').rglob('*')):
 if not p.is_file():continue
 r=dict(path=str(p.relative_to(a.archive.parent)),size=p.stat().st_size)
 try:
  with wave.open(str(p),'rb') as f:r.update(channels=f.getnchannels(),sample_rate=f.getframerate(),sample_width=f.getsampwidth(),frames=f.getnframes(),seconds=f.getnframes()/f.getframerate())
 except (wave.Error,EOFError):pass
 external.append(r)
report=dict(archive=str(a.archive),sha256=hashlib.sha256(b).hexdigest(),size=len(b),count=count,record_bytes=28,table_end=4+count*28,payload_contiguous=True,extensions=dict(collections.Counter(Path(r['name']).suffix for r in rows)),entries=rows,external_resources=external)
(a.output/'inventory.json').write_text(json.dumps(report,ensure_ascii=False,indent=2),encoding='utf-8');print(json.dumps({k:v for k,v in report.items() if k not in ['entries','external_resources']},ensure_ascii=False));print('external files',len(external))
