# coding: utf-8
# Developer maintenance: see tools/README.md; historical scripts may require local analysis inputs and can generate files.
from pathlib import Path
import struct,collections,json,re,hashlib
root=Path('shin_rebuild'); original=Path('shin_pseudocode/recovered_callbacks_round5/functions')
cfg=Path('D:/ws_vs/lbxx/lbxx_asset/RESOURCE/char.cfg').read_text().splitlines()
archive=Path('D:/ws_vs/lbxx/lbxx_asset/RESOURCE.RES').read_bytes();entries={}
for i in range(struct.unpack_from('<I',archive)[0]):
 name,off,size=struct.unpack_from('<20sII',archive,4+i*28);entries[name.split(b'\0')[0].decode()] = archive[off:off+size]
dispatch={}
for address in ['426380','427610']:
 s=(original/f'0x{address}.c').read_text()
 for m in re.finditer(r'case\s+(0x[0-9a-f]+|\d+)\s*:(.*?)(?=\n\s*case\s|\n\s*default:|\Z)',s,re.S):
  t=int(m[1],0);calls=re.findall(r'(?:callback_|FUN_)(004[0-9a-f]{5})',m[2]); callbacks=[]
  for call in calls:
   p=original/f'0x{int(call,16):x}.c'
   if p.exists():
    body=p.read_text();draws=sorted(set(re.findall(r'FUN_(00433[456][0-9a-f]{2})',body)))
    callbacks.append({'address':hex(int(call,16)),'direct_draw_calls':[hex(int(d,16)) for d in draws]})
  dispatch.setdefault(t,[]).append({'dispatcher':'0x'+address,'callbacks':callbacks})
platforms={3,4,5,0x31,0x32,*range(0x60,0x68)};springs={9,0x44,0x45,0x58,0x59,0x5b,0x5e};markers=set(range(13,18));fixed={0x23,0x5f,0x74};icons={0x57,0x7b,0x7c,0x7d,0x7e};pickups={0xa,0xb,0xc,0x18,0x19,0x1a,0x1b,0x1c,0x26,0x2a,0x2b,0x2c,0x2d,0x34,0x35,0x78}
report={'version':'0.2.39','scope':'Complete map placements and tile/resource audit, original callback static comparison, not full original runtime verification','stages':[]}
lines=['# 0.2.39 七关地图元素逐项核对','', '依据：原 RESOURCE.RES、RESOURCE/char.cfg、426380/427610 及其创建/更新回调。地图与资源使用原文件；不把占位图当作应显示的剧情素材。','', '实现：地图加载时建立所有板块、平台、弹簧、拾取物、固定机关与入口。平台接近时开始运动，离屏保留实例和状态，不改写地图后重新创建。背景两层地图在加载时完整解码，绘制时裁剪。','', '|关卡|板块/平台|弹簧|拾取物|方向标记|固定机关/入口|全部放置点|','|---|---:|---:|---:|---:|---:|---:|']
for stage,name in enumerate(['stage1','stage2','shoot','stage3','stage4','stage5','stage6'],1):
 b=entries[name+'.map'];w,h=struct.unpack_from('<II',b);atlas=entries[(['tile1','tile2','stile','tile3','tile4','tile5','tile6'][stage-1])+'.app'];aw,ah=struct.unpack_from('<II',atlas);rows=[];tiles=[];counts=collections.Counter()
 for i in range(w*h):
  bg,fg,t,ox,oy=struct.unpack_from('<5h',b,8+i*10)
  for v in [bg,fg]:
   if v>=0 and (v%20*32+32>aw or v//20*32+32>ah):tiles.append([i%w,i//w,v])
  if t<0:continue
  res=cfg[t].replace('.raw','.app');data=entries[res];sw,sh=struct.unpack_from('<II',data);x=i%w*32+16+ox-sw//2;y=(i//w+1)*32+oy-sh
  category='platform' if t in platforms else 'spring' if t in springs else 'pickup' if t in pickups else 'direction_marker' if t in markers else 'fixed_scenery_or_entry' if t in fixed|icons|{0x41,0x75,0x76} else 'enemy_or_scripted_actor'
  counts[category]+=1;rows.append({'cell':[i%w,i//w],'type':hex(t),'resource':cfg[t],'resource_size':[sw,sh],'world':[x,y],'category':category,'original_dispatch':dispatch.get(t,[])})
 assert not tiles,(name,tiles[:5])
 counts=dict(counts);report['stages'].append({'stage':stage,'map':name+'.map','map_sha256':hashlib.sha256(b).hexdigest(),'size':[w,h],'invalid_tile_references':tiles,'counts':counts,'placements':rows})
 lines.append('|'+ '|'.join(map(str,[stage,counts.get('platform',0),counts.get('spring',0),counts.get('pickup',0),counts.get('direction_marker',0),counts.get('fixed_scenery_or_entry',0),len(rows)])))
lines+=['','飞行关卡的653处云朵也在加载地图时预建；固定壶单独保存为场景实例。','','## 已确认并处理的差异','','- 普通/遗迹平台不再因离屏销毁、写回其他类型和重建；保持本实例的位置和方向。','- MAP23 固定落物发射器随地图创建，离屏不销毁；接近后才运转，避免全地图同时发射。','- 创建坐标统一使用 char.cfg 对应原素材尺寸，而非当前动画帧尺寸；消除不同帧高宽带来的锚点偏移（包括各类弹簧和猴子）。','- 第7关 MAP74 的 hang.raw 固定壶已恢复，演出/战斗持续显示。','','## 绘图核对与尚待确认','','- 七张地图两层全部瓦片引用均在对应原贴图范围内，所有放置类型的 char.cfg 素材均存在并可读。逐放置点坐标、类型、素材尺寸和原回调见 JSON。','- 方向/停止标记、剧情调试牌不作为场景可见素材直接绘出；原程序的剧情回调决定实际角色和动画。不能用素材名未直接出现在绘制语句中推断漏图。','- 第7关原地图另含 MAP29 护士，坐标格(985,96)，在 MAP74 终战入口(568,97)之后很远；正常终战结束后直接结局，重建版未独立激活该尾部放置点。原版是否存在到达这里的路线尚未运行确认，保留记录，不擅自增加第三名终战 Boss。','- 敌人、Boss、弹丸和剧情临时演员仍按战斗或剧情事件创建，不纳入固定机关预建。其行为/帧时序不因本次静态清点而视为等价。','- 本轮未运行原版全关逐帧对照，不宣称所有绘图与剧情已完全还原；原版静态证据不足的尾部内容和复杂演出仍待运行画面对照。','','## 定点检查','','七关构造后及重置后分别核对对象数量、原素材锚点与重复创建；遗迹平台离屏后位置、方向与实例保留。没有进行全关通关自动测试。']
(root/'WORLD_CONTENT_AUDIT_0.2.39.json').write_text(json.dumps(report,ensure_ascii=False,indent=2),encoding='utf8')
(root/'WORLD_CONTENT_AUDIT_0.2.39.md').write_text('\n'.join(lines),encoding='utf8')
print('\n'.join(lines[5:15]))
