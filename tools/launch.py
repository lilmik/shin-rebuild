"""Portable Windows debug launcher; does not modify executable or game saves.

Stages use human-facing numbers (1..7), unlike the historical --scene aliases.
Boss entry grants ten of each supported inventory weapon through the game's
existing debug_boss_entry(). It is not a complete boss-behavior test.
"""
import argparse
import json
from pathlib import Path
import subprocess

ROOT = Path(__file__).resolve().parents[1]
SCENES = {1: 'level1', 2: 'stage2', 3: 'shoot', 4: 'stage3',
          5: 'stage4', 6: 'stage5', 7: 'stage6'}
BONUS = {'car': 'bonus_car', 'card': 'bonus_card', 'flag': 'bonus_flag',
         'mole': 'bonus_mole', 'puzzle': 'bonus_puzzle'}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--exe', type=Path, default=ROOT/'build/Release/SHIN_Level1.exe')
    parser.add_argument('--assets', type=Path, help='Path to RESOURCE.RES, not its directory')
    target = parser.add_mutually_exclusive_group()
    target.add_argument('--stage', type=int, choices=range(1, 8))
    target.add_argument('--boss', type=int, choices=range(1, 8))
    target.add_argument('--bonus', choices=BONUS)
    target.add_argument('--menu', action='store_true')
    parser.add_argument('--width', type=int, default=1280)
    parser.add_argument('--height', type=int, default=720)
    parser.add_argument('--use-save', action='store_true', help='Opt in to normal save loading/writes')
    parser.add_argument('--headless', action='store_true')
    parser.add_argument('--frames', type=int, default=10)
    parser.add_argument('--dry-run', action='store_true', help='Print arguments without launching')
    args = parser.parse_args()
    if args.width <= 0 or args.height <= 0 or args.frames <= 0:
        parser.error('width, height and frames must be positive')
    command = [str(args.exe.resolve()), '--width', str(args.width), '--height', str(args.height)]
    if not args.use_save:
        command += ['--fresh-start']
    if args.assets:
        command += ['--assets', str(args.assets.resolve())]
    if args.boss:
        command += ['--boss-stage', str(args.boss)]
    else:
        command += ['--scene', SCENES[args.stage] if args.stage else
                    BONUS[args.bonus] if args.bonus else 'menu']
    if args.headless:
        command += ['--headless', '--frames', str(args.frames)]
    if args.dry_run:
        print(json.dumps(command, ensure_ascii=False))
        return 0
    if not args.exe.is_file():
        parser.error('Game executable not found; build shin_progress or supply --exe')
    return subprocess.run(command, cwd=args.exe.resolve().parent).returncode


if __name__ == '__main__':
    raise SystemExit(main())
