@echo off
setlocal

set "PROJECT_ROOT=%~dp0"
if "%PROJECT_ROOT:~-1%"=="\" set "PROJECT_ROOT=%PROJECT_ROOT:~0,-1%"

echo Building editor from:
echo %PROJECT_ROOT%
echo.

set "LOG_FILE=%PROJECT_ROOT%\Saved\UBT-TheLastRiteEditor-%RANDOM%%RANDOM%.log"
echo Log file:
echo %LOG_FILE%
echo.

call "C:\Program Files\Epic Games\UE_5.4\Engine\Build\BatchFiles\Build.bat" TheLastRiteEditor Win64 Development -Project="%PROJECT_ROOT%\TheLastRite.uproject" -WaitMutex -NoHotReload -log="%LOG_FILE%"

echo.
echo Done.
pause
