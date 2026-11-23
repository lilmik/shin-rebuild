"""Export local APP previews and a contact sheet; see ../README.md for copyright and palette notes."""
import argparse,struct,zlib,binascii,json,math
from pathlib import Path
ap=argparse.ArgumentParser();ap.add_argument('source',type=Path);ap.add_argument('output',type=Path);ap.add_argument('--palette',default='shin.pal');args=ap.parse_args();src=args.source;root=args.output;root.mkdir(parents=True,exist_ok=True);out=root/'png';out.mkdir(exist_ok=True)
# Palette assignment, RGB order and 6-to-8-bit scaling are preview assumptions.
pal=(src/args.palette).read_bytes();pal=[tuple(min(255,c*4) for c in pal[i:i+3]) for i in range(0,768,3)]
def png(path,w,h,raw):
 def chunk(k,v):return struct.pack('>I',len(v))+k+v+struct.pack('>I',binascii.crc32(k+v)&0xffffffff)
 scan=b''.join(b'\0'+raw[y*w*3:(y+1)*w*3] for y in range(h))
 path.write_bytes(b'\x89PNG\r\n\x1a\n'+chunk(b'IHDR',struct.pack('>IIBBBBB',w,h,8,2,0,0,0))+chunk(b'IDAT',zlib.compress(scan))+chunk(b'IEND',b''))
rows=[];images=[]
for p in sorted(src.glob('*.app')):
 b=p.read_bytes();w,h,n=struct.unpack_from('<III',b);pixels=bytearray()
 assert n==len(b)-12 and n%2==0
 for i in range(12,len(b),2):pixels.extend([b[i]]*b[i+1])
 assert len(pixels)==w*h,(p.name,w,h,len(pixels))
 rgb=b''.join(bytes(pal[v]) for v in pixels);png(out/(p.stem+'.png'),w,h,rgb)
 rows.append(dict(name=p.name,width=w,height=h,pixels=len(pixels),valid=True));images.append((p.name,w,h,rgb))
# Sheet contains first 120 alphabetic entries, 10 columns, 128x112 image area each.
sel=images[:120];cols=10;cw=128;ch=112;sw=cols*cw;sh=math.ceil(len(sel)/cols)*ch;sheet=bytearray([32]*(sw*sh*3))
for idx,(name,w,h,rgb) in enumerate(sel):
 scale=min((cw-4)/w,(ch-4)/h);nw=max(1,int(w*scale));nh=max(1,int(h*scale));ox=(idx%cols)*cw+(cw-nw)//2;oy=(idx//cols)*ch+(ch-nh)//2
 for y in range(nh):
  for x in range(nw):
   si=(min(h-1,int(y/scale))*w+min(w-1,int(x/scale)))*3;di=((oy+y)*sw+ox+x)*3;sheet[di:di+3]=rgb[si:si+3]
png(root/'contact_sheet.png',sw,sh,sheet)
(root/'contact_sheet_index.json').write_text(json.dumps([x[0] for x in sel],indent=2),encoding='utf-8')
(root/'app_validation.json').write_text(json.dumps(dict(count=len(rows),all_exact=True,algorithm='u8 value,u8 count repeated pairs',evidence='ev_d8f44309c95124fef64f7ef5a59250f666e443b4a3e48eac275b6eb87956a542',palette_assumptions='shin.pal; RGB order; channel times 4',entries=rows),indent=2),encoding='utf-8')
print('decoded exact',len(rows),'PNG files; sheet',sw,sh)
