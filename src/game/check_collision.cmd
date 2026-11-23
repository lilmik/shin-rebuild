rem Historical build helper: paths may be machine-specific; prefer tools/quick_check.py and tools/README.md.
@echo off
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat" >nul
cl /nologo /std:c++17 /EHsc /W4 /c stage_scene.cpp collision_world.cpp
