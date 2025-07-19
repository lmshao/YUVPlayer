@echo off
echo Building YUVPlayer (Release)...
echo.

REM Switch to project root directory
cd /d "%~dp0\.."

REM Check if CMake is installed
where cmake >nul 2>&1
if %errorlevel% neq 0 (
    echo CMake is not installed or not in PATH!
    echo Please install CMake from https://cmake.org/download/
    echo Or install via winget: winget install Kitware.CMake
    pause
    exit /b 1
)

echo Step 1: Configuring CMake for Release...
cmake -B build -DCMAKE_BUILD_TYPE=Release -G "Visual Studio 17 2022" -A x64

if %errorlevel% neq 0 (
    echo CMake configuration failed!
    pause
    exit /b 1
)

echo.
echo Step 2: Building Release version...
cmake --build build --config Release

if %errorlevel% neq 0 (
    echo Build failed!
    pause
    exit /b 1
)

echo.
echo Release build completed successfully!
echo Executable location: build\Release\YUVPlayer.exe
echo.
echo You can now run the program with:
echo   .\build\Release\YUVPlayer.exe
echo.
