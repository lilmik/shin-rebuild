# Developer maintenance: see tools/README.md; historical scripts may require local analysis inputs and can generate files.
# MDS layout reference: https://github.com/cdemu/cdemu/blob/master/libmirage/images/image-mds/image-mds.h
from pathlib import Path
import argparse,struct,wave,hashlib,json
p=argparse.ArgumentParser();p.add_argument('mds',type=Path);p.add_argument('output',type=Path);a=p.parse_args();b=a.mds.read_bytes();assert b[:16]==b'MEDIA DESCRIPTOR';assert struct.unpack_from('<H',b,20)[0]==1
session=struct.unpack_from('<I',b,80)[0];count=b[session+10];base=struct.unpack_from('<I',b,session+20)[0];tracks=[]
for i in range(count):
 o=base+80*i;mode,sub,ctl,tno,point=struct.unpack_from('<5B',b,o)
 if not 1<=point<=99:continue
 extra=struct.unpack_from('<I',b,o+12)[0];size=struct.unpack_from('<H',b,o+16)[0];lba=struct.unpack_from('<I',b,o+36)[0];offset=struct.unpack_from('<Q',b,o+40)[0];gap,length=struct.unpack_from('<II',b,extra)
 if (mode & 15) in (1,9):
  assert size==2352 and sub==0;tracks.append(dict(cd_track=point,lba=lba,offset=offset,sectors=length,pregap=gap))
assert len(tracks)==14 and [t['cd_track'] for t in tracks]==list(range(2,16));mdf=a.mds.with_suffix('.mdf');a.output.mkdir(parents=True,exist_ok=True)
with mdf.open('rb') as source:
 for n,t in enumerate(tracks,1):
  size=t['sectors']*2352;assert t['offset']+size<=mdf.stat().st_size;source.seek(t['offset']);raw=source.read(size);assert len(raw)==size;dest=a.output/f'{n:02}.wav'
  with wave.open(str(dest),'wb') as f:f.setnchannels(2);f.setsampwidth(2);f.setframerate(44100);f.writeframes(raw)
  with wave.open(str(dest),'rb') as f:assert f.readframes(f.getnframes())==raw
  t.update(file=dest.name,seconds=t['sectors']/75,pcm_sha256=hashlib.sha256(raw).hexdigest(),wav_sha256=hashlib.sha256(dest.read_bytes()).hexdigest())
report=dict(source=str(a.mds),mdf_sha256=hashlib.sha256(mdf.read_bytes()).hexdigest(),format='PCM signed 16-bit little-endian, 44100 Hz, stereo',conversion='Original MDF audio bytes encapsulated in WAV; no resampling, gain change or lossy conversion',tracks=tracks)
(a.output/'extraction_manifest.json').write_text(json.dumps(report,indent=2),encoding='utf8');print('Extracted and PCM-verified',len(tracks),'tracks;',sum((a.output/t['file']).stat().st_size for t in tracks),'bytes')

