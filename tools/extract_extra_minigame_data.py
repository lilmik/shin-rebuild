from pathlib import Path
import struct
exec(Path('shin_rebuild/tools/rebuild_stage2_scene_events.py').read_text().split('configs=')[0])
lines=[]
for name,addr,n in [('card_xy',0x4601f0,36),('card_lives_xy',0x460280,8),('puzzle_xy',0x461330,18),('puzzle_goal',0x4613a0,9)]:
 vals=struct.unpack('<'+'i'*n,read(addr,n*4));print(name,vals);lines.append('inline constexpr int '+name+'[]={'+','.join(map(str,vals))+'};')
Path('shin_rebuild/src/game/extra_minigame_data.inc').write_text('\n'.join(lines))
