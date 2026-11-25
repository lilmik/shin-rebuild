#!/bin/bash
set -euo pipefail
root="$(cd "$(dirname "$0")/../.." && pwd)"
build="${1:-$root/build/macos-universal}"
cmake -S "$root" -B "$build" -DCMAKE_BUILD_TYPE=Release -DSHIN_FETCH_SDL=ON '-DCMAKE_OSX_ARCHITECTURES=arm64;x86_64' -DCMAKE_OSX_DEPLOYMENT_TARGET=11.0
cmake --build "$build" --config Release --parallel 3 --target shin_portable
app="$build/platforms/desktop/SHIN.app"
lipo -verify_arch arm64 x86_64 "$app/Contents/MacOS/SHIN"
mkdir -p "$app/Contents/Resources/licenses"
cp "$root/LICENSE" "$app/Contents/Resources/licenses/GPL-3.0.txt"
cp "$root/src/graphics/gpu/xbrz-freescale.glsl" "$app/Contents/Resources/licenses/xbrz-freescale.glsl"
cp "$build/_deps/sdl2-src/LICENSE.txt" "$app/Contents/Resources/licenses/SDL2.txt"
cp "$build/_deps/sdl2_mixer-src/LICENSE.txt" "$app/Contents/Resources/licenses/SDL2_mixer.txt"
mkdir -p "$build/AppIcon.iconset" "$app/Contents/Resources/assets"
for size in 16 32 128 256 512; do
 sips -z "$size" "$size" "$root/platforms/desktop/AppIcon.png" --out "$build/AppIcon.iconset/icon_${size}x${size}.png" >/dev/null
 sips -z "$((size*2))" "$((size*2))" "$root/platforms/desktop/AppIcon.png" --out "$build/AppIcon.iconset/icon_${size}x${size}@2x.png" >/dev/null
done
iconutil -c icns "$build/AppIcon.iconset" -o "$app/Contents/Resources/AppIcon.icns"
printf '%s\n' 'Copy your local assets folder contents here before launching.' > "$app/Contents/Resources/assets/README.txt"
# Sign only the executable ad hoc: adding local copyrighted assets must not invalidate a sealed application resource manifest.
codesign --force --sign - "$app/Contents/MacOS/SHIN"
codesign --verify "$app/Contents/MacOS/SHIN"
otool -L "$app/Contents/MacOS/SHIN"
"$app/Contents/MacOS/SHIN" --help
printf 'Universal application: %s\n' "$app"
