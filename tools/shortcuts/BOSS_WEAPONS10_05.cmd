@echo off
rem Developer shortcut: see tools/README.md; --fresh-start is the launcher default.
python "%~dp0..\launch.py" --boss 5 %*
exit /b %errorlevel%
