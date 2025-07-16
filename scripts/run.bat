@echo off
echo Running YUVPlayer...
echo.

REM Switch to project root directory
cd /d "%~dp0\.."

if not exist "build\Debug\YUVPlayer.exe" (
    echo YUVPlayer.exe not found! Please build the project first.
    echo Run scripts\build.bat to build the project.
    pause
    exit /b 1
)

echo Starting YUVPlayer...
start "YUVPlayer" "build\Debug\YUVPlayer.exe"

echo YUVPlayer started.
echo.
