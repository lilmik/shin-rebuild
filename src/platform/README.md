# Windows native backend

`native_backend.hpp/.cpp` implements `shin::platform::NativeWindow` and `WavSound`. Build as C++17 and link user32, gdi32, winmm. It compiled with MSVC 19.44 x64 /W4; no GUI or audio device runtime test has been performed.

Window constructor accepts a title and optional logical width/height (default 640x480). Call pump() each iteration; false requests exit. present() consumes contiguous 8-bit indices and a 256-entry 0x00RRGGBB palette synchronously. It converts to 32-bit top-down DIB, preserves aspect ratio with black margins, and uses nearest-neighbor GDI scaling. input() returns the previously reconstructed portable InputState, preserving original VK flag mapping. The native default cursor remains visible as a usability choice; original hide request is retained in input state.

WavSound loads RIFF/WAVE PCM with independent per-object waveOut voice, supports loop or one-shot, and resets safely before reusing or destroying. Each object must outlive playback. Only PCM is supported, not arbitrary compressed WAVE. load/play return false on parse/device errors. There is no DirectSound volume/pan/frequency emulation or CD audio replacement yet, so this is a replacement platform backend rather than a behavior-equivalence claim.

Evidence: original 0x4332d0 creates DirectDraw, sets 640x480x8 display, creates surfaces and installs palette; 0x436da0 traverses RIFF WAVE fmt/data with `_INMM` mio functions and stores loaded sample bytes. Reimplemented interfaces keep resource loading and gameplay outside Windows API code.
