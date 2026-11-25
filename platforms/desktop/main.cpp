#include "graphics/gpu/presenter.hpp"
#include "game/dedication_splash.hpp"
#include "game/progress_save.hpp"
#include "core/update_clock.hpp"
#include "core/loading_task.hpp"
#include "core/interface_music_cache.hpp"
#define SDL_MAIN_HANDLED
#include <cstdlib>
#include <fstream>
#include <chrono>
#include <thread>
#include <SDL.h>
#include <SDL_mixer.h>
#define DR_FLAC_IMPLEMENTATION
#include "third_party/dr_flac.h"
#include "platform/flac_wav_stream.hpp"
#include "assets/asset_store.hpp"
#include "game/level_runtime.hpp"
#include "game/stage_intro.hpp"
#include "game/stage_select.hpp"
#include "game/menu.hpp"
#include "game/menu_audio.hpp"
#include "game/startup_sequence.hpp"
#include "game/bootstrap_preload.hpp"
#include "game/options.hpp"
#include "game/settings.hpp"
#include "graphics/viewport.hpp"
#include <filesystem>
#include <map>
#include <memory>
#include <iostream>
#include <algorithm>
#include <cstdio>
#include <sys/stat.h>
namespace fs=std::filesystem;
static fs::path data_root,save_root;
static std::string lower(std::string s){for(char&c:s){if(c=='\\')c='/';else c=char(std::tolower(static_cast<unsigned char>(c)));}return s;}
class Audio {
 std::map<std::string,std::string> files_;std::map<std::string,Mix_Chunk*> chunks_;std::map<unsigned,int> voices_;std::map<unsigned,Mix_Chunk*>voice_chunks_;Mix_Music*music_=nullptr;unsigned track_=0;Mix_Music*prepared_music_=nullptr;unsigned prepared_track_=0;std::map<std::string,std::vector<int>>file_channels_;
 struct MusicDelete{void operator()(Mix_Music*p)const{if(p)Mix_FreeMusic(p);}};
 shin::InterfaceMusicCache<Mix_Music,MusicDelete>interface_music_;
 void release_music(Mix_Music*&music){if(music&&!interface_music_.owns(music))Mix_FreeMusic(music);music=nullptr;}
std::string key(std::string name){name=lower(name);auto dot=name.rfind('.');if(dot!=std::string::npos){auto flac=name.substr(0,dot)+".flac";if(files_.count(flac))return flac;}return name;}
public:
 Audio(){for(auto&f:fs::recursive_directory_iterator(data_root))if(f.is_regular_file())files_[lower(f.path().lexically_relative(data_root).generic_string())]=f.path().string();}
 ~Audio(){stop_music();for(auto&c:chunks_)Mix_FreeChunk(c.second);}
 void sound(std::string name,bool loop=false,unsigned id=0){name=key(name);auto found=files_.find(name);if(found==files_.end())return;auto&chunk=chunks_[name];if(!chunk){if(fs::path(found->second).extension()==".flac"){auto*rw=shin::flac_wav_stream(found->second.c_str());if(rw)chunk=Mix_LoadWAV_RW(rw,1);}else chunk=Mix_LoadWAV(found->second.c_str());if(!chunk)throw std::runtime_error(std::string("Sound decode failed: ")+name+" "+Mix_GetError());}if(chunk){int ch=Mix_PlayChannel(-1,chunk,loop?-1:0);if(id){voices_[id]=ch;voice_chunks_[id]=chunk;}auto&history=file_channels_[name];history.erase(std::remove_if(history.begin(),history.end(),[&](int old){return old==ch||old<0||!Mix_Playing(old)||Mix_GetChunk(old)!=chunk;}),history.end());if(ch>=0)history.push_back(ch);}}
 void stop(unsigned id){auto i=voices_.find(id);if(i!=voices_.end()&&i->second>=0&&Mix_GetChunk(i->second)==voice_chunks_[id])Mix_HaltChannel(i->second);}
 int original(unsigned id){static const std::map<unsigned,std::string> names={
#include "platform/sound_bindings.inc"
 };auto i=names.find(id);if(i==names.end())throw std::runtime_error("Unbound sound");sound(i->second,false,id);return 1;}
 void stop_file(std::string name){for(int ch:file_channels_[key(name)])if(ch>=0&&Mix_GetChunk(ch)==chunks_[key(name)])Mix_HaltChannel(ch);file_channels_[key(name)].clear();}
 void prepare_music(unsigned track,bool force=false){
  if(!force&&((track==track_&&music_)||(track==prepared_track_&&prepared_music_)))return;
  release_music(prepared_music_);prepared_track_=0;
  if(auto*cached=interface_music_.find(track)){prepared_music_=cached;prepared_track_=track;return;}
  char file[32];snprintf(file,sizeof(file),"bgm/%02u.flac",track-1);auto i=files_.find(file);
  if(i==files_.end()){snprintf(file,sizeof(file),"bgm/%02u.wav",track-1);i=files_.find(file);}
  if(i==files_.end())throw std::runtime_error("Missing music");
  Mix_Music*next=nullptr;
  if(fs::path(i->second).extension()==".flac"){auto*rw=shin::flac_wav_stream(i->second.c_str());if(rw)next=Mix_LoadMUS_RW(rw,1);}
  else next=Mix_LoadMUS(i->second.c_str());
  if(!next)throw std::runtime_error(std::string("Music preparation failed: ")+Mix_GetError());
  if(track==2||track==3)next=interface_music_.retain(track,std::unique_ptr<Mix_Music,MusicDelete>(next));
  prepared_music_=next;prepared_track_=track;
 }
 void prepare_interface_music(){}
 void music(unsigned track,bool restart=false){
  if(track==track_&&music_&&!restart)return;prepare_music(track,restart);auto*next=prepared_music_;
  if(Mix_PlayMusic(next,-1))throw std::runtime_error(Mix_GetError());
  auto*previous=music_;music_=next;track_=track;prepared_music_=nullptr;prepared_track_=0;release_music(previous);
 }
 void stop_music(){track_=0;Mix_HaltMusic();release_music(music_);release_music(prepared_music_);prepared_track_=0;}

};
static void startup_log(const char*phase,const char*detail="")noexcept{try{std::ofstream out(save_root/"startup.log",std::ios::app);out<<phase<<" "<<detail<<"\n";}catch(...){}}
int main(int argc,char**argv){
 SDL_SetMainReady();bool windowed=false,fresh=false,verify=false;int initial_stage=-1,boss_stage=-1,limit=0;std::string initial_scene="splash";
 char*base=SDL_GetBasePath();fs::path app_dir=base?fs::path(base):fs::current_path();SDL_free(base);data_root=app_dir/"assets";
 if(!fs::exists(data_root/"RESOURCE.RES")&&fs::exists(app_dir.parent_path()/"Resources/assets/RESOURCE.RES"))data_root=app_dir.parent_path()/"Resources/assets";
 char*pref=SDL_GetPrefPath("libaichuan","shin-rebuild");save_root=pref?fs::path(pref):app_dir/"saves";SDL_free(pref);std::error_code ec;fs::create_directories(save_root,ec);
 try{for(int i=1;i<argc;++i){std::string arg=argv[i];if(arg=="--help"){std::cout<<"SHIN 0.3.26: --assets PATH --windowed --fresh-start --stage 1..7 --boss-stage 1..7 --scene menu --frames N --verify-assets\n";return 0;}else if(arg=="--assets"&&i+1<argc){data_root=fs::absolute(argv[++i]);if(data_root.extension()==".RES"||data_root.extension()==".res")data_root=data_root.parent_path();}else if(arg=="--windowed")windowed=true;else if(arg=="--fresh-start")fresh=true;else if(arg=="--verify-assets")verify=true;else if(arg=="--stage"&&i+1<argc)initial_stage=std::stoi(argv[++i])-1;else if(arg=="--boss-stage"&&i+1<argc)boss_stage=std::stoi(argv[++i])-1;else if(arg=="--scene"&&i+1<argc)initial_scene=argv[++i];else if(arg=="--frames"&&i+1<argc)limit=std::stoi(argv[++i]);else throw std::runtime_error("Unknown argument: "+arg);}if(initial_stage < -1||initial_stage>6||boss_stage < -1||boss_stage>6)throw std::runtime_error("stage must be 1..7");if(initial_stage>=0||boss_stage>=0)fresh=true;if(verify){shin::AssetStore test((data_root/"RESOURCE.RES").string());test.preload_all_images();std::cout<<"Asset decoding OK\n";return 0;}}catch(const std::exception&e){std::cerr<<e.what()<<"\n";return 1;}

 SDL_Window*w=nullptr;SDL_Renderer*r=nullptr;SDL_GLContext gpu_context=nullptr;
 startup_log("=== SHIN 0.3.26 startup ===");
 try{
 SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED,"0");if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO|SDL_INIT_GAMECONTROLLER)!=0)throw std::runtime_error(SDL_GetError());
 startup_log("sdl_ready");Mix_Init(0);if(Mix_OpenAudio(48000,AUDIO_S16SYS,2,2048)!=0)throw std::runtime_error(Mix_GetError());Mix_AllocateChannels(32);Mix_VolumeMusic(96);startup_log("audio_ready");
 SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY,"0");
 int display_width=1280,display_height=720;
 SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,3);SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,
