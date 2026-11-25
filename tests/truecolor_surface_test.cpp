#include "../src/graphics/indexed_surface.hpp"
#include <iostream>
int main(){
 shin::IndexedSurface target(4,2),art(2,1);art.pixels={1,2};art.color_pixels={0xffff0000u,0xff000000u};
 target.blit(art,1,0,false);
 if(target.border_rgb[1]!=0xffff0000u||target.border_rgb[2]!=0xff000000u)return 1;
 shin::IndexedSurface transparent(1,1,254);target.blit(transparent,1,0,true);
 if(target.border_rgb[1]!=0xffff0000u)return 2;
 shin::IndexedSurface foreground(1,1,3);target.blit(foreground,1,0,true);
 if(target.border_rgb[1]!=0||target.pixels[1]!=3)return 3;
 target.clear();for(auto color:target.border_rgb)if(color)return 4;
 target.blit(art,0,0,false,0,0,-1,-1,true);
 if(target.border_rgb[0]!=0xff000000u||target.border_rgb[1]!=0xffff0000u)return 5;
 std::cout<<"Truecolor artwork, indexed foreground, transparency, clear and flipped blits passed\n";
}
