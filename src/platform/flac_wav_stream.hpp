#pragma once
#include <SDL.h>
#include "../third_party/dr_flac.h"
#include <array>
#include <memory>
#include <vector>
#include <cstring>
#include <algorithm>
namespace shin {
// Expose decoded FLAC as a seekable PCM WAV without decoding entire BGM into RAM.
inline SDL_RWops* flac_wav_stream(const char*path){
 struct Stream{drflac*d;std::array<Uint8,44>header{};Sint64 pos=0,total=0;drflac_uint64 cursor=0;~Stream(){drflac_close(d);}};
 auto*d=drflac_open_file(path,nullptr);if(!d)return nullptr;
 if(d->bitsPerSample>16||d->channels>8||d->totalPCMFrameCount==0){drflac_close(d);return nullptr;}
 auto*s=new Stream{d};Uint32 bytes=Uint32(d->totalPCMFrameCount*d->channels*2);s->total=44+Sint64(bytes);
 auto put16=[&](int o,Uint16 v){s->header[o]=v;s->header[o+1]=v>>8;};auto put32=[&](int o,Uint32 v){for(int j=0;j<4;++j)s->header[o+j]=v>>(j*8);};
 memcpy(s->header.data(),"RIFF",4);put32(4,36+bytes);memcpy(s->header.data()+8,"WAVEfmt ",8);put32(16,16);put16(20,1);put16(22,d->channels);put32(24,d->sampleRate);put32(28,d->sampleRate*d->channels*2);put16(32,d->channels*2);put16(34,16);memcpy(s->header.data()+36,"data",4);put32(40,bytes);
 auto*rw=SDL_AllocRW();if(!rw){delete s;return nullptr;}rw->hidden.unknown.data1=s;rw->type=SDL_RWOPS_UNKNOWN;
 rw->size=[](SDL_RWops*r)->Sint64{return static_cast<Stream*>(r->hidden.unknown.data1)->total;};
 rw->seek=[](SDL_RWops*r,Sint64 offset,int whence)->Sint64{auto*s=static_cast<Stream*>(r->hidden.unknown.data1);Sint64 base=whence==RW_SEEK_SET?0:whence==RW_SEEK_CUR?s->pos:whence==RW_SEEK_END?s->total:-1;if(base<0||offset< -base||offset>s->total-base)return -1;return s->pos=base+offset;};
 rw->read=[](SDL_RWops*r,void*ptr,size_t size,size_t count)->size_t{if(!size)return 0;auto*s=static_cast<Stream*>(r->hidden.unknown.data1);size_t want=std::min<size_t>((s->total-s->pos)/size,count)*size,done=0;auto*out=static_cast<Uint8*>(ptr);
 if(s->pos<44){size_t n=std::min<size_t>(want,44-s->pos);memcpy(out,s->header.data()+s->pos,n);s->pos+=n;done+=n;}
 if(done<want){auto align=s->d->channels*2;auto start=drflac_uint64(s->pos-44)/align;size_t skip=size_t(s->pos-44)%align,frames=(want-done+skip+align-1)/align;
 if(s->cursor!=start&&!drflac_seek_to_pcm_frame(s->d,start))return done/size;
 std::vector<drflac_int16>pcm(frames*s->d->channels);auto got=drflac_read_pcm_frames_s16(s->d,frames,pcm.data());s->cursor=start+got;size_t available=size_t(got)*align;size_t n=available>skip?std::min(want-done,available-skip):0;memcpy(out+done,reinterpret_cast<Uint8*>(pcm.data())+skip,n);s->pos+=n;done+=n;}
 return done/size;};
 rw->write=[](SDL_RWops*,const void*,size_t,size_t)->size_t{return 0;};rw->close=[](SDL_RWops*r)->int{delete static_cast<Stream*>(r->hidden.unknown.data1);SDL_FreeRW(r);return 0;};return rw;
}
// Decode the complete WAV view before handing it to SDL_mixer. The returned
// RWops owns the PCM bytes and releases them when Mix_FreeMusic closes it.
inline SDL_RWops* flac_wav_memory(const char*path){
 struct Memory{std::vector<Uint8>bytes;Sint64 pos=0;};
 auto*source=flac_wav_stream(path);if(!source)return nullptr;
 std::unique_ptr<SDL_RWops,int(*)(SDL_RWops*)>input(source,SDL_RWclose);
 auto size=SDL_RWsize(source);if(size<44||Uint64(size)>std::vector<Uint8>().max_size())return nullptr;
 auto data=std::make_unique<Memory>();data->bytes.resize(size_t(size));
 if(SDL_RWread(source,data->bytes.data(),1,data->bytes.size())!=data->bytes.size())return nullptr;
 // Close the file and FLAC decoder before playback starts.
 input.reset();
 auto*rw=SDL_AllocRW();if(!rw)return nullptr;rw->hidden.unknown.data1=data.release();rw->type=SDL_RWOPS_UNKNOWN;
 rw->size=[](SDL_RWops*r)->Sint64{return Sint64(static_cast<Memory*>(r->hidden.unknown.data1)->bytes.size());};
 rw->seek=[](SDL_RWops*r,Sint64 offset,int whence)->Sint64{auto*s=static_cast<Memory*>(r->hidden.unknown.data1);auto total=Sint64(s->bytes.size());Sint64 base=whence==RW_SEEK_SET?0:whence==RW_SEEK_CUR?s->pos:whence==RW_SEEK_END?total:-1;if(base<0||offset< -base||offset>total-base)return -1;return s->pos=base+offset;};
 rw->read=[](SDL_RWops*r,void*ptr,size_t size,size_t count)->size_t{if(!size)return 0;auto*s=static_cast<Memory*>(r->hidden.unknown.data1);size_t objects=std::min(count,(s->bytes.size()-size_t(s->pos))/size),bytes=objects*size;memcpy(ptr,s->bytes.data()+s->pos,bytes);s->pos+=bytes;return objects;};
 rw->write=[](SDL_RWops*,const void*,size_t,size_t)->size_t{return 0;};
 rw->close=[](SDL_RWops*r)->int{delete static_cast<Memory*>(r->hidden.unknown.data1);SDL_FreeRW(r);return 0;};return rw;
}
}
