#pragma once
#include <filesystem>
#include <fstream>
#include <sstream>
namespace shin {
inline int extra_life_score_interval=100;
inline bool score_awards_life(int score){return score>0&&score%extra_life_score_interval==0;}
inline void load_game_rules(const std::filesystem::path&path){
 extra_life_score_interval=100;std::ifstream in(path);
 if(!in){std::filesystem::create_directories(path.parent_path());std::ofstream out(path);out<<"# Score points per extra life (restart after editing)\nrules_version=0.3.0\nextra_life_score_interval=100\n";return;}
 bool current=false;std::string line;
 while(std::getline(in,line)){auto comment=line.find('#');if(comment!=std::string::npos)line.resize(comment);auto eq=line.find('=');if(eq==std::string::npos)continue;
  std::istringstream k(line.substr(0,eq)),v(line.substr(eq+1));std::string key,extra,value;int n;if(!(k>>key))continue;
  if(key=="rules_version"){if(v>>value&&value=="0.3.0")current=true;}
  else if(key=="extra_life_score_interval"&&v>>n&&!(v>>extra)&&n>=1&&n<=100000)extra_life_score_interval=n;
 }
 in.close();
 // Upgrade the old test default once; later user edits remain configurable.
 if(!current){std::ofstream out(path,std::ios::app);out<<"\n# 0.3.0 default migration\nrules_version=0.3.0\n";if(extra_life_score_interval==5){extra_life_score_interval=100;out<<"extra_life_score_interval=100\n";}}
}
}
