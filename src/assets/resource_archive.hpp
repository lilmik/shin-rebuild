#pragma once
// Reconstructed from RESOURCE.RES structural observations, not original source.
#include <cstdint>
#include <fstream>
#include <iterator>
#include <memory>
#include <map>
#include <mutex>
#include <filesystem>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>
namespace shin {
struct ResourceEntry { std::string name; std::uint32_t offset, size; };
class ResourceArchive {
 struct Cached{std::weak_ptr<const std::vector<unsigned char>> data;std::uintmax_t size=0;std::filesystem::file_time_type time{};};
 static std::shared_ptr<const std::vector<unsigned char>> acquire(const std::string&path){
  static std::mutex mutex;static std::map<std::string,Cached> cache;std::lock_guard<std::mutex> lock(mutex);
  std::error_code ec;auto size=std::filesystem::file_size(path,ec);bool sized=!ec;ec.clear();auto time=std::filesystem::last_write_time(path,ec);bool timed=!ec;
  auto&entry=cache[path];if(auto data=entry.data.lock())if((!sized||entry.size==size)&&(!timed||entry.time==time))return data;
  std::ifstream f(path,std::ios::binary|std::ios::ate);if(!f)throw std::runtime_error("cannot open archive");auto end=f.tellg();
  if(end<0||std::uint64_t(end)>std::uint64_t(std::numeric_limits<std::streamsize>::max()))throw std::runtime_error("invalid archive size");
  auto data=std::make_shared<std::vector<unsigned char>>(std::size_t(end));f.seekg(0);if(!data->empty()&&!f.read(reinterpret_cast<char*>(data->data()),std::streamsize(data->size())))throw std::runtime_error("cannot read archive");
  entry={data,sized?size:std::uintmax_t(data->size()),time};return data;
 }
 std::shared_ptr<const std::vector<unsigned char>> backing_;
 const std::vector<unsigned char>&bytes_;
 std::vector<ResourceEntry> entries_;
 std::uint32_t u32(std::size_t p) const {
  if(p>bytes_.size() || bytes_.size()-p<4) throw std::runtime_error("truncated integer");
  return std::uint32_t(bytes_[p]) | (std::uint32_t(bytes_[p+1])<<8) | (std::uint32_t(bytes_[p+2])<<16) | (std::uint32_t(bytes_[p+3])<<24);
 }
public:
 explicit ResourceArchive(const std::string& path):backing_(acquire(path)),bytes_(*backing_) {
  auto count=u32(0);if(count>(bytes_.size()-4)/28)throw std::runtime_error("invalid entry count");
  for(std::uint32_t i=0;i<count;++i){std::size_t p=4+std::size_t(i)*28;
   std::size_t n=0;while(n<20 && bytes_[p+n])++n;
   ResourceEntry e{std::string(reinterpret_cast<const char*>(bytes_.data()+p),n),u32(p+20),u32(p+24)};
   if(e.offset>bytes_.size() || e.size>bytes_.size()-e.offset)throw std::runtime_error("invalid resource range");
   entries_.push_back(e);
  }
 }
 const void* storage_identity()const{return backing_.get();}
 const std::vector<ResourceEntry>& entries() const {return entries_;}
 std::vector<unsigned char> read(std::size_t index) const {
  const auto&e=entries_.at(index);return {bytes_.begin()+e.offset,bytes_.begin()+e.offset+e.size};
 }
};
}

namespace shin {
struct AppImage { std::uint32_t width,height;std::vector<unsigned char> indices; };
// Matches REA FUN_004402c0, adding bounds checks absent in the original.
inline AppImage decode_app(const std::vector<unsigned char>& data) {
 if(data.size()<12)throw std::runtime_error("truncated APP header");
 auto u32=[&](std::size_t p){return std::uint32_t(data[p])|(std::uint32_t(data[p+1])<<8)|(std::uint32_t(data[p+2])<<16)|(std::uint32_t(data[p+3])<<24);};
 auto w=u32(0),h=u32(4),n=u32(8);
 if(n!=data.size()-12 || n%2)throw std::runtime_error("invalid APP stream length");
 std::uint64_t expected=std::uint64_t(w)*h;
 if(expected>268435456)throw std::runtime_error("APP exceeds pixel budget");
 AppImage image{w,h,{}};image.indices.reserve(static_cast<std::size_t>(expected));
 for(std::size_t p=12;p<data.size();p+=2){
  if(data[p+1]>expected-image.indices.size())throw std::runtime_error("APP run exceeds dimensions");
  image.indices.insert(image.indices.end(),data[p+1],data[p]);
 }
 if(image.indices.size()!=expected)throw std::runtime_error("APP pixels do not fill dimensions");
 return image;
}
}

namespace shin {
struct MapCell {std::int16_t background_tile,foreground_tile,raw2,raw3,raw4;};
struct MapData {std::uint32_t width,height;std::vector<MapCell> cells;};
inline MapData decode_map(const std::vector<unsigned char>& data){
 if(data.size()<8)throw std::runtime_error("truncated MAP header");
 auto u32=[&](std::size_t p){return std::uint32_t(data[p])|(std::uint32_t(data[p+1])<<8)|(std::uint32_t(data[p+2])<<16)|(std::uint32_t(data[p+3])<<24);};
 auto i16=[&](std::size_t p){std::uint16_t v=std::uint16_t(data[p])|(std::uint16_t(data[p+1])<<8);return static_cast<std::int16_t>(v>=32768?static_cast<int>(v)-65536:static_cast<int>(v));};
 auto w=u32(0),h=u32(4);std::uint64_t count=std::uint64_t(w)*h;
 if(count!=(data.size()-8)/10 || (data.size()-8)%10)throw std::runtime_error("MAP dimensions mismatch");
 MapData map{w,h,{}};map.cells.reserve(static_cast<std::size_t>(count));
 for(std::size_t p=8;p<data.size();p+=10)map.cells.push_back({i16(p),i16(p+2),i16(p+4),i16(p+6),i16(p+8)});
 return map;
}
struct RgbColor {unsigned char red,green,blue;};
inline std::vector<RgbColor> decode_palette(const std::vector<unsigned char>& data){
 if(data.size()!=768)throw std::runtime_error("PAL requires 768 bytes");
 std::vector<RgbColor> colors;colors.reserve(256);
 for(std::size_t p=0;p<768;p+=3){if(data[p]>63 || data[p+1]>63 || data[p+2]>63)throw std::runtime_error("PAL channel exceeds 6 bits");colors.push_back({static_cast<unsigned char>(data[p]<<2),static_cast<unsigned char>(data[p+1]<<2),static_cast<unsigned char>(data[p+2]<<2)});}
 return colors;
}
}
