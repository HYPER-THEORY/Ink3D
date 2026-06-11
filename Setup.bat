@echo off
setlocal

if not exist libs\sdl (
    echo Downloading SDL2-2.32.10 ...
    curl -L -o libs\SDL2-devel-2.32.10-VC.zip https://github.com/libsdl-org/SDL/releases/download/release-2.32.10/SDL2-devel-2.32.10-VC.zip || (echo Failed to download SDL2-2.32.10 & pause & exit /b 1)
    echo Extracting...
    powershell -Command "Expand-Archive -Path 'libs\SDL2-devel-2.32.10-VC.zip' -DestinationPath 'libs' -Force"
    rename libs\SDL2-2.32.10 sdl
    rename libs\sdl\include sdl2
    del libs\SDL2-devel-2.32.10-VC.zip
    echo Setup SDL2 successfully
)

if not exist build mkdir build
cmake -G "Visual Studio 17 2022" -A x64 -S . -B build

pause
