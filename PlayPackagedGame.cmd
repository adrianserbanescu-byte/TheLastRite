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

echo Packaged runtime helper:
echo %PACKAGED_EXE%
echo.
echo Warning: this packaged build is not the current source of truth while M4 remains blocked.
echo Current verified prototype runtime path:
echo %PROJECT_ROOT%\Binaries\Win64\TheLastRite.exe
echo.
echo Use this helper only for explicit packaged-build checks.
echo.

taskkill /IM TheLastRite.exe /F >nul 2>&1
taskkill /IM TheLastRite-Win64-Shipping.exe /F >nul 2>&1

start "" "%PACKAGED_EXE%"
exit /b 0
