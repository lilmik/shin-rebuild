# Linux / macOS desktop port (0.3.26)

Uses SDL2, SDL2_mixer and the shared OpenGL GPU presenter. Gameplay, maps, FLAC decoding, save unlock rules, three filters and opening artwork use the existing portable game code. This entry point shares behaviour with the Switch SDL host, not the Windows-specific audio/window backend.

## Linux

Debian/Ubuntu build dependencies: `build-essential cmake pkg-config libsdl2-dev libsdl2-mixer-dev`. Build: `cmake -S . -B build/linux -DCMAKE_BUILD_TYPE=Release`, then `cmake --build build/linux --parallel 3 --target shin_portable`. Run `build/linux/platforms/desktop/SHIN --assets /path/to/assets`.

Requires a working OpenGL 3.3 desktop driver. Local Debian 12 x86_64 builds require glibc 2.36 or newer and SDL2/SDL2_mixer runtime libraries. GitHub Ubuntu builds may require newer glibc; build locally for older distributions.

## macOS Universal

Requires a Mac with Xcode Command Line Tools, CMake and network access for pinned SDL2 2.30.11 / SDL2_mixer 2.8.1 source archives. Run `bash platforms/desktop/build_macos.sh`. Both SDL dependencies and SHIN compile as `arm64;x86_64`; `lipo -verify_arch` checks the executable. Minimum deployment target: macOS 11.0. The application requests a macOS-compatible 3.2 core OpenGL context and GLSL 150.

Output: `build/macos-universal/platforms/desktop/SHIN.app`. Add your local assets to `SHIN.app/Contents/Resources/assets/`. The executable is signed ad hoc, not Developer ID signed or notarized. Actual Intel and Apple Silicon runtime/audio/display tests remain required even after universal compilation.

## Assets and controls

Original assets are never uploaded by CI. Copy the complete `assets` folder from a local release or use `--assets /path/to/RESOURCE.RES`. Keep `RESOURCE.RES`, `RESOURCE/`, `bgm/`, `_inmm.ini`, and `overrides/` together. The authored application icon is included; original game image/audio data is not.

Arrow keys move/select; Space attacks/flips/strikes; Enter confirms/pauses; F6 exits a minigame; F10 switches GPU nearest/linear/xBRZ; F11 toggles fullscreen; Esc returns from menus. Controllers use SDL mappings and the right trigger switches filters. macOS function keys may need Fn.

`--stage 1..7`, `--boss-stage 1..7` (10 of each weapon), `--fresh-start`, `--windowed`, `--frames N`, `--scene menu`, `--verify-assets`, `--help` are supported. Debug stage entries should be combined with `--fresh-start` to avoid modifying progress. Settings and logs use SDL's per-user application directory; saves are not written beside the executable.

GPU is required for normal play. `SHIN_ALLOW_SOFTWARE_GPU=1` is an explicit software-renderer test override, not a hardware performance claim. No changes to Windows/Switch release packages are implied by this port.
