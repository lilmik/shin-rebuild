#include "game/progress_save.hpp"
#include <switch.h>
#include <SDL.h>
#include <SDL_mixer.h>
#include "assets/asset_store.hpp"
#include "game/level_runtime.hpp"
#include "game/stage_intro.hpp"
#include "game/stage_select.hpp"
#include "game/menu.hpp"
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
static std::string lower(std::string s){for(char&c:s){if(c=='\\')c='/';else c=char(std::tolower(static_cast<unsigned char>(c)));}return s;}
class Audio {
 std::map<std::string,std::string> files_;std::map<std::string,Mix_Chunk*> chunks_;std::map<unsigned,int> voices_;std::map<unsigned,Mix_Chunk*>voice_chunks_;Mix_Music*music_=nullptr;unsigned track_=0;std::map<std::string,std::vector<int>>file_channels_;
public:
 Audio(){for(auto&f:fs::recursive_directory_iterator("romfs:/assets"))if(f.is_regular_file())files_[lower(f.path().lexically_relative("romfs:/assets").generic_string())]=f.path().string();}
 ~Audio(){Mix_HaltMusic();if(music_)Mix_FreeMusic(music_);for(auto&c:chunks_)Mix_FreeChunk(c.second);}
 void sound(std::string name,bool loop=false,unsigned id=0){name=lower(name);auto found=files_.find(name);if(found==files_.end())return;auto&chunk=chunks_[name];if(!chunk)chunk=Mix_LoadWAV(found->second.c_str());if(chunk){int ch=Mix_PlayChannel(-1,chunk,loop?-1:0);if(id){voices_[id]=ch;voice_chunks_[id]=chunk;}file_channels_[name].push_back(ch);}}
 void stop(unsigned id){auto i=voices_.find(id);if(i!=voices_.end()&&i->second>=0&&Mix_GetChunk(i->second)==voice_chunks_[id])Mix_HaltChannel(i->second);}
 int original(unsigned id){static const std::map<unsigned,std::string> names={
#include "platform/sound_bindings.inc"
 };auto i=names.find(id);if(i==names.end())throw std::runtime_error("Unbound sound");sound(i->second,false,id);return 1;}
 void stop_file(std::string name){for(int ch:file_channels_[lower(name)])if(ch>=0&&Mix_GetChunk(ch)==chunks_[lower(name)])Mix_HaltChannel(ch);file_channels_[lower(name)].clear();}
 void music(unsigned track){if(track==track_&&music_)return;char file[32];snprintf(file,sizeof(file),"bgm/%02u.mp3",track-1);auto i=files_.find(file);if(i==files_.end())throw std::runtime_error("Missing music");auto*next=Mix_LoadMUS(i->second.c_str());if(!next)throw std::runtime_error(Mix_GetError());if(Mix_PlayMusic(next,-1)){Mix_FreeMusic(next);throw std::runtime_error(Mix_GetError());}auto*previous=music_;music_=next;track_=track;if(previous)Mix_FreeMusic(previous);}
 void stop_music(){track_=0;Mix_HaltMusic();if(music_){Mix_FreeMusic(music_);music_=nullptr;}}
};
static void startup_log(const char* phase,const char* detail="") noexcept{
 mkdir("sdmc:/switch",0777);mkdir("sdmc:/switch/shin_rebuild",0777);
 if(auto*f=fopen("sdmc:/switch/shin_rebuild/startup.log","a")){fprintf(f,"%s %s\n",phase,detail);fflush(f);fclose(f);}
}
int main(){
 SDL_Window*w=nullptr;SDL_Renderer*r=nullptr;
 startup_log("=== SHIN 0.2.42 startup ===");
 try{
 startup_log("romfs_begin");Result mount=romfsInit();if(R_FAILED(mount)){char msg[64];snprintf(msg,sizeof(msg),"romfsInit result=%08x",unsigned(mount));throw std::runtime_error(msg);}
 startup_log("romfs_ready");if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO|SDL_INIT_GAMECONTROLLER)!=0)throw std::runtime_error(SDL_GetError());
 startup_log("sdl_ready");if((Mix_Init(MIX_INIT_MP3)&MIX_INIT_MP3)==0)throw std::runtime_error(Mix_GetError());if(Mix_OpenAudio(48000,AUDIO_S16SYS,2,2048)!=0)throw std::runtime_error(Mix_GetError());Mix_AllocateChannels(32);Mix_VolumeMusic(96);startup_log("audio_ready");
 SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY,"0");
 int display_width=appletGetOperationMode()==AppletOperationMode_Console?1920:1280,display_height=appletGetOperationMode()==AppletOperationMode_Console?1080:720;
 w=SDL_CreateWindow("SHIN",0,0,display_width,display_height,SDL_WINDOW_SHOWN);if(!w)throw std::runtime_error(SDL_GetError());r=SDL_CreateRenderer(w,-1,SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
 if(!r)throw std::runtime_error(SDL_GetError());startup_log("renderer_ready");
 shin::load_game_rules("sdmc:/switch/shin_rebuild/gameplay.ini");
 startup_log("archive_begin");shin::AssetStore assets("romfs:/assets/RESOURCE.RES");auto view=shin::ViewportLayout::for_client(display_width,display_height);shin::IndexedSurface frame(view.logical_width,view.logical_height);frame.origin_x=view.origin_x;frame.origin_y=view.origin_y;auto palette=assets.palette("shin.pal");
 SDL_Texture*texture=SDL_CreateTexture(r,SDL_PIXELFORMAT_ARGB8888,SDL_TEXTUREACCESS_STREAMING,frame.width,frame.height);if(!texture)throw std::runtime_error(SDL_GetError());std::vector<uint32_t>rgba(frame.pixels.size());startup_log("archive_and_texture_ready");Audio audio;startup_log("audio_index_ready");
 PadState pad;padConfigureInput(1,HidNpadStyleSet_NpadStandard);padInitializeDefault(&pad);shin::InputState input;
 std::unique_ptr<shin::LevelRuntime>level;std::unique_ptr<shin::StageIntro>intro;unsigned stage=0;shin::SplashSequence splash;bool starting=true;bool standalone_bonus=false,quit=false,selecting=false,main_menu=true,option_menu=false;shin::MainMenu menu(SDL_GetTicks());shin::OptionMenu options;shin::SpeedConfig speed;int scrolling=0;std::unique_ptr<shin::SettingsMenu> settings;const std::filesystem::path progress_path="sdmc:/switch/shin_rebuild/progress.dat";shin::ProgressSave progress;progress.load(progress_path);if(!std::filesystem::exists(progress_path.string()+".seeded")&&progress.read("romfs:/assets/default_progress.dat")){progress.save(progress_path);std::ofstream(progress_path.string()+".seeded")<<"completed default imported";}shin::StageSelect select(progress.stages,progress.bonus);
 startup_log("progress_and_menu_ready");
 auto start=[&](unsigned n){startup_log("level_create_begin");standalone_bonus=false;selecting=false;palette=assets.palette("shin.pal");stage=n;level=std::make_unique<shin::LevelRuntime>(assets,"romfs:/assets/RESOURCE.RES",int(n));startup_log("level_created");intro.reset();if(n<=6)intro=std::make_unique<shin::StageIntro>(n,true);
 level->unlock_bonus=[&](int i){if(!standalone_bonus&&progress.unlock(i))progress.save(progress_path);};
 level->clock_ms=[](){return SDL_GetTicks();};level->play_sound_file=[&](const std::string&f){audio.sound(f);};level->play_sound_loop=[&](const std::string&f){audio.sound(f,true);};level->stop_sound_file=[&](const std::string&f){audio.stop_file(f);};level->original_sound=[&](unsigned id){return audio.original(id);};level->play_music=[&](unsigned t){audio.music(t);};level->stop_music=[&]{audio.stop_music();};level->change_palette=[&](const char*p){palette=assets.palette(p);};audio.music(n+4);};
 palette=assets.palette("ss.pal");unsigned tick=0;
 auto back_menu=[&](){frame=shin::IndexedSurface(view.logical_width,view.logical_height);frame.origin_x=view.origin_x;frame.origin_y=view.origin_y;main_menu=true;selecting=false;option_menu=false;settings.reset();intro.reset();menu=shin::MainMenu(SDL_GetTicks());palette=assets.palette(shin::MainMenu::palette());audio.music(2);};
 while(appletMainLoop()&&!quit){frame.origin_x=view.origin_x;frame.origin_y=view.origin_y;uint64_t began=SDL_GetPerformanceCounter();SDL_Event e;while(SDL_PollEvent(&e))if(e.type==SDL_QUIT)quit=true;padUpdate(&pad);u64 held=padGetButtons(&pad);
 auto key=[&](unsigned vk,bool down){input.message(down?0x100:0x101,vk);};key(37,held&(HidNpadButton_Left|HidNpadButton_StickLLeft));key(39,held&(HidNpadButton_Right|HidNpadButton_StickLRight));key(38,(held&(HidNpadButton_Up|HidNpadButton_StickLUp))||(!(main_menu||option_menu||settings||selecting)&&(held&HidNpadButton_B)));key(40,held&(HidNpadButton_Down|HidNpadButton_StickLDown));key(32,held&(HidNpadButton_Y|(intro?HidNpadButton_A:0)));key(13,held&(HidNpadButton_Plus|((main_menu||option_menu||settings||selecting||(level&&level->paused()))?HidNpadButton_A:0)));key(27,false);key(117,level&&level->subgame_active()&&(held&HidNpadButton_X));
 if(padGetButtonsDown(&pad)&HidNpadButton_Minus)back_menu();
 if(held&HidNpadButton_StickL)quit=true;
 auto draw=[&](const auto& draws){frame.origin_x=view.origin_x;frame.origin_y=view.origin_y;frame.clip_core=false;frame.border_rgb.clear();frame.clear();for(const auto&d:draws)frame.blit(assets.image(d.resource),d.x,d.y,d.color_key);};
 bool up=input.read(0x46d4b0),down=input.read(0x46d4b4),enter=input.read(0x46d4ec),escape=padGetButtonsDown(&pad)&HidNpadButton_B;
 if(starting){static unsigned warmup=0;if(warmup==0)startup_log("splash_first_frame");shin::preload_bootstrap(assets,warmup++);auto f=splash.update();if(f.palette_phase>=0)palette=assets.palette(f.palette_phase?"rw.pal":"ss.pal");if(f.transition){starting=false;startup_log("splash_complete");back_menu();}else frame.blit(assets.image(f.image_phase?"rw.app":"ss.app"),0,0,false);}
 else if(main_menu){auto f=menu.update(SDL_GetTicks(),{up,down,enter});draw(f.draws);for(auto&a:f.audio)if(a.find("resource/")==0)audio.sound(a);if(f.action==shin::MenuAction::start_game){main_menu=false;selecting=true;select=shin::StageSelect(progress.stages,progress.bonus);palette=assets.palette("shin.pal");audio.music(3);}else if(f.action==shin::MenuAction::options){main_menu=false;option_menu=true;options=shin::OptionMenu();}else if(f.action==shin::MenuAction::quit)quit=true;}
 else if(option_menu){auto f=options.update({up,down,enter,escape});draw(f.draws);if(f.action==shin::OptionAction::main_menu)back_menu();else if(f.action!=shin::OptionAction::none){option_menu=false;settings=std::make_unique<shin::SettingsMenu>(f.action==shin::OptionAction::choice_zero_callback_42f9a0?shin::SettingsMenu::speed_mode:shin::SettingsMenu::scroll_mode,speed);}}
 else if(settings){auto f=settings->update({up,down,bool(input.read(0x46d4b8)),bool(input.read(0x46d4bc)),enter,escape},speed,scrolling);draw(f.draws);if(f.action==shin::SettingsFrame::manual_speed)settings=std::make_unique<shin::SettingsMenu>(shin::SettingsMenu::speed_slider,speed);else if(f.action==shin::SettingsFrame::option_menu){settings.reset();option_menu=true;options=shin::OptionMenu();}else if(f.action==shin::SettingsFrame::main_menu)back_menu();else if(f.action==shin::SettingsFrame::benchmark_auto||f.action==shin::SettingsFrame::benchmark_manual){unsigned began_calibration=SDL_GetTicks();for(int pass=0;pass<10;++pass)frame.blit(assets.image("tt0001.app"),0,0,true);shin::SettingsMenu::complete_calibration(speed,SDL_GetTicks()-began_calibration,f.action==shin::SettingsFrame::benchmark_auto);back_menu();}}
 else if(selecting){if(escape){back_menu();continue;}for(auto&d:select.draw(tick))frame.blit(assets.image(d.resource),d.x,d.y,d.color_key);auto result=select.update({bool(input.read(0x46d4b8)),bool(input.read(0x46d4bc)),bool(input.read(0x46d4b0)),bool(input.read(0x46d4b4)),bool(input.read(0x46d4e8)),bool(input.read(0x46d4ec)),false});if(result.action==shin::SelectResult::start_stage)start(unsigned(result.index));else if(result.action==shin::SelectResult::start_bonus){start(result.index==3?1:0);intro.reset();standalone_bonus=true;shin::game::EntityPayload entry;level->call(result.index==0?0x401d10:result.index==1?0x41c810:result.index==2?0x41ba30:result.index==4?0x4245a0:0x43d120,level->player,entry,{});}}
 else if(intro){auto f=intro->update(SDL_GetTicks(),input.read(0x46d4e8)!=0);for(auto&d:f.draws)frame.blit(assets.image(d.resource),d.x,d.y,d.color_key);for(auto id:f.stop_sounds)audio.stop(id);for(auto id:f.sounds)audio.original(id);if(f.action==shin::CutsceneFrame::resume_level)intro.reset();}
 else{level->tick(input,frame);if(standalone_bonus&&!level->subgame_active()){standalone_bonus=false;selecting=true;select=shin::StageSelect(progress.stages,progress.bonus);palette=assets.palette("shin.pal");audio.music(3);}else if(level->completed()){if(progress.complete(int(stage)))progress.save(progress_path);if(stage<6){int next=stage+1;auto previous=level->player;start(next);for(unsigned o=0x70;o<=0x8c;o+=4)level->player.at(o)=previous.at(o);for(unsigned o=0xc8;o<=0xfc;o+=4)level->player.at(o)=previous.at(o);if(next==1)level->player.at(0x8c)+=3;}else {selecting=true;select=shin::StageSelect(progress.stages,progress.bonus);intro.reset();audio.music(3);}}else if(level->wants_menu()){selecting=true;select=shin::StageSelect(progress.stages,progress.bonus);intro.reset();audio.music(3);}}
 Mix_VolumeMusic(!main_menu&&!selecting&&level&&(intro||level->cinematic_active())?64:96);
 for(size_t i=0;i<rgba.size();++i)rgba[i]=frame.border_rgb.empty()?0xff000000u|palette[frame.pixels[i]]:frame.border_rgb[i]?frame.border_rgb[i]:0xff000000u|palette[frame.pixels[i]];SDL_UpdateTexture(texture,nullptr,rgba.data(),frame.width*4);SDL_RenderClear(r);SDL_RenderCopy(r,texture,nullptr,nullptr);SDL_RenderPresent(r);++tick;
 double elapsed=double(SDL_GetPerformanceCounter()-began)*1000/SDL_GetPerformanceFrequency();int delay=std::clamp(speed.speed_value,1,209);if(elapsed<delay)SDL_Delay(unsigned(delay-elapsed));
 }
 SDL_DestroyTexture(texture);
 }catch(const std::exception&e){startup_log("ERROR",e.what());
 // Release SDL ownership of the display before starting the error console.
 if(r){SDL_DestroyRenderer(r);r=nullptr;}if(w){SDL_DestroyWindow(w);w=nullptr;}
 Mix_CloseAudio();Mix_Quit();SDL_Quit();consoleInit(nullptr);printf("SHIN startup/runtime error:\n%s\nPress + to exit",e.what());PadState errorPad;padConfigureInput(1,HidNpadStyleSet_NpadStandard);padInitializeDefault(&errorPad);while(appletMainLoop()){padUpdate(&errorPad);if(padGetButtonsDown(&errorPad)&HidNpadButton_Plus)break;consoleUpdate(nullptr);}consoleExit(nullptr);}
 if(r)SDL_DestroyRenderer(r);if(w)SDL_DestroyWindow(w);Mix_CloseAudio();Mix_Quit();SDL_Quit();romfsExit();return 0;
}
