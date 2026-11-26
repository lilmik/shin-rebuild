#pragma once
#include <SDL.h>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdexcept>
namespace shin {
// Extract packaged data once to app-private storage for the existing seekable
// archive/FLAC readers. Temporary writes + manifest marker make updates restartable.
inline void install_mobile_assets(const std::filesystem::path&root){
 auto read=[](const char*name){auto*r=SDL_RWFromFile(name,"rb");if(!r)throw std::runtime_error(std::string("Missing packaged data: ")+name);auto n=SDL_RWsize(r);if(n<0){SDL_RWclose(r);throw std::runtime_error("Invalid packaged data size");}std::string s(size_t(n),'\0');size_t got=SDL_RWread(r,s.data(),1,s.size());SDL_RWclose(r);if(got!=s.size())throw std::runtime_error("Packaged data read failed");return s;};
 auto manifest=read("assets/manifest.txt");auto marker=root/"installed-manifest.txt";
 {std::ifstream f(marker,std::ios::binary);std::string old((std::istreambuf_iterator<char>(f)),{});if(old==manifest&&std::filesystem::exists(root/"RESOURCE.RES"))return;}
 std::istringstream list(manifest);std::string line;
 while(std::getline(list,line)){std::istringstream fields(line);std::string hash,name;unsigned long long size=0;if(!(fields>>hash>>size))continue;fields.get();std::getline(fields,name);std::filesystem::path rel(name);if(rel.is_absolute()||name.find("..")!=std::string::npos)throw std::runtime_error("Invalid asset path");auto target=root/rel;std::filesystem::create_directories(target.parent_path());auto temp=target;temp+=".tmp";auto*src=SDL_RWFromFile(("assets/"+name).c_str(),"rb");if(!src)throw std::runtime_error("Missing asset: "+name);std::ofstream out(temp,std::ios::binary);std::vector<char>buf(262144);unsigned long long copied=0;while(auto got=SDL_RWread(src,buf.data(),1,buf.size())){out.write(buf.data(),got);copied+=got;}SDL_RWclose(src);out.close();if(copied!=size||!out)throw std::runtime_error("Asset install failed: "+name);std::filesystem::rename(temp,target);}
 auto temp=marker;temp+=".tmp";std::ofstream f(temp,std::ios::binary);f<<manifest;f.close();if(!f)throw std::runtime_error("Asset manifest write failed");std::filesystem::rename(temp,marker);
}
}
