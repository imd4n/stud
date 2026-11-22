@echo off
setlocal
cd /d "%~dp0"

echo Building PlaneSim.exe ...
g++ -std=c++11 -O2 -Wall -Wextra planeMain.cpp plane.cpp -lgdi32 -o PlaneSim.exe

if errorlevel 1 (
    echo.
    echo Build failed.
    pause
    exit /b 1
)

echo Running ...
PlaneSim.exe
