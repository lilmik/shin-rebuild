#include "../src/graphics/indexed_surface.hpp"
#include <iostream>
#include <stdexcept>
void check(bool b){if(!b)throw std::runtime_error("surface verification failed");}
int main(){
 shin::IndexedSurface source(3,2);source.pixels={1,0xfe,3,4,5,6};
 shin::IndexedSurface dest(3,2,9);dest.blit(source,-1,0);
 check(dest.pixels==std::vector<std::uint8_t>({9,3,9,5,6,9}));
 dest.clear(0);dest.blit(source,0,0,false,0,0,3,2,true);
 check(dest.pixels==std::vector<std::uint8_t>({3,0xfe,1,6,5,4}));
 dest.clear(9);dest.blit(source,0,-1,false);
 check(dest.pixels==std::vector<std::uint8_t>({4,5,6,9,9,9}));
 std::cout<<"indexed surface clipping/key/flip passed\n";
}
