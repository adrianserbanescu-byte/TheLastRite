@echo off
setlocal

set "PROJECT_ROOT=%OneDrive%\Desktop\alexs"
if not exist "%PROJECT_ROOT%\Packaged\Windows\TheLastRite.exe" set "PROJECT_ROOT=%~dp0"
if "%PROJECT_ROOT:~-1%"=="\" set "PROJECT_ROOT=%PROJECT_ROOT:~0,-1%"

start "" "%PROJECT_ROOT%\Packaged\Windows\TheLastRite.exe"
