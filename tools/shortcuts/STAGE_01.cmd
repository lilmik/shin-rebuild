@echo off
rem Developer shortcut: see tools/README.md; --fresh-start is the launcher default.
python "%~dp0..\launch.py" --stage 1 %*
exit /b %errorlevel%
