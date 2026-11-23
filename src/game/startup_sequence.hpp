#pragma once
#include <cstdint>
namespace shin {
// Reconstructed from FUN_00421bf0. Rendering/palette application supplied by host.
struct SplashSequence {
 int phase=0,ticks=0; bool finished=false;
 struct Step { int palette_phase; int image_phase; bool transition; };
 Step update(){
  if(finished)return {-1,-1,false};
  int palette=ticks==0?phase:-1;
  if(++ticks==100){ticks=0;if(++phase==2){finished=true;return {palette,-1,true};}}
  return {palette,phase,false};
 }
};
// FUN_0043ee50 stores stage+1 only when it advances the previous value.
// A missing save.dat supplies stored=1. I/O failures in original are unchecked.
inline std::int32_t advance_unlocked_stage(std::int32_t stored,std::int32_t stage){
 return (std::int64_t(stored)-1<stage)?stage+1:stored;
}
}
