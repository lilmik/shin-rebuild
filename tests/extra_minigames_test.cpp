#include "../src/game/extra_minigames.hpp"
#include "../src/graphics/minigame_canvas.hpp"
#include <cassert>
#include <queue>
#include <unordered_map>
int main(int argc,char**argv){assert(argc==2);shin::AssetStore a(argv[1]);shin::IndexedSurface f(853,480);f.origin_x=106;shin::ExtraMinigames g;unsigned now=1;assert(g.start(1));g.tick(now,{},a,f);now=2400;
 auto tick=[&](shin::ExtraMinigames::Input i){now+=16;return g.tick(now,i,a,f);};auto go=[&](int n){while(g.selected()<n){tick({false,false,false,true});tick({});}while(g.selected()>n){tick({false,false,true});tick({});}};
 for(int face=0;face<9;++face){int first=-1,second=-1;for(int i=0;i<18;++i)if(g.face(i)==face){if(first<0)first=i;else second=i;}go(first);tick({false,false,false,false,true});tick({});go(second);auto out=tick({false,false,false,false,true});assert(!out.sounds.empty());tick({});now+=801;g.tick(now,{},a,f);}assert(!g.active()&&g.lives()==4);
 assert(g.start(4));now=1;for(int i=0;i<320;++i){now+=16;g.tick(now,{},a,f);}using Board=std::array<int,9>;struct Node{Board b;int parent,dir;};std::vector<Node> nodes{{g.board(),-1,-1}};std::unordered_map<unsigned long long,int> seen;auto key=[](Board b){unsigned long long v=0;for(int n:b)v=v*9+unsigned(n<0?8:n);return v;};seen[key(g.board())]=0;int end=-1;
 for(std::size_t i=0;i<nodes.size()&&end<0;++i){auto b=nodes[i].b;bool solved=true;for(int j=0;j<9;++j)solved&=b[j]==(j==8?-1:j);if(solved){end=int(i);break;}int blank=int(std::find(b.begin(),b.end(),-1)-b.begin());for(int d=0;d<4;++d){int n=blank;if(d==0&&blank>=3)n-=3;if(d==1&&blank%3<2)++n;if(d==2&&blank<6)n+=3;if(d==3&&blank%3>0)--n;if(n==blank)continue;auto next=b;std::swap(next[n],next[blank]);auto k=key(next);if(seen.emplace(k,int(nodes.size())).second)nodes.push_back({next,int(i),d});}}
 assert(end>=0);std::vector<int> path;while(nodes[end].parent>=0){path.push_back(nodes[end].dir);end=nodes[end].parent;}std::reverse(path.begin(),path.end());for(int d:path){tick({d==2,d==0,d==1,d==3});tick({});}now+=2501;assert(g.tick(now,{},a,f).finished);
 for(int typ:{1,4}){assert(g.start(typ));assert(tick({false,false,false,false,false,true}).finished);}
 {shin::MinigameCanvas canvas(a,f);assert(f.clip_core&&!f.border_rgb.empty());auto original=f.border_rgb;shin::IndexedSurface hammer(200,200,5);f.blit(hammer,-200,-200);assert(original==f.border_rgb);}assert(!f.clip_core);
 auto raw=shin::decode_app(a.bytes("savebg.app"));auto&selection=a.image("savebg.app");auto pal=a.palette("shin.pal");for(int yy=257;yy<=422;++yy)for(int xx=40;xx<=171;++xx){auto c=pal[raw.indices[yy*640+xx]];int r=(c>>16)&255,g=(c>>8)&255,b=c&255;if(std::min({r,g,b})<144||std::max({r,g,b})-std::min({r,g,b})>80)assert(selection.pixels[yy*640+xx]==raw.indices[yy*640+xx]);}
 for(auto point:{std::pair<int,int>{200,80},{180,430},{400,400}}){auto i=point.second*640+point.first;assert(selection.pixels[i]==selection.wallpaper[i]);}
 shin::ResourceArchive x(argv[1]),y(argv[1]);assert(x.storage_identity()==y.storage_identity());
}
