@echo off
setlocal

set "PROJECT_ROOT=%~dp0"
if "%PROJECT_ROOT:~-1%"=="\" set "PROJECT_ROOT=%PROJECT_ROOT:~0,-1%"

if not exist "%PROJECT_ROOT%\Packaged\Windows\TheLastRite.exe" (
    echo Packaged build not found:
    echo %PROJECT_ROOT%\Packaged\Windows\TheLastRite.exe
    exit /b 1
)

start "" "%PROJECT_ROOT%\Packaged\Windows\TheLastRite.exe"
