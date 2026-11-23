#include "native_backend.hpp"
#include <algorithm>
#include <cstring>
#include <fstream>
#include <filesystem>
#include <stdexcept>
namespace shin::platform {
LRESULT CALLBACK NativeWindow::proc(HWND w,UINT m,WPARAM a,LPARAM b){
 auto self=reinterpret_cast<NativeWindow*>(GetWindowLongPtrW(w,GWLP_USERDATA));
 if(m==WM_NCCREATE){self=static_cast<NativeWindow*>(reinterpret_cast<CREATESTRUCTW*>(b)->lpCreateParams);SetWindowLongPtrW(w,GWLP_USERDATA,reinterpret_cast<LONG_PTR>(self));}
 if(self){if(m==WM_PAINT){PAINTSTRUCT p;HDC dc=BeginPaint(w,&p);self->paint(dc);EndPaint(w,&p);return 0;}
 if(m==WM_ERASEBKGND)return 1;
 if((m==WM_KEYDOWN||m==WM_SYSKEYDOWN)&&a==VK_F11){if(!(b&(LPARAM(1)<<30)))self->toggle_fullscreen();return 0;}
 if((m==WM_KEYUP||m==WM_SYSKEYUP)&&a==VK_F11)return 0;
 if(self->input_.message(m,static_cast<std::uint32_t>(a))){if(m==WM_DESTROY)PostQuitMessage(0);return 1;}}
 return DefWindowProcW(w,m,a,b);
}
NativeWindow::NativeWindow(const wchar_t* title,int width,int height):width_(width),height_(height){
 auto set_dpi=reinterpret_cast<BOOL(WINAPI*)(DPI_AWARENESS_CONTEXT)>(GetProcAddress(GetModuleHandleW(L"user32.dll"),"SetProcessDpiAwarenessContext"));if(set_dpi)set_dpi(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
 if(width<=0||height<=0||width>4096||height>4096)throw std::invalid_argument("invalid framebuffer size");rgb_.resize(std::size_t(width)*height);
 WNDCLASSW c{};c.lpfnWndProc=proc;c.hInstance=GetModuleHandleW(nullptr);c.lpszClassName=L"ShinRebuiltWindow";c.hCursor=LoadCursorW(nullptr,MAKEINTRESOURCEW(32512));
 if(!RegisterClassW(&c)&&GetLastError()!=ERROR_CLASS_ALREADY_EXISTS)throw std::runtime_error("RegisterClass failed");
 RECT r{0,0,width,height};AdjustWindowRect(&r,WS_OVERLAPPEDWINDOW,FALSE);
 window_=CreateWindowW(c.lpszClassName,title,WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,CW_USEDEFAULT,r.right-r.left,r.bottom-r.top,nullptr,nullptr,c.hInstance,this);
 if(!window_)throw std::runtime_error("CreateWindow failed");ShowWindow(window_,SW_SHOW);UpdateWindow(window_);
}
void NativeWindow::toggle_fullscreen(){
 if(!fullscreen_){MONITORINFO monitor{sizeof(MONITORINFO)};if(!GetMonitorInfoW(MonitorFromWindow(window_,MONITOR_DEFAULTTONEAREST),&monitor))return;
 windowed_style_=GetWindowLongPtrW(window_,GWL_STYLE);windowed_ex_style_=GetWindowLongPtrW(window_,GWL_EXSTYLE);windowed_placement_.length=sizeof(WINDOWPLACEMENT);if(!GetWindowPlacement(window_,&windowed_placement_))return;
 SetWindowLongPtrW(window_,GWL_STYLE,windowed_style_&~WS_OVERLAPPEDWINDOW);SetWindowLongPtrW(window_,GWL_EXSTYLE,windowed_ex_style_&~(WS_EX_WINDOWEDGE|WS_EX_CLIENTEDGE));auto&r=monitor.rcMonitor;SetWindowPos(window_,HWND_TOP,r.left,r.top,r.right-r.left,r.bottom-r.top,SWP_FRAMECHANGED|SWP_NOOWNERZORDER);fullscreen_=true;
 }else{SetWindowLongPtrW(window_,GWL_STYLE,windowed_style_);SetWindowLongPtrW(window_,GWL_EXSTYLE,windowed_ex_style_);SetWindowPlacement(window_,&windowed_placement_);SetWindowPos(window_,nullptr,0,0,0,0,SWP_FRAMECHANGED|SWP_NOMOVE|SWP_NOSIZE|SWP_NOZORDER|SWP_NOOWNERZORDER);fullscreen_=false;}
 InvalidateRect(window_,nullptr,FALSE);
}
void NativeWindow::release_backbuffer(){if(back_dc_&&back_previous_)SelectObject(back_dc_,back_previous_);if(back_bitmap_)DeleteObject(back_bitmap_);if(back_dc_)DeleteDC(back_dc_);back_dc_=nullptr;back_bitmap_=nullptr;back_previous_=nullptr;back_width_=back_height_=0;}
NativeWindow::~NativeWindow(){if(IsWindow(window_))DestroyWindow(window_);release_backbuffer();}
bool NativeWindow::pump(){MSG m;while(PeekMessageW(&m,nullptr,0,0,PM_REMOVE)){if(m.message==WM_QUIT)return false;TranslateMessage(&m);DispatchMessageW(&m);}return !input_.quit_requested;}
void NativeWindow::present(const std::uint8_t* p,const std::array<std::uint32_t,256>& palette){if(!p)throw std::invalid_argument("null pixels");for(std::size_t i=0;i<rgb_.size();++i)rgb_[i]=palette[p[i]];InvalidateRect(window_,nullptr,FALSE);UpdateWindow(window_);}
void NativeWindow::present(const std::uint8_t*p,const std::array<std::uint32_t,256>&palette,int w,int h,const std::uint32_t*border){if(w<=0||h<=0)return;width_=w;height_=h;rgb_.resize(std::size_t(w)*h);for(std::size_t i=0;i<rgb_.size();++i)rgb_[i]=border&&border[i]?border[i]&0xffffffu:palette[p[i]];InvalidateRect(window_,nullptr,FALSE);UpdateWindow(window_);}
void NativeWindow::paint(HDC dc){
 RECT r;GetClientRect(window_,&r);int w=r.right,h=r.bottom;if(w<=0||h<=0)return;
 // Compose the complete client area off-screen; expose it in one BitBlt.
 // Clearing the visible DC first allowed the compositor to show a black frame.
 if(w!=back_width_||h!=back_height_||!back_dc_){release_backbuffer();back_dc_=CreateCompatibleDC(dc);back_bitmap_=CreateCompatibleBitmap(dc,w,h);if(back_dc_&&back_bitmap_){back_previous_=SelectObject(back_dc_,back_bitmap_);back_width_=w;back_height_=h;}else{release_backbuffer();return;}}
 HDC back=back_dc_;

 FillRect(back,&r,static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH)));
 double scale=std::min(double(w)/width_,double(h)/height_);int dw=int(width_*scale),dh=int(height_*scale);
 BITMAPINFO bi{};bi.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);bi.bmiHeader.biWidth=width_;bi.bmiHeader.biHeight=-height_;bi.bmiHeader.biPlanes=1;bi.bmiHeader.biBitCount=32;bi.bmiHeader.biCompression=BI_RGB;
 SetStretchBltMode(back,COLORONCOLOR);int result=StretchDIBits(back,(w-dw)/2,(h-dh)/2,dw,dh,0,0,width_,height_,rgb_.data(),&bi,DIB_RGB_COLORS,SRCCOPY);
 if(result!=GDI_ERROR&&result!=0)BitBlt(dc,0,0,w,h,back,0,0,SRCCOPY);

}
void WavSound::stop(){if(device_){waveOutReset(device_);if(prepared_)waveOutUnprepareHeader(device_,&header_,sizeof(header_));waveOutClose(device_);}device_=nullptr;prepared_=false;header_={};}
bool WavSound::load(const std::wstring& path){stop();data_.clear();std::ifstream f(std::filesystem::path(path),std::ios::binary);std::vector<char>b{std::istreambuf_iterator<char>(f),{}};if(b.size()<12||std::memcmp(b.data(),"RIFF",4)||std::memcmp(b.data()+8,"WAVE",4))return false;bool fmt=false,dat=false;format_={};for(std::size_t p=12;p+8<=b.size();){std::uint32_t n;std::memcpy(&n,b.data()+p+4,4);p+=8;if(n>b.size()-p)return false;if(!std::memcmp(b.data()+p-8,"fmt ",4)){if(n<16)return false;std::memcpy(&format_,b.data()+p,std::min<std::size_t>(16,n));fmt=true;}if(!std::memcmp(b.data()+p-8,"data",4)){data_.assign(b.begin()+p,b.begin()+p+n);dat=true;}p+=n+(n&1);}return fmt&&dat&&format_.wFormatTag==WAVE_FORMAT_PCM&&format_.nBlockAlign&&data_.size()%format_.nBlockAlign==0;}
bool WavSound::play(bool loop){
 if(data_.empty()||format_.wFormatTag!=WAVE_FORMAT_PCM)return false;
 if(!device_){
  if(waveOutOpen(&device_,WAVE_MAPPER,&format_,0,0,CALLBACK_NULL)!=MMSYSERR_NOERROR){device_=nullptr;return false;}
  header_={};header_.lpData=data_.data();header_.dwBufferLength=static_cast<DWORD>(data_.size());
  if(waveOutPrepareHeader(device_,&header_,sizeof(header_))!=MMSYSERR_NOERROR){stop();return false;}prepared_=true;
 }else if(waveOutReset(device_)!=MMSYSERR_NOERROR)return false;
 header_.dwFlags&=~(WHDR_BEGINLOOP|WHDR_ENDLOOP);header_.dwLoops=0;
 if(loop){header_.dwFlags|=WHDR_BEGINLOOP|WHDR_ENDLOOP;header_.dwLoops=0xffffffff;}
 if(waveOutWrite(device_,&header_,sizeof(header_))!=MMSYSERR_NOERROR){stop();return false;}return true;
}
}

