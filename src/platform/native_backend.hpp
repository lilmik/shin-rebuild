#pragma once
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <mmsystem.h>
#include <array>
#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include "../core/input_state.hpp"
namespace shin {class GpuPresenter;}
namespace shin::platform {
class NativeWindow {
 HWND window_{}; int width_,height_; shin::InputState input_; std::vector<std::uint32_t> rgb_;
 HDC back_dc_{}; HBITMAP back_bitmap_{}; HGDIOBJ back_previous_{}; int back_width_=0,back_height_=0;
 bool fullscreen_=false;WINDOWPLACEMENT windowed_placement_{sizeof(WINDOWPLACEMENT)};LONG_PTR windowed_style_=0,windowed_ex_style_=0;
 int scale_mode_=0;ULONGLONG scale_notice_until_=0;std::vector<std::uint32_t> scaled_rgb_,pixel_art_rgb_;
 void cycle_scale_mode();
 bool vsync_enabled_=false;
 HDC gpu_dc_{};HGLRC gpu_context_{};std::unique_ptr<shin::GpuPresenter> gpu_;
 void toggle_fullscreen();
 void release_backbuffer();
 static LRESULT CALLBACK proc(HWND,UINT,WPARAM,LPARAM);
 void paint(HDC);
public:
 explicit NativeWindow(const wchar_t* title=L"SHIN reconstruction",int width=640,int height=480,bool start_fullscreen=true);
 ~NativeWindow(); NativeWindow(const NativeWindow&)=delete; NativeWindow& operator=(const NativeWindow&)=delete;
 bool pump(); void present(const std::uint8_t*,const std::array<std::uint32_t,256>&);
 shin::InputState& input(){return input_;}
 bool vsync_enabled()const{return vsync_enabled_;}
 HWND handle()const{return window_;}
 std::array<int,2> client_size()const{RECT r{};GetClientRect(window_,&r);return {r.right,r.bottom};}
 void present(const std::uint8_t*p,const std::array<std::uint32_t,256>&palette,int w,int h,const std::uint32_t*border=nullptr);
};
// PCM WAV through waveOut; each object owns one independent voice.
class WavSound {
 friend struct AudioDecodeProbe;
 HWAVEOUT device_{}; WAVEHDR header_{}; std::vector<char> data_; WAVEFORMATEX format_{}; bool prepared_=false;
public:
 ~WavSound(){stop();} WavSound()=default; WavSound(const WavSound&)=delete; WavSound& operator=(const WavSound&)=delete;
 bool load(const std::wstring&); bool play(bool loop=false); void stop(); void pause(){if(device_)waveOutReset(device_);}
};
// Decoder and waveOut streaming do not depend on OS FLAC codecs.
class FlacMusic {
 struct State;std::unique_ptr<State> state_;
public:
 struct PcmTrack{std::vector<std::int16_t>samples;unsigned channels=0,rate=0;};
 using Track=std::shared_ptr<const PcmTrack>;
 static Track load_pcm(const std::wstring&);
 FlacMusic();~FlacMusic();
 bool start(const std::wstring&,float volume,bool paused=false,bool stream=false,Track cached={});void stop();void set_volume(float);
 bool buffered()const;void resume();
 bool playing()const;unsigned loops()const;
};

}

