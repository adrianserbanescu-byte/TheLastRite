@echo off
setlocal

set "PROJECT_ROOT=%~dp0"
if "%PROJECT_ROOT:~-1%"=="\" set "PROJECT_ROOT=%PROJECT_ROOT:~0,-1%"
if not defined UE_ROOT set "UE_ROOT=C:\Program Files\Epic Games\UE_5.4"
set "EDITOR_EXE=%UE_ROOT%\Engine\Binaries\Win64\UnrealEditor.exe"

if not exist "%EDITOR_EXE%" (
    echo UnrealEditor.exe not found:
    echo %EDITOR_EXE%
    echo Set UE_ROOT to your Unreal Engine install path and try again.
    exit /b 1
)

echo Opening Unreal from:
echo %PROJECT_ROOT%
echo.

start "" "%EDITOR_EXE%" "%PROJECT_ROOT%\TheLastRite.uproject"
