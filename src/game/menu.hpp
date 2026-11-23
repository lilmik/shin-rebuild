#pragma once
#include <cstdint>
#include <string>
#include <vector>
namespace shin {
struct MenuInput {bool up=false,down=false,enter=false;};
struct MenuDraw {std::string resource; int x,y; bool color_key=true;};
enum class MenuAction { none,start_game,options,quit };
struct MenuFrame {std::vector<MenuDraw> draws;std::vector<std::string> audio;MenuAction action=MenuAction::none;};
// Reconstructed from 0x4352d0/435370/435900..980. Milliseconds use GetTickCount wrap semantics.
class MainMenu {
 int selected_=0,character_y_=0,title_y_=300,items_x_=0;
 bool up_=false,down_=false,enter_=false,first_=true;
 int variant_=0,frame_=0,frame_ticks_=0;
 std::uint32_t created_,music_,updates_=0;
public:
 explicit MainMenu(std::uint32_t now):created_(now),music_(now){}
 MenuFrame update(std::uint32_t now,MenuInput input);
 int selected() const{return selected_;}
 static constexpr const char* palette(){return "tt0002.pal";}
};
}
