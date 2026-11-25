"""Generate self-contained CMD debug entries next to a Windows SHIN.exe.

The generated scripts do not require Python. Stage aliases deliberately map
human stage 3 to 'shoot', and human stages 4..7 to stage3..stage6.
"""
import argparse
from pathlib import Path


def generate(directory):
    scenes = {1: 'level1', 2: 'stage2', 3: 'shoot', 4: 'stage3',
              5: 'stage4', 6: 'stage5', 7: 'stage6'}
    routes = {'START.cmd': '', 'START_WINDOWED.cmd': '--windowed',
              'START_FRESH.cmd': '--fresh-start',
              'START_STAGE2_MOLE_EVENT.cmd': '--fresh-start --scene stage2_mole',
              'START_STAGE7_BOSS_UNARMED.cmd': '--fresh-start --scene stage7_boss'}
    for stage, scene in scenes.items():
        routes[f'START_STAGE{stage}.cmd'] = '--fresh-start --scene '+scene
        routes[f'START_STAGE{stage}_BOSS.cmd'] = '--fresh-start --boss-stage '+str(stage)
    for name, scene in {'car': 'bonus_car', 'card': 'bonus_card', 'flag': 'bonus_flag',
                        'mole': 'bonus_mole', 'puzzle': 'bonus_puzzle'}.items():
        routes[f'TEST_{name.upper()}.cmd'] = '--fresh-start --scene '+scene
    routes['START_STAGE7_BOSS_WEAPONS.cmd'] = '--fresh-start --boss-stage 7'
    directory.mkdir(parents=True, exist_ok=True)
    for name, arguments in routes.items():
        text = ('@echo off\nrem Debug entry: see DEBUG_SHORTCUTS.txt.\n'
                'pushd "%~dp0"\n'
                f'start "" "%~dp0SHIN.exe" {arguments} %*\n'
                'popd\n')
        (directory/name).write_text(text, encoding='ascii')
    (directory/'DEBUG_SHORTCUTS.txt').write_text(
        'Windows 快速调试入口\n\n'
        'START：正常启动，读取正式存档。START_WINDOWED：窗口启动。\n'
        'START_FRESH：初次游戏状态，不写正式进度。\n'
        'START_STAGE1～7：对应第一至第七关入口。\n'
        'START_STAGE1_BOSS～7_BOSS：Boss／结束触发点前，各库存武器 10 件。\n'
        'START_STAGE7_BOSS_UNARMED：第七关 Boss 入口，不携带武器。\n'
        'START_STAGE2_MOLE_EVENT：第二关打地鼠触发入口。\n'
        'TEST_CAR/CARD/FLAG/MOLE/PUZZLE：五个小游戏。\n\n'
        '调试入口默认 --fresh-start，绕过关卡开放限制，不写正式进度；'
        '配置和诊断日志仍可能生成。Boss 入口绕过前置流程，不能替代整关测试。\n'
        '小游戏：方向选择／移动；翻牌和打地鼠按空格操作；F6 退出。\n'
        'PC 主菜单 Esc 重绘，F11 切换全屏。脚本可附加 --windowed 等参数。\n'
        '这些 CMD 直接调用同目录 SHIN.exe，不需要安装 Python。\n', encoding='utf-8-sig')
    print(f'Generated {len(routes)} standalone CMD shortcuts in {directory}')


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('directory', type=Path)
    generate(parser.parse_args().directory)
