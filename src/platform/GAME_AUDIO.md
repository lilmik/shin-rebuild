# Game audio integration

GameAudio owns independent waveOut PCM WAV voices, and Windows Media Foundation MFPlay MP3 playback. Repeated WAV play restarts that voice; independent IDs overlap. Missing files return false, and unbound original globals throw.

The original _inmmserv inserts one empty data-track entry before INI lines: CD2 selects bgm/01.mp3, CD3 selects 02, CD4 selects 03. load_inmm_playlist applies this verified mapping. First-level background music is CD4.

The prior MCI mpegvideo backend failed on this actual Windows host with error277 despite an installed MCI MPEGVideo registration. MFPlay replaces it. COM initializes on the calling thread; the host must use and destroy GameAudio on that thread. Shutdown releases the media player before balancing COM initialization. Link mfplay.lib and ole32.lib alongside winmm/user32/gdi32.

Actual-device verification: build/verify_device_audio.cmd. One waveOut device was enumerated; CD4 MFPCreateMediaPlayer returned S_OK, subsequent GetState returned PLAYING; resource/itemsho.wav waveOutOpen/PrepareHeader/Write returned success. These are device/API observations, not a claim of audibly hearing playback. The result is stored in build/device_audio_result.txt.

GUI observations: rebuilt executable only, no original executable/save access. A real native window displayed the first-level intro; Space changed to the first-level map. Later screenshots displayed house, fence, ground, player, pickup and five-heart HUD, then park platforms. Further injected Right and Alt-F4 were rejected by the computer-use helper with user-input detection; movement cannot be attributed to those rejected inputs and pause remains unverified. Test game processes were closed before rebuilding. No entire test suite was repeated.
