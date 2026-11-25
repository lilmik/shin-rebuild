#pragma once
#include <filesystem>
#include <fstream>
#include <vector>
#include <cstdint>
#include <cstring>
#include <stdexcept>
namespace shin {
inline std::vector<unsigned char> read_rgba_overlay(const std::filesystem::path&path,int w,int h){
 std::ifstream file(path,std::ios::binary);if(!file)return {};
 unsigned char header[16]{};if(!file.read(reinterpret_cast<char*>(header),16)||std::memcmp(header,"SHRGBA1",8))throw std::runtime_error("Invalid RGBA overlay header");
 auto u32=[&](int at){return std::uint32_t(header[at])|(std::uint32_t(header[at+1])<<8)|(std::uint32_t(header[at+2])<<16)|(std::uint32_t(header[at+3])<<24);};
 if(u32(8)!=unsigned(w)||u32(12)!=unsigned(h))throw std::runtime_error("RGBA overlay dimensions do not match background");
 std::vector<unsigned char>pixels(size_t(w)*h*4);if(!file.read(reinterpret_cast<char*>(pixels.data()),std::streamsize(pixels.size()))||file.peek()!=EOF)throw std::runtime_error("Invalid RGBA overlay pixel data");
 return pixels;
}
}
