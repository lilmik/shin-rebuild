@echo off
rem Pass -OriginalGame "path to your own original game directory".
powershell.exe -NoProfile -ExecutionPolicy RemoteSigned -File "%~dp0Launch_Original_Boss.ps1" %*
exit /b %errorlevel%
