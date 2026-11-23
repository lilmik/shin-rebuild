#include "menu.hpp"
namespace shin {
MenuFrame MainMenu::update(std::uint32_t now,MenuInput in){
 MenuFrame out;
 if(first_){out.audio.push_back("play_cdaudio_2_to_3");music_=now;first_=false;}
 if(variant_<20 && std::uint32_t(now-created_)>std::uint32_t((variant_+1)*10000)){
  ++variant_;frame_=0;
  if(variant_>=4){out.audio.push_back("resource/_1.wav");out.audio.push_back("resource/_2.wav");out.audio.push_back("resource/_3.wav");}
 }
 if(std::uint32_t(now-music_)>165000){out.audio.push_back("restart_cdaudio_2_to_3");music_=now;}
 static const char* chars[]={"tt0000.app","tt0003.app","tt0004.app","tt0011.app","tt0021.app","tt0022.app","tt0031.app","tt0032.app","tt0041.app","tt0042.app"};
 static const int sequence[5][3]={{0,1,2},{2,3,0},{4,5,0},{6,7,0},{8,9,0}};
 int set=variant_<4?variant_:4;
 out.draws.push_back({"tt0001.app",0,0,true});
 out.draws.push_back({chars[sequence[set][frame_]],50,character_y_,true});
 out.draws.push_back({"tt0002.app",50,title_y_,true});
 static const char* items[3][2]={{"menu_g0.app","menu_g1.app"},{"option2.app","option1.app"},{"menu_e0.app","menu_e1.app"}};
 for(int i=0;i<3;++i){bool active=selected_==i;int x=items_x_-(i==1?2:0);int y=180+i*40-(i==1&&!active?4:0);out.draws.push_back({items[i][active],x,y,true});}
 character_y_+=(160-character_y_)/2;title_y_+=(20-title_y_)/2;items_x_+=(460-items_x_)/2;
 if(!in.up)up_=false;else if(!up_){if(selected_>0)--selected_;up_=true;}
 if(!in.down)down_=false;else if(!down_){if(selected_<2)++selected_;down_=true;}
 if(in.enter)enter_=true;
 else if(enter_){enter_=false;out.audio.push_back("stop_cdaudio");out.action=selected_==0?MenuAction::start_game:selected_==1?MenuAction::options:MenuAction::quit;return out;}
 if(++frame_ticks_==12){frame_ticks_=0;if(++frame_==(set==0?3:2))frame_=0;}
 ++updates_;return out;
}
}
