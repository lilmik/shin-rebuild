# Developer maintenance: see tools/README.md; historical scripts may require local analysis inputs and can generate files.
from pathlib import Path
import struct,json
exec(Path('shin_rebuild/tools/rebuild_stage2_scene_events.py').read_text().split('configs=')[0])
names=[read(a,20).split(b'\0')[0].decode().replace('.raw','.app') for a in range(0x45e8d8,0x45e9a0,20)];seq=struct.unpack('<16I',read(0x45e9a0,64));dur=struct.unpack('<10I',read(0x45e9e0,40));Path('shin_rebuild/src/game/flying_intro_data.inc').write_text('inline constexpr const char* flying_intro_assets[]={'+','.join(json.dumps(n) for n in names)+'};\ninline constexpr unsigned flying_intro_sequence[]={'+','.join(map(str,seq))+'};\ninline constexpr unsigned flying_intro_duration[]={'+','.join(map(str,dur))+'};\n');print(names,seq,dur)
