@echo off
setlocal

set "PROJECT_ROOT=%~dp0"
if "%PROJECT_ROOT:~-1%"=="\" set "PROJECT_ROOT=%PROJECT_ROOT:~0,-1%"
set "PACKAGED_EXE=%PROJECT_ROOT%\Packaged\Windows\TheLastRite.exe"

if not exist "%PACKAGED_EXE%" (
    echo Packaged game not found:
    echo %PACKAGED_EXE%
    exit /b 1
)

start "" "%PACKAGED_EXE%"
exit /b 0
