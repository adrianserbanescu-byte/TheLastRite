@echo off
setlocal

set "PROJECT_ROOT=%OneDrive%\Desktop\alexs"
if not exist "%PROJECT_ROOT%\TheLastRite.uproject" set "PROJECT_ROOT=%~dp0"
if "%PROJECT_ROOT:~-1%"=="\" set "PROJECT_ROOT=%PROJECT_ROOT:~0,-1%"

echo Opening Unreal from:
echo %PROJECT_ROOT%
echo.

start "" "C:\Program Files\Epic Games\UE_5.4\Engine\Binaries\Win64\UnrealEditor.exe" "%PROJECT_ROOT%\TheLastRite.uproject"
