"""Build and run selected developer checks; never bundles proprietary assets.

Audio checks submit sound to the local device. Exit 0 only means the selected
checks passed, not that every stage has been manually verified.
"""
import argparse
from pathlib import Path
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[1]


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--suite', choices=['core', 'resources', 'audio'], default='core')
    parser.add_argument('--build-dir', type=Path, default=ROOT/'build')
    parser.add_argument('--config', default='Release')
    parser.add_argument('--assets', type=Path, help='Local asset directory containing RESOURCE.RES')
    parser.add_argument('--test', action='append', help='Explicit CMake test target; repeat if needed')
    parser.add_argument('--argument', action='append', default=[], help='Argument passed to explicit test targets')
    parser.add_argument('--no-build', action='store_true')
    args = parser.parse_args()
    if not args.test and args.suite != 'core' and not args.assets:
        parser.error('This suite requires --assets')
    targets = {'core': ['update_clock_test', 'game_flow_test', 'menu_idle_timing_test', 'progress_save_test', 'truecolor_surface_test', 'interface_music_cache_test'],
               'resources': ['verify_resources'],
               'audio': ['menu_idle_timing_test', 'audio_dispatch_test', 'audio_worker_test',
                         'music_transition_benchmark']}[args.suite]
    if args.test:
        targets = args.test
    if not args.no_build:
        subprocess.run(['cmake', '-S', str(ROOT), '-B', str(args.build_dir)], check=True)
        subprocess.run(['cmake', '--build', str(args.build_dir), '--config', args.config,
                        '--parallel', '4', '--target', *targets], check=True)
    with tempfile.TemporaryDirectory(prefix='shin-check-') as temporary:
        for target in targets:
            candidates = [args.build_dir/args.config/(target+'.exe'), args.build_dir/(target+'.exe'),
                          args.build_dir/target]
            executable = next((p for p in candidates if p.is_file()), None)
            if not executable:
                parser.error('Missing test executable: '+target)
            extra = list(args.argument)
            if args.test:
                print('Running '+target, flush=True)
                subprocess.run([str(executable.resolve()), *extra], check=True)
                continue
            if target == 'progress_save_test':
                extra = [temporary]
            elif target == 'verify_resources':
                extra = [str(args.assets.resolve()/'RESOURCE.RES')]
            elif target in {'audio_dispatch_test', 'audio_worker_test', 'music_transition_benchmark'}:
                extra = [str(args.assets.resolve())]
            print('Running '+target, flush=True)
            subprocess.run([str(executable.resolve()), *extra], check=True)
    print('Selected checks passed. Manual stage and platform testing remains necessary.')


if __name__ == '__main__':
    main()
