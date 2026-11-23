#include "../src/platform/game_audio.hpp"
#include <iostream>
#include <cstring>
namespace shin::platform {
struct AudioDecodeProbe {
 static bool same(WavSound&a,WavSound&b){if(a.format_.nChannels!=b.format_.nChannels||a.format_.nSamplesPerSec!=b.format_.nSamplesPerSec||b.format_.wBitsPerSample!=16)return false;size_t samples=a.data_.size()/(a.format_.wBitsPerSample/8);if(b.data_.size()!=samples*2)return false;for(size_t i=0;i<samples;++i){short expected=0,actual=0;if(a.format_.wBitsPerSample==8)expected=short((int(static_cast<unsigned char>(a.data_[i]))-128)*256);else if(a.format_.wBitsPerSample==16)memcpy(&expected,a.data_.data()+i*2,2);else return false;memcpy(&actual,b.data_.data()+i*2,2);if(expected!=actual)return false;}return true;}
};
}
int main(int argc,char**argv){if(argc!=3)return 1;try{int count=0;for(auto dir:{"bgm","RESOURCE"})for(auto&p:std::filesystem::directory_iterator(std::filesystem::path(argv[2])/dir))if(p.path().extension()==".flac"){auto original=std::filesystem::path(argv[1])/dir/p.path().filename();original.replace_extension(dir==std::string("RESOURCE")?".WAV":".wav");shin::platform::WavSound wav,flac;if(!wav.load(original.wstring())||!flac.load(p.path().wstring())||!shin::platform::AudioDecodeProbe::same(wav,flac)){std::cerr<<"Decode mismatch: "<<p.path()<<"\n";return 2;}++count;}
 shin::platform::GameAudio audio(argv[2],false);audio.play_wav("RESOURCE/MENU.WAV",true);audio.wait_sound_idle();if(audio.sound_failures())return 3;audio.stop("RESOURCE/MENU.WAV");audio.wait_sound_idle();audio.play_wav("RESOURCE/DEM-SHIN.WAV");audio.wait_sound_idle();if(audio.sound_failures())return 4;audio.stop_all();std::cout<<count<<" files: application decoder PCM equivalence passed; looping effect play/stop and damage effect passed\n";return count==207?0:5;}catch(const std::exception&e){std::cerr<<e.what()<<"\n";return 6;}}
