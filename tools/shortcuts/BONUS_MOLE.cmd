@echo off
rem Requires Python 3 and a separately built Windows game; see tools/README.md.
python "%~dp0..\launch.py" --bonus mole %*
exit /b %errorlevel%
