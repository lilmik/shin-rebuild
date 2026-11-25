#include "../src/game/menu.hpp"
#include "../src/game/options.hpp"
#include "../src/game/stage_select.hpp"
#include "../src/game/startup_sequence.hpp"
#include <stdexcept>
#include <iostream>
static void check(bool b,const char* why){if(!b)throw std::runtime_error(why);}
int main(){
 using namespace shin;
 // 421bf0 increments first, then switches phase at exactly 100, returns on 200.
 SplashSequence splash;auto st=splash.update();check(st.palette_phase==0&&st.image_phase==0&&!st.transition,"splash first");
 for(int i=1;i<99;++i)splash.update();st=splash.update();check(st.palette_phase==1&&st.image_phase==1&&!st.transition,"logo and palette switch atomically at tick 100");
 st=splash.update();check(st.palette_phase==1&&st.image_phase==1,"black logo retains matching palette");
 for(int i=101;i<199;++i)splash.update();st=splash.update();check(st.transition&&st.image_phase==-1&&splash.finished,"splash tick 200");check(!splash.update().transition,"splash only one transition");
 SplashSequence atomic;int current_palette=-1;
 for(int i=0;i<200;++i){auto step=atomic.update();if(step.palette_phase>=0)current_palette=step.palette_phase;if(!step.transition)check(current_palette==step.image_phase,"every splash image uses its matching palette");else check(step.palette_phase==-1,"menu transition does not publish a stale logo palette");}
 check(advance_unlocked_stage(1,0)==1&&advance_unlocked_stage(2,0)==2&&advance_unlocked_stage(1,3)==4,"save monotonic");
 MainMenu m(0);auto mf=m.update(0,{});check(mf.audio.size()==1&&mf.draws.size()==6,"main first draw/audio");
 MenuInput down;down.down=true;mf=m.update(1,down);check(m.selected()==1&&mf.draws[3].resource=="menu_g1.app","main draws before navigation");
 m.update(2,down);check(m.selected()==1,"main hold debounce");m.update(3,{});m.update(4,down);check(m.selected()==2,"main released repeat");
 m.update(5,{});m.update(6,down);check(m.selected()==2,"main boundary clamp");
 MenuInput enter;enter.enter=true;check(m.update(7,enter).action==MenuAction::none,"main enter waits release");check(m.update(8,enter).action==MenuAction::none,"main held enter");check(m.update(9,{}).action==MenuAction::quit,"main enter release quit");
 MainMenu wrap(0xfffffff0u);check(wrap.update(0xfffffff0u,{}).audio.size()==1,"wrap first");check(wrap.update(0x10u,{}).audio.empty(),"GetTickCount wrap elapsed");
 MainMenu music(0);music.update(0,{});auto at=music.update(165000,{});check(at.audio.empty()||at.audio.back()!="play_cdaudio_2_to_3","music strict threshold");auto after=music.update(165001,{});bool replay=false;for(auto&a:after.audio)replay|=a=="restart_cdaudio_2_to_3";check(replay,"music replay after threshold");
 OptionMenu o;OptionInput oi;oi.up=oi.down=true;auto of=o.update(oi);check(o.choice()==1&&of.draws[1].resource=="ssc1.app","option down wins/draw old");oi={};oi.enter=true;check(o.update(oi).action==OptionAction::none,"option waits release");oi={};oi.escape=true;of=o.update(oi);check(of.action==OptionAction::choice_one_callback_4405c0&&of.stop_cd,"option release beats escape");
 OptionMenu esc;oi={};oi.escape=true;check(esc.update(oi).action==OptionAction::main_menu,"option immediate escape");
 StageSelect select(7,{true,false,true,false,true});SelectInput sk;sk.right=true;select.update(sk);check(select.stage()==0,"select starts unarmed");select.update({});auto sr=select.update(sk);check(select.stage()==1&&sr.navigation_sound,"select released edge");select.update(sk);check(select.stage()==1,"select holds");select.update({});sk={};sk.down=true;select.update(sk);check(select.stage()==5,"select grid down four");select.update({});sr=select.update(sk);check(select.stage()==6&&!sr.navigation_sound,"select tail clamp silent");
 select.update({});sk={};sk.space=true;select.update(sk);check(select.mode()==0&&select.bonus()==0,"select bonus toggle first unlocked");select.update({});sk={};sk.down=true;select.update(sk);check(select.bonus()==2,"select skips locked bonus");select.update({});select.update(sk);check(select.bonus()==4,"select skips another locked");sk={};sk.enter=true;sr=select.update(sk);check(sr.action==SelectResult::start_bonus&&sr.index==4,"select enter immediate");sk.escape=true;check(select.update(sk).action==SelectResult::main_menu,"select escape wins enter");
 StageSelect locked(1,{});locked.update({});sk={};sk.space=true;locked.update(sk);check(locked.mode()==1,"all locked prevents toggle");sk={};sk.enter=true;check(locked.update(sk).action==SelectResult::start_stage,"stage enter immediate");
 std::cout<<"game flow timing/navigation/release/locked skip passed\n";
}