#ifdef __APPLE__
2
#else
3
#endif
);SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,SDL_GL_CONTEXT_PROFILE_CORE);SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);
 w=SDL_CreateWindow("蜡笔小新1-重构版",0,0,display_width,display_height,SDL_WINDOW_SHOWN|SDL_WINDOW_OPENGL|SDL_WINDOW_ALLOW_HIGHDPI|SDL_WINDOW_RESIZABLE|(windowed?0:SDL_WINDOW_FULLSCREEN_DESKTOP));if(!w)throw std::runtime_error(SDL_GetError());gpu_context=SDL_GL_CreateContext(w);if(!gpu_context)throw std::runtime_error(SDL_GetError());if(SDL_GL_MakeCurrent(w,gpu_context)!=0)throw std::runtime_error(SDL_GetError());bool vsync=SDL_GL_SetSwapInterval(1)==0&&SDL_GL_GetSwapInterval()==1;startup_log("vsync",vsync?"1":"0");SDL_GL_GetDrawableSize(w,&display_width,&display_height);
 auto gpu=std::make_unique<shin::GpuPresenter>([](const char*name){return SDL_GL_GetProcAddress(name);});startup_log("gpu_renderer",gpu->renderer().c_str());
 shin::load_game_rules((save_root/"gameplay.ini"));
 auto loading_pump=[](){SDL_PumpEvents();};Audio audio;shin::loading_task([&]{audio.prepare_interface_music();},loading_pump);startup_log("initial_music_cached");
 startup_log("archive_begin");shin::AssetStore assets((data_root/"RESOURCE.RES").string());auto view=shin::ViewportLayout::for_client(display_width,display_height);shin::IndexedSurface frame(view.logical_width,view.logical_height);frame.origin_x=view.origin_x;frame.origin_y=view.origin_y;auto palette=assets.palette("shin.pal");
 std::vector<uint32_t>rgba(frame.pixels.size());startup_log("archive_and_texture_ready");shin::loading_task([&]{assets.preload_all_images();},loading_pump);startup_log("images_cached");
 auto dedication=std::make_unique<shin::DedicationSplash>((data_root/"overrides/dedication.rgba"));shin::IndexedSurface dedication_frame(display_width,display_height);bool dedication_active=false;
 int display_filter=0;unsigned filter_notice_until=0;
 SDL_GameController*controller=nullptr;for(int i=0;i<SDL_NumJoysticks();++i)if(SDL_IsGameController(i)){controller=SDL_GameControllerOpen(i);break;}bool trigger_held=false;shin::InputState input;
 std::unique_ptr<shin::LevelRuntime>level;std::unique_ptr<shin::StageIntro>intro;unsigned stage=0;shin::SplashSequence splash;bool starting=true;bool standalone_bonus=false,quit=false,selecting=false,main_menu=true,option_menu=false;shin::MainMenu menu(SDL_GetTicks());shin::OptionMenu options;shin::SpeedConfig speed;int scrolling=0;std::unique_ptr<shin::SettingsMenu> settings;const std::filesystem::path progress_path=(save_root/"progress.dat");shin::ProgressSave progress;if(!fresh)progress.load(progress_path);shin::StageSelect select(progress.stages,progress.bonus);
 bool save_pending=false;auto save_progress=[&]{std::string error;save_pending=!progress.try_save(progress_path,error);if(save_pending)startup_log("SAVE_WARNING",error.c_str());};
 startup_log("progress_and_menu_ready");
 auto start=[&](unsigned n){shin::loading_task([&]{audio.prepare_music(n+4);},loading_pump);startup_log("level_music_cached");startup_log("level_create_begin");standalone_bonus=false;selecting=false;palette=assets.palette("shin.pal");stage=n;level=shin::loading_task([&]{return std::make_unique<shin::LevelRuntime>(assets,(data_root/"RESOURCE.RES").string(),int(n));},loading_pump);startup_log("level_created");intro.reset();if(n<=6)intro=std::make_unique<shin::StageIntro>(n,true);
 level->unlock_bonus=[&](int i){if(!standalone_bonus&&progress.unlock(i))save_progress();};
 level->clock_ms=[](){return SDL_GetTicks();};level->play_sound_file=[&](const std::string&f){audio.sound(f);};level->play_sound_loop=[&](const std::string&f){audio.sound(f,true);};level->stop_sound_file=[&](const std::string&f){audio.stop_file(f);};level->original_sound=[&](unsigned id){return audio.original(id);};level->play_music=[&](unsigned t){audio.music(t);};level->stop_music=[&]{audio.stop_music();};level->change_palette=[&](const char*p){palette=assets.palette(p);};audio.music(n+4);};
 palette=assets.palette("ss.pal");unsigned tick=0;shin::UpdateClock update_clock;const auto clock_start=SDL_GetPerformanceCounter();
 auto back_menu=[&](){frame=shin::IndexedSurface(view.logical_width,view.logical_height);frame.origin_x=view.origin_x;frame.origin_y=view.origin_y;main_menu=true;selecting=false;option_menu=false;settings.reset();intro.reset();menu=shin::MainMenu(SDL_GetTicks());palette=assets.palette(shin::MainMenu::palette());audio.music(2);};
 if(initial_scene=="menu"){starting=false;back_menu();}if(initial_stage>=0||boss_stage>=0){starting=false;main_menu=false;start(unsigned(boss_stage>=0?boss_stage:initial_stage));if(boss_stage>=0){intro.reset();level->debug_boss_entry(10);}}
 while(!quit){frame.origin_x=view.origin_x;frame.origin_y=view.origin_y;uint64_t began=SDL_GetPerformanceCounter();bool filter_pressed=false,escape=false;SDL_Event event;
 while(SDL_PollEvent(&event)){if(event.type==SDL_QUIT)quit=true;if(event.type==SDL_KEYDOWN&&!event.key.repeat){if(event.key.keysym.sym==SDLK_F10)filter_pressed=true;if(event.key.keysym.sym==SDLK_F11)SDL_SetWindowFullscreen(w,(SDL_GetWindowFlags(w)&SDL_WINDOW_FULLSCREEN_DESKTOP)?0:SDL_WINDOW_FULLSCREEN_DESKTOP);if(event.key.keysym.sym==SDLK_ESCAPE)escape=true;}if(event.type==SDL_CONTROLLERDEVICEADDED&&!controller)controller=SDL_GameControllerOpen(event.cdevice.which);}
 if(controller&&!SDL_GameControllerGetAttached(controller)){SDL_GameControllerClose(controller);controller=nullptr;}
 auto keys=SDL_GetKeyboardState(nullptr);auto button=[&](SDL_GameControllerButton b){return controller&&SDL_GameControllerGetButton(controller,b);};auto axis=[&](SDL_GameControllerAxis a){return controller?SDL_GameControllerGetAxis(controller,a):0;};bool in_minigame=level&&level->subgame_active();bool menus=main_menu||option_menu||settings||selecting;
 auto key=[&](unsigned vk,bool held){input.message(held?0x100:0x101,vk);};key(37,keys[SDL_SCANCODE_LEFT]||button(SDL_CONTROLLER_BUTTON_DPAD_LEFT)||axis(SDL_CONTROLLER_AXIS_LEFTX)<-16000);key(39,keys[SDL_SCANCODE_RIGHT]||button(SDL_CONTROLLER_BUTTON_DPAD_RIGHT)||axis(SDL_CONTROLLER_AXIS_LEFTX)>16000);key(38,keys[SDL_SCANCODE_UP]||button(SDL_CONTROLLER_BUTTON_DPAD_UP)||axis(SDL_CONTROLLER_AXIS_LEFTY)<-16000||(!menus&&!in_minigame&&button(SDL_CONTROLLER_BUTTON_B)));key(40,keys[SDL_SCANCODE_DOWN]||button(SDL_CONTROLLER_BUTTON_DPAD_DOWN)||axis(SDL_CONTROLLER_AXIS_LEFTY)>16000);key(32,keys[SDL_SCANCODE_SPACE]||button(SDL_CONTROLLER_BUTTON_Y)||((intro||in_minigame)&&button(SDL_CONTROLLER_BUTTON_A)));key(13,keys[SDL_SCANCODE_RETURN]||button(SDL_CONTROLLER_BUTTON_START)||(menus&&button(SDL_CONTROLLER_BUTTON_A)));key(27,keys[SDL_SCANCODE_ESCAPE]);key(117,keys[SDL_SCANCODE_F6]||(in_minigame&&button(SDL_CONTROLLER_BUTTON_X)));input.active=(SDL_GetWindowFlags(w)&SDL_WINDOW_INPUT_FOCUS)!=0;
 bool trigger=axis(SDL_CONTROLLER_AXIS_TRIGGERRIGHT)>16000;if(filter_pressed||(trigger&&!trigger_held)){display_filter=(display_filter+1)%3;filter_notice_until=SDL_GetTicks()+2500;}trigger_held=trigger;if(main_menu&&escape)back_menu();
 auto draw=[&](const auto&draws){frame.origin_x=view.origin_x;frame.origin_y=view.origin_y;frame.clip_core=false;frame.border_rgb.clear();frame.clear();for(const auto&d:draws)frame.blit(assets.image(d.resource),d.x,d.y,d.color_key);};bool up=input.read(0x46d4b0),down=input.read(0x46d4b4),enter=input.read(0x46d4ec);
 const bool gameplay=!starting&&!main_menu&&!selecting&&!option_menu&&!settings&&!intro&&level&&!level->paused()&&!level->game_over();
 auto elapsed_ns=std::int64_t(double(SDL_GetPerformanceCounter()-clock_start)*1000000000.0/SDL_GetPerformanceFrequency());
 unsigned updates=update_clock.due(elapsed_ns,gameplay&&speed.speed_value!=16?std::int64_t(std::clamp(speed.speed_value,1,209))*1000000:shin::UpdateClock::display_ns);
 for(unsigned update=0;update<updates&&!quit;++update){
 if(starting&&dedication&&dedication->render(SDL_GetTicks(),dedication_frame)){dedication_active=true;}
 else if(starting){dedication_active=false;dedication.reset();dedication_frame=shin::IndexedSurface{};static unsigned warmup=0;if(warmup==0)startup_log("splash_first_frame");shin::preload_bootstrap(assets,warmup++);auto f=splash.update();if(f.palette_phase>=0)palette=assets.palette(f.palette_phase?"rw.pal":"ss.pal");if(f.transition){starting=false;startup_log("splash_complete");back_menu();draw(menu.update(SDL_GetTicks(),{}).draws);}else frame.blit(assets.image(f.image_phase?"rw.app":"ss.app"),0,0,false);}
 else if(main_menu){auto f=menu.update(SDL_GetTicks(),{up,down,enter});draw(f.draws);shin::dispatch_menu_audio(f,[&](unsigned t,bool restart){audio.music(t,restart);},[&]{audio.stop_music();},[&](const std::string&file){audio.sound(file);});shin::stop_menu_idle_audio(f,[&](const std::string&file){audio.stop_file(file);});if(f.action==shin::MenuAction::start_game){main_menu=false;selecting=true;select=shin::StageSelect(progress.stages,progress.bonus);palette=assets.palette("shin.pal");audio.music(3);}else if(f.action==shin::MenuAction::options){main_menu=false;option_menu=true;options=shin::OptionMenu();}else if(f.action==shin::MenuAction::quit)quit=true;}
 else if(option_menu){auto f=options.update({up,down,enter,escape});draw(f.draws);if(f.stop_cd)audio.stop_music();if(f.action==shin::OptionAction::main_menu)back_menu();else if(f.action!=shin::OptionAction::none){option_menu=false;settings=std::make_unique<shin::SettingsMenu>(f.action==shin::OptionAction::choice_zero_callback_42f9a0?shin::SettingsMenu::speed_mode:shin::SettingsMenu::scroll_mode,speed);}}
 else if(settings){auto f=settings->update({up,down,bool(input.read(0x46d4b8)),bool(input.read(0x46d4bc)),enter,escape},speed,scrolling);draw(f.draws);if(f.stop_cd)audio.stop_music();if(f.action==shin::SettingsFrame::manual_speed)settings=std::make_unique<shin::SettingsMenu>(shin::SettingsMenu::speed_slider,speed);else if(f.action==shin::SettingsFrame::option_menu){settings.reset();option_menu=true;options=shin::OptionMenu();}else if(f.action==shin::SettingsFrame::main_menu)back_menu();else if(f.action==shin::SettingsFrame::benchmark_auto||f.action==shin::SettingsFrame::benchmark_manual){unsigned began_calibration=SDL_GetTicks();for(int pass=0;pass<10;++pass)frame.blit(assets.image("tt0001.app"),0,0,true);shin::SettingsMenu::complete_calibration(speed,SDL_GetTicks()-began_calibration,f.action==shin::SettingsFrame::benchmark_auto);back_menu();}}
 else if(selecting){if(escape){back_menu();continue;}for(auto&d:select.draw(tick))frame.blit(assets.image(d.resource),d.x,d.y,d.color_key);auto result=select.update({bool(input.read(0x46d4b8)),bool(input.read(0x46d4bc)),bool(input.read(0x46d4b0)),bool(input.read(0x46d4b4)),bool(input.read(0x46d4e8)),bool(input.read(0x46d4ec)),false});if(result.action==shin::SelectResult::start_stage)start(unsigned(result.index));else if(result.action==shin::SelectResult::start_bonus){start(result.index==3?1:0);intro.reset();standalone_bonus=true;shin::game::EntityPayload entry;level->call(result.index==0?0x401d10:result.index==1?0x41c810:result.index==2?0x41ba30:result.index==4?0x4245a0:0x43d120,level->player,entry,{});}}
 else if(intro){auto f=intro->update(SDL_GetTicks(),input.read(0x46d4e8)!=0);for(auto&d:f.draws)frame.blit(assets.image(d.resource),d.x,d.y,d.color_key);for(auto id:f.stop_sounds)audio.stop(id);for(auto id:f.sounds)audio.original(id);if(f.action==shin::CutsceneFrame::resume_level)intro.reset();}
 else{level->tick(input,frame);if(standalone_bonus&&!level->subgame_active()){standalone_bonus=false;selecting=true;select=shin::StageSelect(progress.stages,progress.bonus);palette=assets.palette("shin.pal");audio.music(3);}else if(level->completed()){if(progress.complete(int(stage)))save_progress();if(stage<6){int next=stage+1;auto previous=level->player;start(next);for(unsigned o=0x70;o<=0x8c;o+=4)level->player.at(o)=previous.at(o);for(unsigned o=0xc8;o<=0xfc;o+=4)level->player.at(o)=previous.at(o);if(next==1)level->player.at(0x8c)+=3;}else {selecting=true;select=shin::StageSelect(progress.stages,progress.bonus);intro.reset();audio.music(3);}}else if(level->wants_menu()){selecting=true;select=shin::StageSelect(progress.stages,progress.bonus);intro.reset();audio.music(3);}}
 }
 Mix_VolumeMusic(!main_menu&&!selecting&&level&&(intro||level->cinematic_active())?64:96);
 const char*filter_names[]={"1/3 GPU NEAREST","2/3 GPU BILINEAR","3/3 GPU XBRZ"};const char*notice=SDL_GetTicks()<filter_notice_until?filter_names[display_filter]:nullptr;int output_w=0,output_h=0;SDL_GL_GetDrawableSize(w,&output_w,&output_h);
 if(dedication_active)gpu->draw(dedication_frame.border_rgb.data(),dedication_frame.width,dedication_frame.height,output_w,output_h,0,notice);
 else {for(size_t i=0;i<rgba.size();++i)rgba[i]=frame.border_rgb.empty()?0xff000000u|palette[frame.pixels[i]]:frame.border_rgb[i]?frame.border_rgb[i]:0xff000000u|palette[frame.pixels[i]];gpu->draw(rgba.data(),frame.width,frame.height,output_w,output_h,display_filter,notice);}SDL_GL_SwapWindow(w);++tick;if(limit>0&&tick>=unsigned(limit))quit=true;

 double elapsed=double(SDL_GetPerformanceCounter()-began)*1000/SDL_GetPerformanceFrequency();if(!vsync&&elapsed<16.0)SDL_Delay(unsigned(16.0-elapsed));
 }
 if(save_pending)save_progress();if(controller)SDL_GameControllerClose(controller);
 gpu.reset();SDL_GL_DeleteContext(gpu_context);gpu_context=nullptr;
 }catch(const std::exception&e){startup_log("ERROR",e.what());std::cerr<<e.what()<<"\n";if(gpu_context)SDL_GL_DeleteContext(gpu_context);if(w)SDL_DestroyWindow(w);Mix_CloseAudio();Mix_Quit();SDL_Quit();return 1;}
 if(w)SDL_DestroyWindow(w);Mix_CloseAudio();Mix_Quit();SDL_Quit();return 0;
}
