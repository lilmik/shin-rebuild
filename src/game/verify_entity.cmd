rem Historical build helper: paths may be machine-specific; prefer tools/quick_check.py and tools/README.md.
@echo off
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat" >nul
cl /nologo /EHsc /std:c++17 /W4 entity_controller.cpp verify_entity.cpp /Fe:verify_entity.exe
if errorlevel 1 exit /b 1
verify_entity.exe
