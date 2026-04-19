@echo off
setlocal

set "PROJECT_ROOT=%~dp0"
if "%PROJECT_ROOT:~-1%"=="\" set "PROJECT_ROOT=%PROJECT_ROOT:~0,-1%"
set "UPROJECT=%PROJECT_ROOT%\TheLastRite.uproject"
set "UE_EDITOR=C:\Program Files\Epic Games\UE_5.4\Engine\Binaries\Win64\UnrealEditor.exe"

if not exist "%UPROJECT%" (
    echo Project file not found:
    echo %UPROJECT%
    exit /b 1
)

if not exist "%UE_EDITOR%" (
    echo Unreal Editor not found:
    echo %UE_EDITOR%
    exit /b 1
)

echo Opening Unreal from:
echo %PROJECT_ROOT%
echo.
echo Project file:
echo %UPROJECT%
echo.
echo Unreal Editor:
echo %UE_EDITOR%
echo.

start "" "%UE_EDITOR%" "%UPROJECT%"
