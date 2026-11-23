from pathlib import Path
import struct,zlib,sys
p=Path(sys.argv[1]);_,s,_,b=p.read_bytes().split(b'\n',3);w,h=map(int,s.split())
def c(t,d):return struct.pack('>I',len(d))+t+d+struct.pack('>I',zlib.crc32(t+d))
p.with_suffix('.png').write_bytes(b'\x89PNG\r\n\x1a\n'+c(b'IHDR',struct.pack('>IIBBBBB',w,h,8,2,0,0,0))+c(b'IDAT',zlib.compress(b''.join(b'\0'+b[y*w*3:(y+1)*w*3] for y in range(h))))+c(b'IEND',b''))
