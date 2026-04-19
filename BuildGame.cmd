@echo off
setlocal

set "PROJECT_ROOT=%~dp0"
if "%PROJECT_ROOT:~-1%"=="\" set "PROJECT_ROOT=%PROJECT_ROOT:~0,-1%"
set "PAUSE_AT_END=1"
if /I "%~1"=="--no-pause" set "PAUSE_AT_END="

echo Building game from:
echo %PROJECT_ROOT%
echo.

set "LOG_FILE=%PROJECT_ROOT%\Saved\UBT-TheLastRiteGame-%RANDOM%%RANDOM%.log"
echo Log file:
echo %LOG_FILE%
echo.

call "C:\Program Files\Epic Games\UE_5.4\Engine\Build\BatchFiles\Build.bat" TheLastRite Win64 Development -Project="%PROJECT_ROOT%\TheLastRite.uproject" -WaitMutex -log="%LOG_FILE%"
set "EXIT_CODE=%ERRORLEVEL%"

echo.
echo Done.
if defined PAUSE_AT_END pause
exit /b %EXIT_CODE%
