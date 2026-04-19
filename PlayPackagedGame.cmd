@echo off
setlocal

set "PROJECT_ROOT=%~dp0"
if "%PROJECT_ROOT:~-1%"=="\" set "PROJECT_ROOT=%PROJECT_ROOT:~0,-1%"
set "LAUNCHER=%PROJECT_ROOT%\Packaged\Windows\TheLastRite.exe"
set "CHECK_ONLY="
if /I "%~1"=="--check-only" set "CHECK_ONLY=1"

echo Launching packaged build from:
echo %PROJECT_ROOT%
echo.
echo Launcher path:
echo %LAUNCHER%
echo.

if not exist "%LAUNCHER%" (
    echo Packaged build not found:
    echo %LAUNCHER%
    echo.
    echo Run PackageGame.cmd or ValidatePackageLane.cmd from:
    echo %PROJECT_ROOT%
    exit /b 1
)

if defined CHECK_ONLY (
    echo Launcher check passed.
    exit /b 0
)

for /f "usebackq delims=" %%I in (`powershell -NoProfile -ExecutionPolicy Bypass -Command "$p = Start-Process -FilePath '%LAUNCHER%' -PassThru; $p.Id"`) do set "LAUNCH_PID=%%I"

if not defined LAUNCH_PID (
    echo Failed to launch packaged build.
    exit /b 1
)

echo Started packaged build with PID %LAUNCH_PID%.
exit /b 0
