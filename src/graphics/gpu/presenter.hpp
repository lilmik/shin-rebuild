#pragma once
#include "shader_source.hpp"
#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <cstddef>
#include <cstring>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>
#ifdef _WIN32
#define SHIN_GL_CALL __stdcall
#else
#define SHIN_GL_CALL
#endif
namespace shin {
// GPU-only scaling. The CPU uploads the original framebuffer; no CPU upscaled image.
class GpuPresenter {
#define GPU_FUNCTIONS(F) \
 F(unsigned,CreateShader,(unsigned)) F(void,ShaderSource,(unsigned,int,const char*const*,const int*)) F(void,CompileShader,(unsigned)) F(void,GetShaderiv,(unsigned,unsigned,int*)) F(void,GetShaderInfoLog,(unsigned,int,int*,char*)) F(unsigned,CreateProgram,()) F(void,AttachShader,(unsigned,unsigned)) F(void,BindAttribLocation,(unsigned,unsigned,const char*)) F(void,LinkProgram,(unsigned)) F(void,GetProgramiv,(unsigned,unsigned,int*)) F(void,GetProgramInfoLog,(unsigned,int,int*,char*)) F(void,DeleteShader,(unsigned)) F(void,DeleteProgram,(unsigned)) F(void,UseProgram,(unsigned)) F(int,GetUniformLocation,(unsigned,const char*)) F(void,Uniform1i,(int,int)) F(void,Uniform2f,(int,float,float)) F(void,GenBuffers,(int,unsigned*)) F(void,BindBuffer,(unsigned,unsigned)) F(void,BufferData,(unsigned,std::ptrdiff_t,const void*,unsigned)) F(void,DeleteBuffers,(int,const unsigned*)) F(void,GenVertexArrays,(int,unsigned*)) F(void,BindVertexArray,(unsigned)) F(void,DeleteVertexArrays,(int,const unsigned*)) F(void,EnableVertexAttribArray,(unsigned)) F(void,VertexAttribPointer,(unsigned,int,unsigned,unsigned char,int,const void*)) F(void,GenTextures,(int,unsigned*)) F(void,DeleteTextures,(int,const unsigned*)) F(void,BindTexture,(unsigned,unsigned)) F(void,TexParameteri,(unsigned,unsigned,int)) F(void,TexImage2D,(unsigned,int,int,int,int,int,unsigned,unsigned,const void*)) F(void,TexSubImage2D,(unsigned,int,int,int,int,int,unsigned,unsigned,const void*)) F(void,PixelStorei,(unsigned,int)) F(void,Viewport,(int,int,int,int)) F(void,ClearColor,(float,float,float,float)) F(void,Clear,(unsigned)) F(void,DrawArrays,(unsigned,int,int)) F(void,ReadPixels,(int,int,int,int,unsigned,unsigned,void*)) F(void,ActiveTexture,(unsigned)) F(const unsigned char*,GetString,(unsigned))
#define DECL(ret,name,args) using name##Fn=ret(SHIN_GL_CALL*)args;name##Fn name=nullptr;
 GPU_FUNCTIONS(DECL)
#undef DECL
 unsigned programs_[2]{},texture_=0,label_texture_=0,buffer_=0,vao_=0;int lw_=0,lh_=0;int tw_=0,th_=0;std::string renderer_;
 unsigned compile(unsigned type,const char*source){std::string adapted(source);
#ifdef __APPLE__
 if(adapted.rfind("#version 130",0)==0)adapted.replace(0,12,"#version 150");
#endif
 source=adapted.c_str();unsigned shader=CreateShader(type);ShaderSource(shader,1,&source,nullptr);CompileShader(shader);int ok=0;GetShaderiv(shader,0x8b81,&ok);if(!ok){char log[4096]{};GetShaderInfoLog(shader,sizeof(log),nullptr,log);DeleteShader(shader);throw std::runtime_error(std::string("GPU shader: ")+log);}return shader;}
 unsigned link(const char*fragment){const char*vertex="#version 130\nin vec2 Position;in vec2 UV;out vec4 TEX0;void main(){gl_Position=vec4(Position,0,1);TEX0=vec4(UV,0,1);}";unsigned vs=compile(0x8b31,vertex),fs=compile(0x8b30,fragment),program=CreateProgram();AttachShader(program,vs);AttachShader(program,fs);BindAttribLocation(program,0,"Position");BindAttribLocation(program,1,"UV");LinkProgram(program);DeleteShader(vs);DeleteShader(fs);int ok=0;GetProgramiv(program,0x8b82,&ok);if(!ok){char log[4096]{};GetProgramInfoLog(program,sizeof(log),nullptr,log);DeleteProgram(program);throw std::runtime_error(std::string("GPU link: ")+log);}return program;}
 void image(const uint32_t*p,int iw,int ih,int x,int y,int ow,int oh,int mode){
  ActiveTexture(0x84c0);BindTexture(0xde1,texture_);PixelStorei(0xcf5,4);TexParameteri(0xde1,0x2801,mode==1?0x2601:0x2600);TexParameteri(0xde1,0x2800,mode==1?0x2601:0x2600);TexParameteri(0xde1,0x2802,0x812f);TexParameteri(0xde1,0x2803,0x812f);
  if(tw_!=iw||th_!=ih){TexImage2D(0xde1,0,0x8058,iw,ih,0,0x80e1,0x1401,p);tw_=iw;th_=ih;}else TexSubImage2D(0xde1,0,0,0,iw,ih,0x80e1,0x1401,p);
  unsigned program=programs_[mode==2?1:0];UseProgram(program);Uniform1i(GetUniformLocation(program,"Texture"),0);Uniform2f(GetUniformLocation(program,"TextureSize"),float(iw),float(ih));Uniform2f(GetUniformLocation(program,"OutputSize"),float(ow),float(oh));Viewport(x,y,ow,oh);BindVertexArray(vao_);DrawArrays(5,0,4);
 }
public:
 template<class Resolver>explicit GpuPresenter(Resolver resolve){
#define LOAD(ret,name,args) name=reinterpret_cast<name##Fn>(resolve("gl" #name));if(!name)throw std::runtime_error("GPU missing gl" #name);
 GPU_FUNCTIONS(LOAD)
#undef LOAD
  auto*name=GetString(0x1f01);renderer_=name?reinterpret_cast<const char*>(name):"Unknown";
  if(!std::getenv("SHIN_ALLOW_SOFTWARE_GPU")&&(renderer_.find("llvmpipe")!=std::string::npos||renderer_.find("Software Rasterizer")!=std::string::npos||renderer_.find("GDI Generic")!=std::string::npos))throw std::runtime_error("Hardware GPU unavailable: "+renderer_);
  programs_[0]=link("#version 130\nuniform sampler2D Texture;in vec4 TEX0;out vec4 FragColor;void main(){FragColor=vec4(texture(Texture,TEX0.xy).rgb,1.0);}");programs_[1]=link(xbrz_fragment);
  const float vertices[]={-1,1,0,0,-1,-1,0,1,1,1,1,0,1,-1,1,1};GenVertexArrays(1,&vao_);BindVertexArray(vao_);GenBuffers(1,&buffer_);BindBuffer(0x8892,buffer_);BufferData(0x8892,sizeof(vertices),vertices,0x88e4);EnableVertexAttribArray(0);VertexAttribPointer(0,2,0x1406,0,4*sizeof(float),nullptr);EnableVertexAttribArray(1);VertexAttribPointer(1,2,0x1406,0,4*sizeof(float),reinterpret_cast<void*>(2*sizeof(float)));GenTextures(1,&texture_);
 }
 ~GpuPresenter(){if(label_texture_)DeleteTextures(1,&label_texture_);if(texture_)DeleteTextures(1,&texture_);if(buffer_)DeleteBuffers(1,&buffer_);if(vao_)DeleteVertexArrays(1,&vao_);for(auto p:programs_)if(p)DeleteProgram(p);}
 void capture(const char*path,int w,int h){std::vector<unsigned char>data(size_t(w)*h*3);PixelStorei(0xd05,1);ReadPixels(0,0,w,h,0x1907,0x1401,data.data());std::ofstream out(path,std::ios::binary);out<<"P6\n"<<w<<" "<<h<<"\n255\n";for(int y=h-1;y>=0;--y)out.write(reinterpret_cast<const char*>(data.data()+size_t(y)*w*3),w*3);}
 const std::string& renderer()const{return renderer_;}
 void draw(const uint32_t*p,int iw,int ih,int width,int height,int mode,const char*notice=nullptr){if(iw<=0||ih<=0||width<=0||height<=0)return;ClearColor(0,0,0,1);Clear(0x4000);double scale=std::min(double(width)/iw,double(height)/ih);int ow=int(iw*scale),oh=int(ih*scale);image(p,iw,ih,(width-ow)/2,(height-oh)/2,ow,oh,(iw==ow&&ih==oh)?0:mode);
  if(notice){const char*keys="NEARSTBILXZ123/GPU";const unsigned char glyph[][5]={{127,4,8,16,127},{127,73,73,73,65},{126,17,17,17,126},{127,9,25,41,70},{70,73,73,73,49},{1,1,127,1,1},{127,73,73,73,54},{0,65,127,65,0},{127,64,64,64,64},{99,20,8,20,99},{97,81,73,69,67},{0,66,127,64,0},{66,97,81,73,70},{33,65,69,75,49},{32,16,8,4,2},{62,65,73,73,122},{127,9,9,9,6},{63,64,64,64,63}};int w=int(std::strlen(notice))*12+12,h=26;std::vector<uint32_t>text(size_t(w)*h,0);for(int i=0;notice[i];++i)if(auto*found=std::strchr(keys,notice[i]))for(int x=0;x<5;++x)for(int y=0;y<7;++y)if(glyph[found-keys][x]&(1<<y))for(int a=0;a<2;++a)for(int b=0;b<2;++b)text[size_t(6+y*2+b)*w+6+i*12+x*2+a]=0xffffff;unsigned original=texture_;int owcache=tw_,ohcache=th_;if(!label_texture_)GenTextures(1,&label_texture_);texture_=label_texture_;tw_=lw_;th_=lh_;image(text.data(),w,h,8,height-8-h,w,h,0);lw_=tw_;lh_=th_;texture_=original;tw_=owcache;th_=ohcache;}
 }
};
#undef GPU_FUNCTIONS
}
#undef SHIN_GL_CALL
