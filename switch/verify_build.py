import argparse,hashlib,json,struct
from pathlib import Path
root=Path(__file__).resolve().parent
ap=argparse.ArgumentParser();ap.add_argument('--nsp',type=Path);args=ap.parse_args()
def pfs(path):
 b=path.read_bytes();assert b[:4]==b'PFS0';count,strings=struct.unpack_from('<II',b,4);base=16+24*count;data=base+strings;entries=[]
 for i in range(count):
  offset,size,name,_=struct.unpack_from('<QQII',b,16+i*24);end=b.index(0,base+name);label=b[base+name:end].decode();assert data+offset+size<=len(b)
  if label.endswith('.nca'):
   digest=hashlib.sha256(b[data+offset:data+offset+size]).hexdigest();assert label.split('.')[0]==digest[:32], 'NCA content hash mismatch';assert size>=0xC00
  entries.append({'name':label,'size':size,'sha256':hashlib.sha256(b[data+offset:data+offset+size]).hexdigest()})
 return entries
manifest={'compiled':True,'installed_or_device_verified':False,'final_installable_nsp_created':False,'docker_image':'devkitpro/devkita64@sha256:1fc388c3a0d34bd2045a6dadcb1020e069d5f876a187fd705de14b4440c00282','title_id':'0100f55100020000','artifacts':{}}
for name,magic,offset in [('SHIN_MVP.elf',b'\x7fELF',0),('SHIN_MVP.nso',b'NSO0',0),('SHIN_MVP.npdm',b'META',0),('SHIN_MVP.nro',b'NRO0',16)]:
 b=(root/name).read_bytes();assert b[offset:offset+4]==magic;manifest['artifacts'][name]={'size':len(b),'sha256':hashlib.sha256(b).hexdigest()}
manifest['internal_exefs_entries']=pfs(root/'SHIN_MVP.exefs.pfs0')
for entry in manifest['internal_exefs_entries']:
 if entry['name']=='main':assert entry['sha256']==manifest['artifacts']['SHIN_MVP.nso']['sha256'], 'ExeFS program differs from compiled NSO'
assert (root/'package/exefs/main').read_bytes()==(root/'SHIN_MVP.nso').read_bytes(), 'Packaged program differs from compiled NSO'
compiled_npdm=(root/'SHIN_MVP.npdm').read_bytes();packaged_npdm=bytearray((root/'package/exefs/main.npdm').read_bytes())
acid_offset=struct.unpack_from('<I',compiled_npdm,0x78)[0]
# hacBrewPack intentionally replaces only the ACID RSA public key.
packaged_npdm[acid_offset+0x100:acid_offset+0x200]=compiled_npdm[acid_offset+0x100:acid_offset+0x200]
assert bytes(packaged_npdm)==compiled_npdm, 'Packaged metadata differs outside the expected public-key patch'
if args.nsp:
 entries=pfs(args.nsp);assert len(entries)==3 and all(e['name'].endswith('.nca') for e in entries);assert sum(e['name'].endswith('.cnmt.nca') for e in entries)==1;manifest['installable_nsp_entries']=entries;manifest['final_installable_nsp_created']=True;manifest['nsp_sha256']=hashlib.sha256(args.nsp.read_bytes()).hexdigest();manifest['nca_content_hashes_verified']=True
(root/'build_manifest.json').write_text(json.dumps(manifest,indent=2),encoding='utf-8');print(json.dumps(manifest,indent=2))
