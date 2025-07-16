@echo off
echo Cleaning build directory...
echo.

REM Switch to project root directory
cd /d "%~dp0\.."

if exist "build" (
    echo Removing build directory...
    rmdir /s /q build
    echo Build directory removed.
) else (
    echo Build directory does not exist.
)

echo.
echo Clean completed!
echo You can now run scripts\build.bat to rebuild the project.
pause
