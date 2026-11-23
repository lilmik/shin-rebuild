#pragma once
#include <array>
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <stdexcept>
namespace shin {
struct ProgressSave {
 int stages=1;std::array<bool,5> bonus{};
 static unsigned checksum(const std::array<unsigned char,16>&b){unsigned h=2166136261u;for(int i=0;i<12;++i)h=(h^b[i])*16777619u;return h;}
 bool read(const std::filesystem::path&p){std::ifstream f(p,std::ios::binary);std::array<unsigned char,16>b{};if(!f.read(reinterpret_cast<char*>(b.data()),16)||f.peek()!=EOF)return false;const char*m="SHINSAV1";for(int i=0;i<8;++i)if(b[i]!=m[i])return false;unsigned hash=0;for(int i=0;i<4;++i)hash|=unsigned(b[12+i])<<(i*8);if(hash!=checksum(b)||b[8]<1||b[8]>7||b[9]>31)return false;stages=b[8];for(int i=0;i<5;++i)bonus[i]=(b[9]>>i)&1;return true;}
 void load(const std::filesystem::path&p){*this=ProgressSave{};if(!read(p)&&!read(p.string()+".tmp"))read(p.string()+".bak");}
 void save(const std::filesystem::path&p)const{std::filesystem::create_directories(p.parent_path());std::array<unsigned char,16>b{};const char*m="SHINSAV1";for(int i=0;i<8;++i)b[i]=m[i];b[8]=static_cast<unsigned char>(std::clamp(stages,1,7));for(int i=0;i<5;++i)if(bonus[i])b[9]|=1<<i;unsigned h=checksum(b);for(int i=0;i<4;++i)b[12+i]=h>>(i*8);auto tmp=p.string()+".tmp";{std::ofstream f(tmp,std::ios::binary|std::ios::trunc);f.write(reinterpret_cast<const char*>(b.data()),b.size());f.close();if(!f)throw std::runtime_error("progress save write failed");}if(std::filesystem::exists(p)){ProgressSave valid;if(valid.read(p)){
 // libnx filesystem copy_file may reject an existing destination even with
 // overwrite_existing. Stream truncation is supported on both targets.
 std::ifstream source(p,std::ios::binary);std::ofstream backup(p.string()+".bak",std::ios::binary|std::ios::trunc);
 if(!source||!backup)throw std::runtime_error("progress backup open failed");
 backup<<source.rdbuf();backup.close();
 if(source.bad()||!backup)throw std::runtime_error("progress backup write failed");
 }std::filesystem::remove(p);}std::filesystem::rename(tmp,p);}
 bool try_save(const std::filesystem::path&p,std::string&error)const noexcept{try{save(p);error.clear();return true;}catch(const std::exception&e){error=e.what();return false;}catch(...){error="Unknown save error";return false;}}
 bool complete(int stage){int n=std::clamp(stage+2,1,7);if(n<=stages)return false;stages=n;return true;}
 bool unlock(int index){if(index<0||index>=5||bonus[index])return false;bonus[index]=true;return true;}
};
}
