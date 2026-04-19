@echo off
setlocal

set "PROJECT_ROOT=%~dp0"
if "%PROJECT_ROOT:~-1%"=="\" set "PROJECT_ROOT=%PROJECT_ROOT:~0,-1%"

echo Validating package lane from:
echo %PROJECT_ROOT%
echo.

call "%PROJECT_ROOT%\PackageGame.cmd" --no-pause
set "EXIT_CODE=%ERRORLEVEL%"
if not "%EXIT_CODE%"=="0" (
    echo.
    echo Package step failed.
    exit /b %EXIT_CODE%
)

call "%PROJECT_ROOT%\SmokeTestPackagedGame.cmd"
set "EXIT_CODE=%ERRORLEVEL%"
if not "%EXIT_CODE%"=="0" (
    echo.
    echo Smoke test step failed.
    exit /b %EXIT_CODE%
)

echo.
echo Package lane validation passed.
exit /b 0
