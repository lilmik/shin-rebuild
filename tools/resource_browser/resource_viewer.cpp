// Developer tool: reads user-supplied resources; never bundles or modifies the archive.
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shellapi.h>
#include "assets/resource_archive.hpp"
#include <memory>
#include <algorithm>
#include <sstream>
namespace {
std::unique_ptr<shin::ResourceArchive> archive;
std::vector<std::size_t> apps,pals;
std::size_t current=0,palette=0;
shin::AppImage image;
std::vector<unsigned char> dib;
BITMAPINFO info{};
std::string error;
bool suffix(const std::string&s,const char*ext){return s.size()>=4 && s.substr(s.size()-4)==ext;}
void load(HWND window){
 try{
  image=shin::decode_app(archive->read(apps.at(current)));
  auto pal=archive->read(pals.at(palette));if(pal.size()!=768)throw std::runtime_error("invalid palette");
  dib.assign(std::size_t(image.width)*image.height*4,0);
  for(std::size_t i=0;i<image.indices.size();++i){auto idx=std::size_t(image.indices[i])*3;dib[i*4]=static_cast<unsigned char>(pal[idx+2]<<2);dib[i*4+1]=static_cast<unsigned char>(pal[idx+1]<<2);dib[i*4+2]=static_cast<unsigned char>(pal[idx]<<2);}
  info={};info.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);info.bmiHeader.biWidth=static_cast<LONG>(image.width);info.bmiHeader.biHeight=-static_cast<LONG>(image.height);info.bmiHeader.biPlanes=1;info.bmiHeader.biBitCount=32;info.bmiHeader.biCompression=BI_RGB;
  std::ostringstream title;title<<"SHIN resource preview - "<<(current+1)<<"/"<<apps.size()<<" "<<archive->entries()[apps[current]].name<<" "<<image.width<<"x"<<image.height<<" | Palette: "<<archive->entries()[pals[palette]].name<<" | Left/Right image, Up/Down palette";
  SetWindowTextA(window,title.str().c_str());error.clear();
 }catch(const std::exception&e){error=e.what();}
 InvalidateRect(window,nullptr,TRUE);
}
LRESULT CALLBACK proc(HWND w,UINT m,WPARAM wp,LPARAM lp){
 switch(m){
 case WM_KEYDOWN:
  if(wp==VK_RIGHT)current=(current+1)%apps.size();else if(wp==VK_LEFT)current=(current+apps.size()-1)%apps.size();else if(wp==VK_UP)palette=(palette+1)%pals.size();else if(wp==VK_DOWN)palette=(palette+pals.size()-1)%pals.size();else return 0;load(w);return 0;
 case WM_SIZE:InvalidateRect(w,nullptr,TRUE);return 0;
 case WM_PAINT:{PAINTSTRUCT ps;HDC dc=BeginPaint(w,&ps);RECT r;GetClientRect(w,&r);FillRect(dc,&r,static_cast<HBRUSH>(GetStockObject(DKGRAY_BRUSH)));
  if(!error.empty()){SetTextColor(dc,RGB(255,255,255));SetBkMode(dc,TRANSPARENT);DrawTextA(dc,error.c_str(),-1,&r,DT_CENTER|DT_VCENTER|DT_SINGLELINE);}
  else if(!dib.empty() && image.width && image.height){double scale=std::min(double(r.right)/image.width,double(r.bottom)/image.height);int dw=static_cast<int>(image.width*scale),dh=static_cast<int>(image.height*scale);SetStretchBltMode(dc,COLORONCOLOR);StretchDIBits(dc,(r.right-dw)/2,(r.bottom-dh)/2,dw,dh,0,0,static_cast<int>(image.width),static_cast<int>(image.height),dib.data(),&info,DIB_RGB_COLORS,SRCCOPY);}
  EndPaint(w,&ps);return 0;}
 case WM_DESTROY:PostQuitMessage(0);return 0;
 }return DefWindowProcW(w,m,wp,lp);
}
}
int WINAPI wWinMain(HINSTANCE instance,HINSTANCE,PWSTR,int show){
 try{
  int argc=0;LPWSTR*argv=CommandLineToArgvW(GetCommandLineW(),&argc);std::wstring path=argc>1?argv[1]:L"assets/RESOURCE.RES";if(argv)LocalFree(argv);
  // ResourceArchive accepts narrow paths; original default is ASCII. Convert explicitly to the active Windows codepage.
  int n=WideCharToMultiByte(CP_ACP,0,path.c_str(),-1,nullptr,0,nullptr,nullptr);std::string narrow(static_cast<std::size_t>(n),0);WideCharToMultiByte(CP_ACP,0,path.c_str(),-1,narrow.data(),n,nullptr,nullptr);narrow.pop_back();
  archive=std::make_unique<shin::ResourceArchive>(narrow);
  for(std::size_t i=0;i<archive->entries().size();++i){auto&name=archive->entries()[i].name;if(suffix(name,".app"))apps.push_back(i);if(suffix(name,".pal")){if(name=="shin.pal")palette=pals.size();pals.push_back(i);}}
  if(apps.empty()||pals.empty())throw std::runtime_error("archive has no APP or PAL resources");
  WNDCLASSW wc{};wc.lpfnWndProc=proc;wc.hInstance=instance;wc.lpszClassName=L"ShinResourceViewer";wc.hCursor=LoadCursorW(nullptr,MAKEINTRESOURCEW(32512));if(!RegisterClassW(&wc))throw std::runtime_error("cannot register window");
  HWND window=CreateWindowExW(0,wc.lpszClassName,L"SHIN resource preview",WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,CW_USEDEFAULT,1000,760,nullptr,nullptr,instance,nullptr);if(!window)throw std::runtime_error("cannot create window");load(window);ShowWindow(window,show);UpdateWindow(window);
  MSG msg;BOOL result;while((result=GetMessageW(&msg,nullptr,0,0))>0){TranslateMessage(&msg);DispatchMessageW(&msg);}return result<0?1:static_cast<int>(msg.wParam);
 }catch(const std::exception&e){MessageBoxA(nullptr,e.what(),"SHIN resource preview",MB_OK|MB_ICONERROR);return 1;}
}
