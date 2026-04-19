@echo off
setlocal

set "PROJECT_ROOT=%~dp0"
if "%PROJECT_ROOT:~-1%"=="\" set "PROJECT_ROOT=%PROJECT_ROOT:~0,-1%"
set "PACKAGE_SCRIPT=%PROJECT_ROOT%\PackageGame.cmd"
set "SMOKE_SCRIPT=%PROJECT_ROOT%\SmokeTestPackagedGame.cmd"

if not exist "%PACKAGE_SCRIPT%" (
    echo Package script not found:
    echo %PACKAGE_SCRIPT%
    exit /b 1
)

if not exist "%SMOKE_SCRIPT%" (
    echo Smoke test script not found:
    echo %SMOKE_SCRIPT%
    exit /b 1
)

echo Validating package lane from:
echo %PROJECT_ROOT%
echo.

call "%PACKAGE_SCRIPT%" --no-pause
set "EXIT_CODE=%ERRORLEVEL%"
if not "%EXIT_CODE%"=="0" (
    echo.
    echo Package step failed.
    exit /b %EXIT_CODE%
)

call "%SMOKE_SCRIPT%"
set "EXIT_CODE=%ERRORLEVEL%"
if not "%EXIT_CODE%"=="0" (
    echo.
    echo Smoke test step failed.
    exit /b %EXIT_CODE%
)

echo.
echo Package lane validation passed.
exit /b 0
