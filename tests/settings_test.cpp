#include "../src/game/settings.hpp"
#include <stdexcept>
void require(bool b){if(!b)throw std::runtime_error("settings verification");}
int main(){
 shin::SpeedConfig speed;int scroll=0;
 shin::SettingsMenu automatic(shin::SettingsMenu::speed_mode,speed);
 automatic.update({false,false,false,false,true,false},speed,scroll);
 auto f=automatic.update({},speed,scroll);require(f.action==shin::SettingsFrame::benchmark_auto);
 shin::SettingsMenu::complete_calibration(speed,499,true);require(speed.frame_divisor==1&&speed.speed_value==16&&speed.persist);
 shin::SettingsMenu::complete_calibration(speed,500,true);require(speed.frame_divisor==2&&speed.speed_value==32);
 shin::SettingsMenu::complete_calibration(speed,1200,true);require(speed.frame_divisor==3&&speed.speed_value==48);
 shin::SettingsMenu slider(shin::SettingsMenu::speed_slider,speed);
 for(int i=0;i<300;++i)slider.update({false,false,true,false,false,false},speed,scroll);
 require(slider.slider()==0);
 for(int i=0;i<300;++i)slider.update({false,false,false,true,false,false},speed,scroll);
 require(slider.slider()==209);
 slider.update({false,false,false,false,true,false},speed,scroll);
 f=slider.update({},speed,scroll);require(f.action==shin::SettingsFrame::benchmark_manual&&speed.speed_value==0);
 shin::SettingsMenu mode(shin::SettingsMenu::scroll_mode,speed);
 mode.update({false,false,false,true,true,false},speed,scroll);
 f=mode.update({},speed,scroll);require(f.action==shin::SettingsFrame::main_menu&&scroll==1);
}
