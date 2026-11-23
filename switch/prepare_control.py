from pathlib import Path
import shutil,struct,json
root=Path(__file__).resolve().parent
control=root/'package/control';control.mkdir(parents=True,exist_ok=True)
icon=root/'icon.jpg';raw=icon.read_bytes();assert raw[:2]==b'\xff\xd8' and raw[-2:]==b'\xff\xd9';assert len(raw)<131072
# Verify SOF dimensions and baseline JPEG rather than relying on filename.
p=2;dimensions=None
while p<len(raw):
 if raw[p]!=255:p+=1;continue
 marker=raw[p+1];p+=2
 if marker in (216,217):continue
 n=struct.unpack_from('>H',raw,p)[0]
 if marker in (192,194):dimensions=struct.unpack_from('>HH',raw,p+3);assert marker==192;break
 p+=n
assert dimensions==(256,256)
languages=['AmericanEnglish','BritishEnglish','Japanese','French','German','LatinAmericanSpanish','Spanish','Italian','Dutch','CanadianFrench','Portuguese','Russian','Korean','TraditionalChinese','SimplifiedChinese','BrazilianPortuguese']
b=bytearray((control/'control.nacp').read_bytes());assert len(b)==16384
title='蜡笔小新1-重构版'.encode('utf-8');assert len(title)<0x200
for i in range(16):
 offset=i*0x300;b[offset:offset+0x300]=bytes(0x300);b[offset:offset+len(title)]=title;publisher=b'SHIN Reconstruction';b[offset+0x200:offset+0x200+len(publisher)]=publisher
struct.pack_into('<I',b,0x302c,0xffff);b[0x3060:0x3070]=b'0.3.7'.ljust(16,b'\0');(control/'control.nacp').write_bytes(b)
for language in languages:shutil.copyfile(icon,control/f'icon_{language}.dat')
print(json.dumps({'jpeg':[256,256],'languages':len(languages),'supported_language_mask':'0xffff','display_version':'0.3.7','title_id_unchanged':'0100f55100020000'}))

