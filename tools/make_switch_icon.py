# Developer maintenance: see tools/README.md; historical scripts may require local analysis inputs and can generate files.
import struct,zlib
from pathlib import Path
b=Path(r'D:/ws_vs/lbxx/lbxx_asset/RESOURCE.RES').read_bytes();e={}
for i in range(struct.unpack_from('<I',b)[0]):
 p=4+i*28;n=b[p:p+20].split(b'\0')[0].decode();o,s=struct.unpack_from('<II',b,p+20);e[n]=b[o:o+s]
def image(n):
 d=e[n];w,h,_=struct.unpack_from('<III',d);return w,h,b''.join(bytes([d[i]])*d[i+1] for i in range(12,len(d),2))
pal=e['tt0002.pal'];rgb=bytearray([252]*256*256*3)
def place(n,x,y,maxw,maxh):
 w,h,p=image(n);scale=min(maxw/w,maxh/h);dw,dh=round(w*scale),round(h*scale);print(n,w,h,dw,dh)
 for j in range(dh):
  for i in range(dw):
   idx=p[min(h-1,int(j/scale))*w+min(w-1,int(i/scale))]
   if idx==254:continue
   xx=x+(maxw-dw)//2+i;yy=y+(maxh-dh)//2+j
   if 0<=xx<256 and 0<=yy<256:rgb[(yy*256+xx)*3:(yy*256+xx)*3+3]=bytes(v*4 for v in pal[idx*3:idx*3+3])
place('tt0002.app',8,5,240,65);place('tt0004.app',4,72,248,180)
c=lambda t,d:struct.pack('>I',len(d))+t+d+struct.pack('>I',zlib.crc32(t+d))
Path('shin_rebuild/switch/icon.png').write_bytes(b'\x89PNG\r\n\x1a\n'+c(b'IHDR',struct.pack('>IIBBBBB',256,256,8,2,0,0,0))+c(b'IDAT',zlib.compress(b''.join(b'\0'+rgb[y*768:(y+1)*768] for y in range(256))))+c(b'IEND',b''))
