rem Historical build helper: paths may be machine-specific; prefer tools/quick_check.py and tools/README.md.
@echo off
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat" >nul
cl /nologo /std:c++17 /EHsc /W4 player_object_response.cpp verify_object_response.cpp /Fe:verify_object_response.exe
