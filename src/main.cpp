#include "assets/asset_store.hpp"
#include "graphics/viewport.hpp"
#include "core/scheduler.hpp"
#include "core/update_clock.hpp"
#include "game/menu.hpp"
#include "game/startup_sequence.hpp"
#include "game/bootstrap_preload.hpp"
#include "game/stage_select.hpp"
#include "game/progress_save.hpp"
#include "game/options.hpp"
#include "game/settings.hpp"
#include "game/level_runtime.hpp"
#include "game/level1_cutscenes.hpp"
#include "game/stage_intro.hpp"
#include "platform/game_audio.hpp"
#include "platform/native_backend.hpp"
#include "platform/frame_pacer.hpp"
#include <chrono>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <memory>
#include <thread>
#include <sstream>

namespace {
void save_frame(const std::string& path,const shin::IndexedSurface& frame,const std::array<std::uint32_t,256>& palette){
 std::ofstream out(path,std::ios::binary);out<<"P6\n"<<frame.width<<" "<<frame.height<<"\n255\n";
 for(std::size_t i=0;i<frame.pixels.size();++i){auto c=frame.border_rgb.empty()?palette[frame.pixels[i]]:frame.border_rgb[i]?frame.border_rgb[i]:palette[frame.pixels[i]];char rgb[]={char(c>>16),char(c>>8),char(c)};out.write(rgb,3);}
}
}
int main(int argc,char** argv){
 bool headless_requested=false;for(int i=1;i<argc;++i)if(std::string(argv[i])=="--headless"||std::string(argv[i])=="--timed-headless")headless_requested=true;
 try {
  bool headless=false,timed_headless=false,skip_intro=false,fresh_start=false;int frames=0,window_width=640,window_height=480,speed_override=-1,boss_stage=-1;std::string archive="assets/RESOURCE.RES",output,initial="splash",script_path,trace_path,settings_override;
  const auto application_dir=std::filesystem::absolute(argv[0]).parent_path();
  if(std::filesystem::is_regular_file(application_dir/"assets"/"RESOURCE.RES"))archive=(application_dir/"assets"/"RESOURCE.RES").string();
  for(int i=1;i<argc;++i){std::string a=argv[i];if(a=="--fresh-start")fresh_start=true;else if(a=="--headless")headless=true;else if(a=="--timed-headless"){headless=true;timed_headless=true;}else if(a=="--speed-ms"&&i+1<argc)speed_override=std::stoi(argv[++i]);
   else if(a=="--frames"&&i+1<argc)frames=std::stoi(argv[++i]);else if(a=="--assets"&&i+1<argc)archive=argv[++i];
   else if(a=="--boss-stage"&&i+1<argc){boss_stage=std::stoi(argv[++i])-1;if(boss_stage<0||boss_stage>=7)throw std::runtime_error("boss-stage must be 1..7");}else if(a=="--skip-intro")skip_intro=true;else if(a=="--settings-file"&&i+1<argc)settings_override=argv[++i];else if(a=="--output"&&i+1<argc)output=argv[++i];else if(a=="--scene"&&i+1<argc)initial=argv[++i];else if(a=="--script"&&i+1<argc)script_path=argv[++i];else if(a=="--trace"&&i+1<argc)trace_path=argv[++i];else if(a=="--width"&&i+1<argc)window_width=std::stoi(argv[++i]);else if(a=="--height"&&i+1<argc)window_height=std::stoi(argv[++i]);else throw std::runtime_error("unknown argument: "+a);}
  if(headless&&frames<=0)throw std::runtime_error("headless mode requires --frames > 0");
  shin::load_game_rules(application_dir/"gameplay.ini");
  auto viewport=shin::ViewportLayout::for_client(window_width,window_height);
  shin::AssetStore assets(archive);shin::IndexedSurface frame(viewport.logical_width,viewport.logical_height);frame.origin_x=viewport.origin_x;frame.origin_y=viewport.origin_y;auto palette=assets.palette("ss.pal");
  shin::Scheduler scheduler;shin::SplashSequence splash;std::unique_ptr<shin::MainMenu> menu;
  shin::InputState silent_input;std::unique_ptr<shin::platform::NativeWindow> window;
  if(!headless)window=std::make_unique<shin::platform::NativeWindow>(L"SHIN - Windows MVP",window_width,window_height);
  shin::platform::GameAudio audio(std::filesystem::path(archive).parent_path(),headless);
  if(!audio.load_inmm_playlist())throw std::runtime_error("missing _inmm.ini music playlist");
  std::unique_ptr<shin::LevelRuntime> level;
  std::unique_ptr<shin::StageIntro> level_intro;
  int ticks=0;const auto host_started=std::chrono::steady_clock::now();
  auto now_ms=[&](){return headless?unsigned(ticks*16):unsigned(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()-host_started).count());};
  bool standalone_bonus=false;bool quit=false;std::string pending,scene="splash";
  struct KeyEvent{int tick;unsigned key;bool pressed;};std::vector<KeyEvent> events;
  if(!script_path.empty()){
   if(!headless)throw std::runtime_error("input script is for headless verification");
   std::ifstream script(script_path);if(!script)throw std::runtime_error("cannot open input script");
   std::string line;while(std::getline(script,line)){if(line.empty()||line[0]=='#')continue;std::istringstream row(line);KeyEvent e{};int down;if(!(row>>e.tick>>e.key>>down))throw std::runtime_error("invalid input script");e.pressed=down!=0;events.push_back(e);}
  }
  auto input=[&]() -> shin::InputState& {return window?window->input():silent_input;};
  auto pressed=[&](std::uint32_t a){return bool(input().read(a));};
  auto render=[&](const std::vector<shin::MenuDraw>& draws){frame.origin_x=viewport.origin_x;frame.origin_y=viewport.origin_y;frame.clip_core=false;frame.border_rgb.clear();frame.clear();for(const auto& d:draws)frame.blit(assets.image(d.resource),d.x,d.y,d.color_key);};
  std::unique_ptr<shin::StageSelect> select;std::unique_ptr<shin::OptionMenu> options;std::unique_ptr<shin::SettingsMenu> settings;
  shin::SpeedConfig speed;int scrolling_mode=0;
  const auto progress_path=(_wgetenv(L"LOCALAPPDATA")?std::filesystem::path(_wgetenv(L"LOCALAPPDATA"))/L"SHIN_Rebuild":application_dir/L"saves")/L"progress.dat";shin::ProgressSave progress;if(!fresh_start){progress.load(progress_path);if(!std::filesystem::exists(progress_path.string()+".seeded")&&progress.read(application_dir/"default_progress.dat")){progress.save(progress_path);std::ofstream(progress_path.string()+".seeded")<<"completed default imported";}}
  const auto settings_path=settings_override.empty()?application_dir/"shin_speed.dat":std::filesystem::path(settings_override);
  std::ifstream speed_file(settings_path,std::ios::binary);
  if(!speed_file)speed_file.open(std::filesystem::path(archive).parent_path()/"RESOURCE"/"speed.dat",std::ios::binary);
  if(speed_file){speed_file.read(reinterpret_cast<char*>(&speed.frame_divisor),4);speed_file.read(reinterpret_cast<char*>(&speed.speed_value),4);}
  if(speed_override>=0)speed.speed_value=speed_override;
  // Legacy skip counts must not reduce modern display cadence to 20/30Hz.
  speed.frame_divisor=1;speed.speed_value=std::clamp(speed.speed_value,0,209);
  auto save_speed=[&]{if(headless&&settings_override.empty())return;std::ofstream out(settings_path,std::ios::binary);out.write(reinterpret_cast<const char*>(&speed.frame_divisor),4);out.write(reinterpret_cast<const char*>(&speed.speed_value),4);if(!out)throw std::runtime_error("cannot save speed settings");};
  std::function<void()> show_menu,show_select,show_options;
  std::function<void(int)> show_level,show_bonus;
  show_level=[&](int stage){
   standalone_bonus=false;

   auto previous_stats=level?std::make_unique<shin::PlayerPayload>(level->player):nullptr;bool advancing=level&&level->completed()&&stage==level->stage_index()+1;
   scene=stage==0?"level1_intro":"stage_"+std::to_string(stage);palette=assets.palette("shin.pal");level=std::make_unique<shin::LevelRuntime>(assets,archive,stage);if(advancing){for(unsigned o=0x70;o<=0x84;o+=4)level->player.at(o)=previous_stats->at(o);level->player.at(0x8c)=previous_stats->at(0x8c)+3;for(unsigned o=0xc8;o<=0xfc;o+=4)level->player.at(o)=previous_stats->at(o);}
   level_intro.reset();if(stage<=6&&!skip_intro){level_intro=std::make_unique<shin::StageIntro>(unsigned(stage));scene=stage==0?"level1_intro":stage==1?"stage2_intro":"flying_intro";}else if(stage==0)scene="level1";
   audio.play_track(unsigned(stage+4));auto first_music=std::make_shared<bool>(true);
   level->clock_ms=now_ms;
   level->unlock_bonus=[&](int i){if(!standalone_bonus&&progress.unlock(i)&&!fresh_start)progress.save(progress_path);};
   level->play_sound_file=[&](const std::string&file){audio.play_wav(file);};
   level->play_sound_loop=[&](const std::string&file){audio.play_wav(file,true);};
   level->stop_sound_file=[&](const std::string&file){audio.stop(file);};
   level->original_sound=[&](unsigned id){return audio.play_sound(id)?1:0;};
   level->play_music=[&,first_music,stage](unsigned track){if(track==unsigned(stage+4)&&*first_music){*first_music=false;return;}if(!audio.play_track(track))throw std::runtime_error("music playback failed");};
   level->stop_music=[&]{audio.stop_music();};level->change_palette=[&](const char*name){palette=assets.palette(name);};
   level->event=[&](unsigned id,shin::PlayerPayload&,std::initializer_list<std::int64_t>)->int{
    throw std::runtime_error("level event not yet implemented: "+std::to_string(id));
   };
   scheduler.create(8,[&,stage](shin::Scheduler::Node& self){if(pressed(0x46d4f0)){self.state=2;show_menu();return;}
    if(level_intro){auto f=level_intro->update(now_ms(),pressed(0x46d4e8));render(f.draws);for(auto sound:f.stop_sounds)audio.stop_sound(sound);for(auto sound:f.sounds)audio.play_sound(sound);if(f.action==shin::CutsceneFrame::resume_level){level_intro.reset();scene=stage==0?"level1":"stage_"+std::to_string(stage);}return;}
    level->tick(input(),frame);if(standalone_bonus&&!level->subgame_active()){standalone_bonus=false;self.state=2;show_select();return;}if(level->completed()){if(progress.complete(level->stage_index())&&!fresh_start)progress.save(progress_path);self.state=2;scene="stage_complete";if(window)SetWindowTextW(window->handle(),L"SHIN - 关卡已通关");if(level->stage_index()<6)show_level(level->stage_index()+1);else show_select();}
    else if(level->wants_menu()){self.state=2;show_menu();}});
  };
  show_bonus=[&](int index){show_level(index==3?1:0);standalone_bonus=true;level_intro.reset();scene="bonus_"+std::to_string(index);shin::game::EntityPayload entry;level->call(index==0?0x401d10:index==1?0x41c810:index==2?0x41ba30:index==4?0x4245a0:0x43d120,level->player,entry,{});};
  std::function<void(shin::SettingsMenu::Page)> show_settings;
  show_settings=[&](shin::SettingsMenu::Page page){
   scene=page==shin::SettingsMenu::speed_mode?"speed_mode":page==shin::SettingsMenu::scroll_mode?"scroll_mode":"speed_slider";
   settings=std::make_unique<shin::SettingsMenu>(page,speed);
   scheduler.create(5,[&](shin::Scheduler::Node& self){
    auto f=settings->update({pressed(0x46d4b0),pressed(0x46d4b4),pressed(0x46d4b8),pressed(0x46d4bc),pressed(0x46d4ec),pressed(0x46d4f0)},speed,scrolling_mode);render(f.draws);
    if(f.action==shin::SettingsFrame::main_menu){self.state=2;show_menu();}
    else if(f.action==shin::SettingsFrame::option_menu){self.state=2;show_options();}
    else if(f.action==shin::SettingsFrame::manual_speed){self.state=2;show_settings(shin::SettingsMenu::speed_slider);}
    else if(f.action==shin::SettingsFrame::benchmark_auto||f.action==shin::SettingsFrame::benchmark_manual){
     auto begin=std::chrono::steady_clock::now();const auto& background=assets.image("tt0001.app");
     for(int i=0;i<10;++i)frame.blit(background,0,0,false);
     auto elapsed=std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()-begin).count();
     shin::SettingsMenu::complete_calibration(speed,static_cast<unsigned>(elapsed),f.action==shin::SettingsFrame::benchmark_auto);
     speed.frame_divisor=1;if(f.action==shin::SettingsFrame::benchmark_auto)speed.speed_value=16;save_speed();
     self.state=2;show_menu();
    }
   });
  };
  show_select=[&]{
   audio.play_track(3);
   scene="stage_select";palette=assets.palette(shin::StageSelect::palette());
   select=std::make_unique<shin::StageSelect>(progress.stages,progress.bonus);
   auto noise=std::make_shared<unsigned>(0);
   scheduler.create(6,[&,noise](shin::Scheduler::Node& self){
    if(!pressed(0x46d4f0))render(select->draw((*noise)++));
    auto r=select->update({pressed(0x46d4b8),pressed(0x46d4bc),pressed(0x46d4b0),pressed(0x46d4b4),pressed(0x46d4e8),pressed(0x46d4ec),pressed(0x46d4f0)});
    if(r.action==shin::SelectResult::main_menu){self.state=2;show_menu();}
    else if(r.action==shin::SelectResult::start_stage){self.state=2;show_level(r.index);}
    else if(r.action==shin::SelectResult::start_bonus){self.state=2;show_bonus(r.index);}
    else if(r.action==shin::SelectResult::start_stage){pending=(r.action==shin::SelectResult::start_stage?"stage_":"bonus_")+std::to_string(r.index);quit=true;}
   });
  };
  show_options=[&]{
   scene="options";options=std::make_unique<shin::OptionMenu>();
   scheduler.create(5,[&](shin::Scheduler::Node& self){
    auto f=options->update({pressed(0x46d4b0),pressed(0x46d4b4),pressed(0x46d4ec),pressed(0x46d4f0)});render(f.draws);
    if(f.action==shin::OptionAction::main_menu){self.state=2;show_menu();}
    else if(f.action!=shin::OptionAction::none){self.state=2;show_settings(f.action==shin::OptionAction::choice_zero_callback_42f9a0?shin::SettingsMenu::speed_mode:shin::SettingsMenu::scroll_mode);}
   });
  };
  show_menu=[&]{
   audio.play_track(2);
   scene="main_menu";menu=std::make_unique<shin::MainMenu>(now_ms());palette=assets.palette(shin::MainMenu::palette());
   scheduler.create(5,[&](shin::Scheduler::Node& self){
    auto& keys=input();
    auto draw=menu->update(now_ms(),{bool(keys.read(0x46d4b0)),bool(keys.read(0x46d4b4)),bool(keys.read(0x46d4ec))});
    render(draw.draws);
    if(draw.action==shin::MenuAction::quit)quit=true;
    else if(draw.action==shin::MenuAction::start_game){self.state=2;show_select();}
    else if(draw.action==shin::MenuAction::options){self.state=2;show_options();}
   });
  };
  if(boss_stage>=0){show_level(boss_stage);level_intro.reset();scene="stage_"+std::to_string(boss_stage);level->debug_boss_entry(10);}else if(initial=="stage7_boss"||initial=="stage7_boss_weapons"){show_level(6);level_intro.reset();scene="stage_6";level->debug_final_boss_entry(initial=="stage7_boss_weapons");}else if(initial=="bonus_card")show_bonus(1);else if(initial=="bonus_puzzle")show_bonus(4);else if(initial=="shoot")show_level(2);else if(initial=="bonus_mole")show_bonus(3);else if(initial=="bonus_car")show_bonus(0);else if(initial=="bonus_flag")show_bonus(2);else if(initial=="stage2_mole"){show_level(1);level_intro.reset();scene="stage_1";shin::game::EntityPayload entry;level->call(0x43d120,level->player,entry,{});}else if(initial=="gameover"){show_level(0);level_intro.reset();scene="game_over";level->call(0x44d790,level->player,{});}else if(initial=="level1")show_level(0);else if(initial=="stage2")show_level(1);else if(initial=="stage3")show_level(3);else if(initial=="stage4")show_level(4);else if(initial=="stage5")show_level(5);else if(initial=="stage6")show_level(6);else if(initial=="menu")show_menu();else if(initial=="select")show_select();else if(initial=="options"){palette=assets.palette(shin::MainMenu::palette());show_options();}
  else if(initial=="speed"||initial=="scroll"||initial=="slider"){palette=assets.palette(shin::MainMenu::palette());show_settings(initial=="speed"?shin::SettingsMenu::speed_mode:initial=="scroll"?shin::SettingsMenu::scroll_mode:shin::SettingsMenu::speed_slider);}
  else if(initial=="splash")scheduler.create(5,[&](shin::Scheduler::Node& self){
   static unsigned warmup=0;shin::preload_bootstrap(assets,warmup++);auto step=splash.update();if(step.palette_phase>=0)palette=assets.palette(step.palette_phase?"rw.pal":"ss.pal");
   if(!step.transition){frame.blit(assets.image(step.image_phase?"rw.app":"ss.app"),0,0,false);return;}
   self.state=2;show_menu();
  });else throw std::runtime_error("invalid scene");
  std::ofstream trace;if(!trace_path.empty()){trace.open(trace_path);if(!trace)throw std::runtime_error("cannot open trace output");trace<<"tick,scene,x,y,state,lives,score,camera_x,camera_y,completed,up,down,left,right,space,enter,escape\n";}
  shin::platform::FramePacer pacer;shin::UpdateClock update_clock;
  auto display_deadline=std::chrono::steady_clock::now();const auto measured_start=display_deadline;
  unsigned simulation_updates=0,presentations=0;std::vector<double> frame_times;
  while(!quit&&(frames==0||ticks<frames)){
   auto frame_started=std::chrono::steady_clock::now();
   if(window&&!window->pump())break;audio.service_music();
   if(window){auto size=window->client_size();auto layout=shin::ViewportLayout::for_client(size[0],size[1]);viewport=layout;if(frame.width!=layout.logical_width||frame.height!=layout.logical_height){frame=shin::IndexedSurface(layout.logical_width,layout.logical_height);frame.origin_x=layout.origin_x;frame.origin_y=layout.origin_y;}}
   for(const auto& e:events)if(e.tick==ticks)silent_input.message(e.pressed?0x100:0x101,e.key);
   unsigned updates=1;
   const bool gameplay=level&&!level_intro&&!level->paused()&&!level->game_over()&&(scene=="level1"||scene.rfind("stage_",0)==0);
   if(window){auto elapsed=std::chrono::duration_cast<std::chrono::nanoseconds>(frame_started-measured_start).count();updates=update_clock.due(elapsed,gameplay&&speed.speed_value!=16?std::int64_t(std::clamp(speed.speed_value,1,209))*1000000:shin::UpdateClock::display_ns);}
   for(unsigned i=0;i<updates;++i){scheduler.tick();++simulation_updates;if(quit)break;}
   if(trace&&level)trace<<ticks<<","<<scene<<","<<level->player.at(8)<<","<<level->player.at(12)<<","<<level->player.at(0x5c)<<","<<level->player.at(0x8c)<<","<<level->player.at(0x70)<<","<<level->camera_x()<<","<<level->camera_y()<<","<<level->completed()<<","<<pressed(0x46d4b0)<<","<<pressed(0x46d4b4)<<","<<pressed(0x46d4b8)<<","<<pressed(0x46d4bc)<<","<<pressed(0x46d4e8)<<","<<pressed(0x46d4ec)<<","<<pressed(0x46d4f0)<<"\n";
   audio.set_music_volume(level&&(level_intro||level->cinematic_active())&&(scene=="level1"||scene=="level1_intro"||scene=="stage2_intro"||scene=="flying_intro"||scene.rfind("stage_",0)==0)?0.50f:0.75f);
   if(window){window->present(frame.pixels.data(),palette,frame.width,frame.height,frame.border_rgb.empty()?nullptr:frame.border_rgb.data());++presentations;}++ticks;
   if(!headless||timed_headless){
    auto interval=window?std::chrono::nanoseconds(shin::UpdateClock::display_ns):std::chrono::nanoseconds(std::int64_t(std::clamp(speed.speed_value,1,209))*1000000);
    display_deadline+=interval;
    // Keep fractional deadlines, but discard a long stall rather than burning
    // seconds replaying old frames after a window drag or breakpoint.
    auto now=std::chrono::steady_clock::now();if(now>display_deadline+interval*4)display_deadline=now;
    pacer.wait_until(display_deadline);
    frame_times.push_back(std::chrono::duration<double,std::milli>(std::chrono::steady_clock::now()-frame_started).count());
   }
  }
  if(!output.empty())save_frame(output,frame,palette);
  if(level)std::cout<<"player="<<level->player.at(8)<<","<<level->player.at(12)<<" camera="<<level->camera_x()<<","<<level->camera_y()<<"\n";
  std::cout<<"updates="<<ticks<<" scene="<<scene<<" speed_ms="<<std::clamp(speed.speed_value,1,209)<<"\n";
  if(!frame_times.empty()){std::sort(frame_times.begin(),frame_times.end());double elapsed=std::chrono::duration<double>(std::chrono::steady_clock::now()-measured_start).count();std::cout<<"simulation_updates="<<simulation_updates<<" presentations="<<presentations<<" elapsed_s="<<elapsed<<" frame_p50_ms="<<frame_times[frame_times.size()/2]<<" frame_p95_ms="<<frame_times[frame_times.size()*95/100]<<"\n";}
  if(!pending.empty()){std::cerr<<"Scene transition not integrated: "<<pending<<"\n";return 3;}
  return 0;
 }catch(const std::exception& e){std::cerr<<e.what()<<"\n";if(!headless_requested)MessageBoxA(nullptr,e.what(),"SHIN",MB_OK|MB_ICONERROR);return 1;}
}
