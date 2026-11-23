#include "game_audio.hpp"
#include <fstream>
#include <cctype>
namespace shin::platform {
void GameAudio::report_audio_error(const std::string&message){
 std::lock_guard<std::mutex>lock(error_mutex_);std::error_code ec;auto path=root_.parent_path()/"audio_error.log";
 if(std::filesystem::file_size(path,ec)>256*1024&&!ec){std::ofstream clear(path,std::ios::trunc);}
 std::ofstream out(path,std::ios::app);out<<GetTickCount64()<<" "<<message<<"\n";
}
void GameAudio::enqueue_sound(std::function<void()> job){
 {std::lock_guard<std::mutex> lock(sound_mutex_);sound_jobs_.push_back(std::move(job));}sound_ready_.notify_one();
}
void GameAudio::run_sound_worker(){
 for(;;){std::function<void()> job;{
  std::unique_lock<std::mutex> lock(sound_mutex_);sound_ready_.wait(lock,[this]{return sound_stopping_||!sound_jobs_.empty();});
  if(sound_jobs_.empty()&&sound_stopping_)break;
  job=std::move(sound_jobs_.front());sound_jobs_.pop_front();sound_busy_=true;
 }try{job();}catch(const std::exception&e){++sound_failures_;report_audio_error(e.what());}catch(...){++sound_failures_;report_audio_error("Unknown sound worker failure");}
 {std::lock_guard<std::mutex> lock(sound_mutex_);sound_busy_=false;if(sound_jobs_.empty())sound_idle_.notify_all();}
 }
 voices_.clear();
}
void GameAudio::wait_sound_idle(){if(silent_)return;std::unique_lock<std::mutex> lock(sound_mutex_);sound_idle_.wait(lock,[this]{return sound_jobs_.empty()&&!sound_busy_;});}
GameAudio::~GameAudio(){
 if(sound_thread_.joinable()){{std::lock_guard<std::mutex> lock(sound_mutex_);sound_stopping_=true;}sound_ready_.notify_one();sound_thread_.join();}
 stop_music();if(com_initialized_)CoUninitialize();
}

// _inmmserv 40de80 constructs one empty entry before appending ordinary INI
// lines. 409ae0 selects entry track-1: CD2 therefore selects INI line 1.
bool GameAudio::load_inmm_playlist(){std::ifstream in(root_/"_inmm.ini");if(!in)return false;std::string line;unsigned track=2;std::map<unsigned,std::string> parsed;
 while(std::getline(in,line)){if(!line.empty()&&line.back()=='\r')line.pop_back();if(line.empty()||line[0]=='#')continue;if(line[0]=='[')throw std::runtime_error("sectioned INMM playlist not supported");parsed[track++]=line;}
 tracks_=std::move(parsed);return !tracks_.empty();
}
const std::map<unsigned,std::string>& GameAudio::sound_bindings(){static const std::map<unsigned,std::string> bindings={
#include "sound_bindings.inc"
};return bindings;}
namespace {
std::string sound_key(std::string name){for(char&c:name)c=c=='\\'?'/':char(std::tolower(static_cast<unsigned char>(c)));auto dot=name.rfind('.');if(dot!=std::string::npos)name.replace(dot,std::string::npos,".wav");return name;}
}
void GameAudio::stop(const std::string&relative){
 const auto key=sound_key(relative);std::set<unsigned>ids;
 for(const auto&entry:file_voices_)if(sound_key(entry.first)==key)ids.insert(entry.second);
 for(const auto&entry:sound_bindings())if(sound_key(entry.second)==key)ids.insert(entry.first);
 for(auto id:ids)stop_sound(id);
}
bool GameAudio::play_sound(unsigned id,bool loop){auto it=sound_bindings().find(id);if(it==sound_bindings().end())throw std::runtime_error("unbound original sound global");return play_file(it->second,id,loop);}
bool GameAudio::play_file(const std::string& relative,unsigned id,bool loop){
 if(id==0){auto it=file_voices_.find(relative);if(it==file_voices_.end())it=file_voices_.emplace(relative,0x80000000u+unsigned(file_voices_.size())).first;id=it->second;}
 auto path=root_/std::filesystem::path(relative);auto flac=path;flac.replace_extension(".flac");if(std::filesystem::is_regular_file(flac))path=flac;if(!known_sound_files_.count(relative)){if(!std::filesystem::is_regular_file(path)){events_.push_back({"missing_sound",relative,id,loop});return false;}known_sound_files_.insert(relative);}
 events_.push_back({"play_sound",relative,id,loop});if(silent_)return true;
 enqueue_sound([this,path,id,loop]{auto& voice=voices_[id];if(!voice){voice=std::make_unique<WavSound>();if(!voice->load(path.wstring())){voice.reset();++sound_failures_;report_audio_error("Decode failed: "+path.string());return;}}if(!voice->play(loop)){++sound_failures_;report_audio_error("Output failed: "+path.string());}});
 return true;
}
void GameAudio::stop_sound(unsigned id){events_.push_back({"stop_sound","",id,false});if(!silent_)enqueue_sound([this,id]{auto it=voices_.find(id);if(it!=voices_.end())it->second->pause();});}
bool GameAudio::prepare_track(unsigned track){
 if(silent_||active_track_==track||prepared_track_==track)return true;
 prepared_music_.reset();prepared_track_=0;
 auto it=tracks_.find(track);if(it==tracks_.end())return false;
 auto file=root_/it->second;if(file.extension()!=".flac")return true;
 auto next=std::make_unique<FlacMusic>();if(!next->start(file.wstring(),music_volume_,true))return false;
 auto begin=GetTickCount64();while(!next->buffered()){if(GetTickCount64()-begin>=10000)return false;Sleep(1);}
 prepared_music_=std::move(next);prepared_track_=track;return true;
}
bool GameAudio::play_track(unsigned track,bool restart){
 if(active_track_==track&&!restart)return true;auto it=tracks_.find(track);if(it==tracks_.end())throw std::runtime_error("unverified CD track mapping: bind_track required");auto file=root_/it->second;if(!std::filesystem::is_regular_file(file))throw std::runtime_error("missing music asset");
 if(silent_){active_track_=track;events_.push_back({"play_track",it->second,track,true});return true;}
 if(file.extension()==".flac"){
  std::unique_ptr<FlacMusic>next;
  if(prepared_track_==track){next=std::move(prepared_music_);prepared_track_=0;next->set_volume(music_volume_);next->resume();}
  else{prepared_music_.reset();prepared_track_=0;next=std::make_unique<FlacMusic>();if(!next->start(file.wstring(),music_volume_)){music_result_=E_FAIL;return false;}}
  if(retiring_flac_music_)flac_music_.reset();else retiring_flac_music_=std::move(flac_music_);
  flac_music_=std::move(next);flac_loops_=0;active_track_=track;music_seen_playing_=false;music_result_=S_OK;events_.push_back({"play_track",it->second,track,true});return true;
 }
 flac_music_.reset();retiring_flac_music_.reset();
 if(!com_initialized_){auto result=CoInitializeEx(nullptr,COINIT_APARTMENTTHREADED);if(SUCCEEDED(result))com_initialized_=true;else if(result!=RPC_E_CHANGED_MODE){music_result_=result;return false;}}
 IMFPMediaPlayer* next=nullptr;music_result_=MFPCreateMediaPlayer(file.wstring().c_str(),TRUE,MFP_OPTION_NONE,nullptr,nullptr,&next);if(FAILED(music_result_))return false;next->SetVolume(music_volume_);
 // Keep the previous decoder playing while the next file opens asynchronously.
 if(retiring_music_player_){if(music_player_){music_player_->Shutdown();music_player_->Release();}}
 else retiring_music_player_=music_player_;
 music_player_=next;active_track_=track;music_seen_playing_=false;events_.push_back({"play_track",it->second,track,true});return true;
}
bool GameAudio::music_playing()const{if(flac_music_)return flac_music_->playing();if(!music_player_)return false;MFP_MEDIAPLAYER_STATE state=MFP_MEDIAPLAYER_STATE_EMPTY;return SUCCEEDED(music_player_->GetState(&state))&&state==MFP_MEDIAPLAYER_STATE_PLAYING;}
void GameAudio::service_music(){
 if(events_.size()>4096)events_.erase(events_.begin(),events_.end()-2048);
 if(journal.size()>4096)journal.erase(journal.begin(),journal.end()-2048);
 if(flac_music_){if(flac_music_->playing()){
  music_seen_playing_=true;if(retiring_flac_music_){retiring_flac_music_.reset();events_.push_back({"music_handoff",tracks_.at(active_track_),active_track_,true});}
  if(retiring_music_player_){retiring_music_player_->Shutdown();retiring_music_player_->Release();retiring_music_player_=nullptr;}
  if(music_player_){music_player_->Shutdown();music_player_->Release();music_player_=nullptr;}
  unsigned loops=flac_music_->loops();if(loops!=flac_loops_){flac_loops_=loops;events_.push_back({"loop_music",tracks_.at(active_track_),active_track_,true});}
 }return;}
 if(!music_player_||!active_track_)return;MFP_MEDIAPLAYER_STATE state;music_result_=music_player_->GetState(&state);if(FAILED(music_result_))return;
 if(state==MFP_MEDIAPLAYER_STATE_PLAYING){music_seen_playing_=true;if(retiring_music_player_){retiring_music_player_->Shutdown();retiring_music_player_->Release();retiring_music_player_=nullptr;events_.push_back({"music_handoff",tracks_.at(active_track_),active_track_,true});}return;}
 if(state!=MFP_MEDIAPLAYER_STATE_STOPPED||!music_seen_playing_)return;
 PROPVARIANT start{};start.vt=VT_I8;start.hVal.QuadPart=0;music_result_=music_player_->SetPosition(GUID{},&start);
 if(SUCCEEDED(music_result_))music_result_=music_player_->Play();
 if(SUCCEEDED(music_result_)){events_.push_back({"loop_music",tracks_.at(active_track_),active_track_,true});music_seen_playing_=false;}
}
void GameAudio::stop_music(){prepared_music_.reset();prepared_track_=0;flac_music_.reset();retiring_flac_music_.reset();flac_loops_=0;if(retiring_music_player_){retiring_music_player_->Shutdown();retiring_music_player_->Release();retiring_music_player_=nullptr;}active_track_=0;music_seen_playing_=false;if(music_player_){music_player_->Shutdown();music_player_->Release();music_player_=nullptr;}}
}
