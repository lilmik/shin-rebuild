# Third-party dependencies

Dependencies are installed separately and retain their own license terms. This repository does not relicense them.

- Windows SDK / system multimedia APIs: provided by the user's Windows development environment; not bundled.
- libnx and devkitPro toolchain/portlibs: external Switch build dependencies; retain their upstream license notices and comply with applicable distribution requirements.
- SDL2 and SDL2_mixer: external dependencies under the zlib license; codec libraries may have separate requirements. Review the exact devkitPro portlibs versions before binary distribution.
- hacBrewPack and its bundled cryptographic dependencies: not bundled in this source repository. The historical local packager carries GPLv2 and separate dependency notices; independently review version compatibility and distribution terms before bundling it. It is a separate packaging tool, not linked into the game.
- Original game assets, audio, artwork, proprietary executables and decompiler artifacts: not distributed here. No permission for them is granted by LICENSE.

A source publication is not a completed license review for distributing every possible compiled dependency combination.

## Author-provided artwork

`docs/images/living-room.png` was created and supplied by lilmik for this project. It is not extracted original-game artwork. See `docs/images/NOTICE.md` for the separate artwork notice; the code license does not automatically apply to it.

## Historical edition reference images

`docs/images/editions/` contains packaging/disc reference photographs extracted from the owner-provided project document. These are distinct from the author-created living-room artwork. Existing watermarks are retained. They are not covered by the code license; see `docs/images/editions/NOTICE.md`.

## Bundled FLAC decoder

`src/third_party/dr_flac.h` contains dr_flac 0.13.5 from the dr_libs project. It is used under its MIT No Attribution (MIT-0) option; the complete upstream notices remain in the header. Upstream: https://github.com/mackron/dr_libs .

`resources/windows/living-room.ico` is an application-icon conversion of the author-provided living-room artwork. Its artwork notice is the same as `docs/images/living-room.png`.
