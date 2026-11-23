// Developer tool: reads user-supplied resources; never bundles or modifies the archive.
#include "assets/resource_archive.hpp"
#include <iostream>
int main(int argc,char**argv){try{
 if(argc!=2){std::cerr<<"usage: verify_resources RESOURCE.RES\n";return 2;}
 shin::ResourceArchive a(argv[1]);std::size_t apps=0,maps=0,pals=0;std::uint64_t pixels=0,cells=0;
 for(std::size_t i=0;i<a.entries().size();++i){const auto&n=a.entries()[i].name;if(n.size()<4)continue;auto ext=n.substr(n.size()-4);auto bytes=a.read(i);
 if(ext==".app"){auto image=shin::decode_app(bytes);++apps;pixels+=image.indices.size();}
 else if(ext==".map"){auto map=shin::decode_map(bytes);++maps;cells+=map.cells.size();}
 else if(ext==".pal"){auto colors=shin::decode_palette(bytes);if(colors.size()!=256)throw std::runtime_error("invalid palette count");++pals;}}
 std::cout<<"entries="<<a.entries().size()<<" apps="<<apps<<" pixels="<<pixels<<" maps="<<maps<<" cells="<<cells<<" palettes="<<pals<<" all valid\n";return 0;
 }catch(const std::exception&e){std::cerr<<e.what()<<'\n';return 1;}}
