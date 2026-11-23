#pragma once
#include "native_backend.hpp"
#include <mfplay.h>
#include <filesystem>
#include <map>
#include <set>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <deque>
#include <functional>
#include <atomic>
#include <memory>
#include <vector>
#include <stdexcept>
namespace shin::platform {
struct AudioEvent{std::string resource;bool loop;};
struct AudioDispatch{std::string action,resource;unsigned id;bool loop;};
class GameAudio {
 std::filesystem::path root_;bool silent_,com_initialized_=false;IMFPMediaPlayer* music_player_=nullptr;IMFPMediaPlayer* retiring_music_player_=nullptr;HRESULT music_result_=S_OK;std::map<unsigned,std::unique_ptr<WavSound>> voices_;std::map<unsigned,std::string>tracks_;std::vector<AudioDispatch>events_;
 std::map<std::string,unsigned>file_voices_;std::set<std::string>known_sound_files_;
 std::thread sound_thread_;std::mutex sound_mutex_;std::condition_variable sound_ready_,sound_idle_;
 std::deque<std::function<void()>> sound_jobs_;bool sound_stopping_=false,sound_busy_=false;
 std::atomic<unsigned> sound_failures_{0};
 void enqueue_sound(std::function<void()>);void run_sound_worker();
 std::mutex error_mutex_;void report_audio_error(const std::string&);
 float music_volume_=0.75f;std::unique_ptr<FlacMusic>flac_music_,retiring_flac_music_;unsigned flac_loops_=0;
 unsigned active_track_=0;bool music_seen_playing_=false;
 std::unique_ptr<FlacMusic>prepared_music_;unsigned prepared_track_=0;
public:
 std::vector<AudioEvent>journal;
 GameAudio(std::filesystem::path root,bool silent):root_(std::move(root)),silent_(silent){if(!silent_)sound_thread_=std::thread([this]{run_sound_worker();});}
 ~GameAudio();
 void wait_sound_idle();
 unsigned sound_failures()const{return sound_failures_.load();}
 HRESULT music_result()const{return music_result_;}
 bool music_playing()const;
 unsigned active_track()const{return active_track_;}
 void service_music();
 void set_music_volume(float v){if(v==music_volume_)return;music_volume_=v;if(music_player_)music_player_->SetVolume(v);if(flac_music_)flac_music_->set_volume(v);if(retiring_flac_music_)retiring_flac_music_->set_volume(v);}
 static const std::map<unsigned,std::string>&sound_bindings();
 bool play_sound(unsigned id,bool loop=false);bool play_file(const std::string&,unsigned id=0,bool loop=false);
 void play_wav(const std::string&file,bool loop=false){if(!play_file(file,0,loop))report_audio_error("Sound unavailable: "+file);journal.push_back({file,loop});}
 void stop_sound(unsigned);void stop_music();bool play_track(unsigned,bool restart=false);
 bool prepare_track(unsigned);
 void bind_track(unsigned track,std::string file){tracks_[track]=std::move(file);}
 bool load_inmm_playlist();
 const auto&events()const{return events_;}
 void stop(const std::string&relative);
 void stop_all(){if(!silent_)enqueue_sound([this]{voices_.clear();});stop_music();}
};
}
