@echo off
setlocal

set "PROJECT_ROOT=%~dp0"
if "%PROJECT_ROOT:~-1%"=="\" set "PROJECT_ROOT=%PROJECT_ROOT:~0,-1%"
set "PAUSE_AT_END=1"
if /I "%~1"=="--no-pause" set "PAUSE_AT_END="
set "UPROJECT=%PROJECT_ROOT%\TheLastRite.uproject"
set "UE_BUILD_BAT=C:\Program Files\Epic Games\UE_5.4\Engine\Build\BatchFiles\Build.bat"

if not exist "%UPROJECT%" (
    echo Project file not found:
    echo %UPROJECT%
    set "EXIT_CODE=1"
    goto :done
)

if not exist "%UE_BUILD_BAT%" (
    echo Unreal build script not found:
    echo %UE_BUILD_BAT%
    set "EXIT_CODE=1"
    goto :done
)

echo Building editor from:
echo %PROJECT_ROOT%
echo.

set "LOG_FILE=%PROJECT_ROOT%\Saved\UBT-TheLastRiteEditor-%RANDOM%%RANDOM%.log"
echo Log file:
echo %LOG_FILE%
echo.

call "%UE_BUILD_BAT%" TheLastRiteEditor Win64 Development -Project="%UPROJECT%" -WaitMutex -NoHotReload -log="%LOG_FILE%"
set "EXIT_CODE=%ERRORLEVEL%"

:done
echo.
if "%EXIT_CODE%"=="0" (
    echo Done.
) else (
    echo Build failed with exit code %EXIT_CODE%.
)
if defined PAUSE_AT_END pause
exit /b %EXIT_CODE%
