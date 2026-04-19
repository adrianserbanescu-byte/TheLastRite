@echo off
setlocal

set "PROJECT_ROOT=%~dp0"
if "%PROJECT_ROOT:~-1%"=="\" set "PROJECT_ROOT=%PROJECT_ROOT:~0,-1%"
if not defined UE_ROOT set "UE_ROOT=C:\Program Files\Epic Games\UE_5.4"
set "BUILD_BAT=%UE_ROOT%\Engine\Build\BatchFiles\Build.bat"

if not exist "%BUILD_BAT%" (
    echo Unreal Build.bat not found:
    echo %BUILD_BAT%
    echo Set UE_ROOT to your Unreal Engine install path and try again.
    exit /b 1
)

echo Building editor from:
echo %PROJECT_ROOT%
echo.

set "LOG_FILE=%PROJECT_ROOT%\Saved\UBT-TheLastRiteEditor-%RANDOM%%RANDOM%.log"
echo Log file:
echo %LOG_FILE%
echo.

call "%BUILD_BAT%" TheLastRiteEditor Win64 Development -Project="%PROJECT_ROOT%\TheLastRite.uproject" -WaitMutex -NoHotReload -log="%LOG_FILE%"
set "EXIT_CODE=%ERRORLEVEL%"

echo.
if "%EXIT_CODE%"=="0" (
    echo Done.
) else (
    echo Build failed with exit code %EXIT_CODE%.
)
exit /b %EXIT_CODE%
